/*
 * DataAccessLayer.cpp
 *
 *  Created on: Aug 1, 2012
 *      Author: adamtraub
 */

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <hiredis/hiredis.h>
#include "DataAccessLayer.h"

using namespace DAL::Redis_Keys;

using namespace DAL::Account_Values;
using namespace DAL::Debt_Values;
using namespace DAL::Income_Values;

namespace DAL{

	bool notInitialized = true;

	/*Maps that relate enumerators to redis database fields*/
	std::map<AccountValues, std::string> accountKeys;
	std::map<DebtValues, std::string> debtKeys;
	std::map<IncomeValues, std::string> incomeKeys;

	std::string database;
	int port;

	/*Necessary to initialize the DAL, only needs to be called once*/
	void initialize(){
		if(notInitialized){
			accountKeys[name] = "name";
			accountKeys[email] = "email";
			accountKeys[phone] = "phone";
			accountKeys[monthlyEmail] = "monthlyEmail";
			accountKeys[emailOnUpdate] = "emailOnUpdate";

			debtKeys[debt_source] = "debt_source";
			debtKeys[balance] = "balance";
			debtKeys[apr] = "apr";
			debtKeys[minimum_payment] = "minimum_payment";
			debtKeys[extra_payment] = "extra_payment";
			debtKeys[due_date] = "due_date";
			debtKeys[payment_scheduled] = "payment_scheduled";
			debtKeys[payment_processed] = "payment_processed";

			incomeKeys[income_source] = "income_source";
			incomeKeys[amount] = "amount";
			incomeKeys[savings] = "savings";

			database = "localhost";
			port = 6379;

			notInitialized = false;
		}
	}


	/**TODO Create a clean way to check the output of redisCommands for failure
	 * and implement error handling on all redisCommand calls*/

	/* Function used for generating a generic ID.
	 * First checks to see if there's an unregistered ID
	 * If there isn't, it creates a new one.*/
	std::string createID(std::string unregisteredList, std::string incrementingID){
		redisReply *reply;
		std::stringstream out;
		redisContext *c = redisConnect(database.c_str(), port);

		reply = redisCommand(c, "LLEN %s", unregisteredList.c_str());

		if(reply->integer > 0){//if there is an unregistered ID
			reply = redisCommand(c, "LPOP %s", unregisteredList.c_str());
			out << reply -> str;
		} else {//if there are no unregistered ID's
			reply = redisCommand(c, "INCR %s",incrementingID.c_str()); //create a new ID from the id counter
			out << reply -> integer;
		}

		freeReplyObject(reply);
		redisFree(c);

		return out.str();
	}

	/*Creates a unique ID for a given user account
	 *returns the newly created user's id*/
	std::string createAccountID(){
		return createID(unregisteredUserIDs(),lastUserID());
	}

	/*Creates a unique ID for a given source of income for a given user*/
	std::string createIncomeID(std::string accountID){
		return createID(unregisteredIncomeIDs(accountID),lastIncomeID(accountID));
	}

	/*Creates a unique ID for a given source of debt for a given user*/
	std::string createDebtID(std::string accountID){
		return createID(unregisteredDebtIDs(accountID),lastDebtID(accountID));
	}

	/*Creates a new User in redis*/
	std::string createAccount(std::map<AccountValues, std::string> &vals){
		redisContext *c = redisConnect(database.c_str(), port);

		if(redisCommand(c,"HGET %s %s",idLookupHash().c_str(),vals[email].c_str())->type != REDIS_REPLY_NIL)
			return "-1"; //returns an invalid ID to signify an error, namely that the email already belongs to a registered user

		std::string accountID = createAccountID();//accountID for this user.

		for ( std::map<AccountValues, std::string>::iterator it = vals.begin(); it != vals.end(); it++)
		{
		   redisCommand(c, "HSET %s %s %s",accountInfo(accountID).c_str(), accountKeys[it->first].c_str(), vals[it->first].c_str());
		}

		//Add email and account info to id lookup table
		redisCommand(c, "HSET %s %s %s",idLookupHash().c_str(), vals[email].c_str(), accountID.c_str());//table for getting accountID from email

		redisFree(c);

		return accountID;
	}

	/*Adds a source of debt to a given user in Redis*/
	std::string addDebt(std::string accountID, std::map<DebtValues, std::string> &vals){
		std::string debtID = createDebtID(accountID);
		redisContext *c = redisConnect(database.c_str(), port);
		for ( std::map<DebtValues, std::string>::iterator it = vals.begin(); it != vals.end(); it++)
		{
		   redisCommand(c,"HSET %s %s %s",debtInfo(accountID, debtID).c_str(), debtKeys[it->first].c_str(), vals[it->first].c_str());
		}
		redisFree(c);
		return debtID;
	}

	/*Adds a source of income to a given user in Redis*/
	std::string addIncome(std::string accountID, std::map<IncomeValues, std::string> &vals){
		std::string incomeID = createIncomeID(accountID);
		redisContext *c = redisConnect(database.c_str(), port);
		for ( std::map<IncomeValues, std::string>::iterator it = vals.begin(); it != vals.end(); it++)
		{
		   redisCommand(c, "HSET %s %s %s", incomeInfo(accountID, incomeID).c_str(), incomeKeys[it->first].c_str(), vals[it->first].c_str());
		}
		redisFree(c);
		return incomeID;
	}

	/*gets a user ID from an email address*/
	std::string getIDFromEmail(std::string email){
		redisContext *c = redisConnect(database.c_str(), port);
		std::string id = redisCommand(c, "HGET %s %s",idLookupHash().c_str(), email.c_str())->str;
		redisFree(c);
		return id;
	}

	/* Deletes a user from redis including:
	 * his account information
	 * his debt information
	 * his income information*/
	void deleteAccount(std::string accountID){
		redisReply *mainReply, *emailReply ;
		std::string item;
		std::string email;
		redisContext *c = redisConnect(database.c_str(), port);

		mainReply = redisCommand(c, "KEYS account:%s:*",accountID.c_str());
		bool userExists = mainReply->elements > 0;

		if(userExists){
			emailReply = redisCommand(c, "HGET %s email",accountInfo(accountID).c_str());
			if(emailReply->type == REDIS_REPLY_STRING) {
				email = emailReply ->str;
			} else {
				//TODO: LOG AN ERROR! The email wasn't in the lookup table!
			}
			//delete all user info
			for(unsigned int i=0; i < mainReply->elements; i++){
				item = mainReply->element[i] ->str;
				redisCommand(c, "DEL %s",item.c_str());
			}

			//add ID to unregistered id list
			redisCommand(c, "LPUSH %s %s",unregisteredUserIDs().c_str(), accountID.c_str());

			//remove email from lookup table
			redisCommand(c, "HDEL %s %S",idLookupHash().c_str(), email.c_str());

			freeReplyObject(emailReply);
		}
		freeReplyObject(mainReply);
		redisFree(c);
	}

	/* Deletes a given income source for a user and adds the incomeID
	 * to that user's unregistered income id list.*/
	void deleteIncome(std::string accountID, std::string incomeID){
		redisContext *c = redisConnect(database.c_str(), port);
		redisCommand(c, "DEL %s",incomeInfo(accountID, incomeID).c_str());
		redisCommand(c, "LPUSH %s %s",unregisteredIncomeIDs(accountID).c_str(), incomeID.c_str());
		redisFree(c);
	}

	/* deletes a given debt source for a user and adds the debtID
	 * to that user's unregistered debt id list.*/
	void deleteDebt(std::string accountID, std::string debtID){
		redisContext *c = redisConnect(database.c_str(), port);
		redisCommand(c, "DEL %s",debtInfo(accountID, debtID).c_str());
		redisCommand(c, "LPUSH %s %s",unregisteredDebtIDs(accountID).c_str(), debtID.c_str());
		redisFree(c);
	}

	/**
	 * Given an ID and a map of accountValue String pairs,
	 * this function updates a corresponding account*/
	bool updateAccount(std::string accountID, std::map<AccountValues, std::string> &vals){
		bool badEmail = false;
		redisContext *c = redisConnect(database.c_str(), port);
		for ( std::map<AccountValues, std::string>::iterator it = vals.begin(); it != vals.end(); it++)
		{
			if(it->first != email) { //if the value we're looking at is NOT email
				redisCommand(c, "HSET %s %s %s",accountInfo(accountID).c_str(), accountKeys[it->first].c_str(), vals[it->first].c_str());
			} else { //if we're trying to update the email
				 //if the new email isn't already in the lookup hash and the new email isn't blank
				if((redisCommand(c,"HGET %s %s",idLookupHash().c_str() ,vals[email].c_str())->type == REDIS_REPLY_NIL) && (vals[email].compare("") != 0)){
					//get the old email address
					std::string oldEmail = redisCommand(c, "HGET %s email",accountInfo(accountID).c_str())->str;
					//remove old email address from lookuphash
					redisCommand(c,"HDEL %s %s",idLookupHash().c_str(), oldEmail.c_str());
					//update the account's email
					redisCommand(c, "HSET %s %s %s",accountInfo(accountID).c_str(), accountKeys[email].c_str(), vals[email].c_str());
					//update the lookupHash's entry for the new email
					redisCommand(c,"HSET %s %s %s",idLookupHash().c_str(), vals[email].c_str(), accountID.c_str());
				} else {
					badEmail = true;
				}
			}
		}
		redisFree(c);
		return badEmail;
	}

	/*Updates debt info*/
	void updateDebt(std::string accountID, std::string debtID, std::map<DebtValues, std::string> &vals){
		redisContext *c = redisConnect(database.c_str(), port);
		for ( std::map<DebtValues, std::string>::iterator it = vals.begin(); it != vals.end(); it++)
		{
		   redisCommand(c,"HSET %s %s %s",debtInfo(accountID, debtID).c_str(), debtKeys[it->first].c_str(), vals[it->first].c_str());
		}
		redisFree(c);
	}

	/*Updates income Info*/
	void updateIncome(std::string accountID, std::string incomeID, std::map<IncomeValues, std::string> &vals){
		redisContext *c = redisConnect(database.c_str(), port);
		for ( std::map<IncomeValues, std::string>::iterator it = vals.begin(); it != vals.end(); it++)
		{
		   redisCommand(c, "HSET %s %s %s", incomeInfo(accountID, incomeID).c_str(), incomeKeys[it->first].c_str(), vals[it->first].c_str());
		}
		redisFree(c);
	}

	std::map<AccountValues, std::string> getAccountInfo(std::string accountID){
		redisContext *c = redisConnect(database.c_str(), port);
		std::map<AccountValues, std::string> accountData;

		for ( std::map<AccountValues, std::string>::iterator it = accountKeys.begin(); it != accountKeys.end(); it++){
			accountData[it->first] = redisCommand(c, "HGET %s %s", accountInfo(accountID).c_str(), accountKeys[it->first].c_str())->str;
		}

		redisFree(c);
		return accountData;
	}

	std::vector<std::map<DebtValues, std::string> > getDebtInfo(std::string accountID){
		redisContext *c = redisConnect(database.c_str(), port);
		std::vector<std::map<DebtValues, std::string> > debtData;
		redisReply *debtNames;

		debtNames = redisCommand(c, "KEYS %s", debtInfo(accountID,"*").c_str());

		for(unsigned int i=0; i < debtNames->elements; i++){
			std::map<DebtValues, std::string> tempMap; //purposely re-define the variable at the loop scope each iteration
			for ( std::map<DebtValues, std::string>::iterator it = debtKeys.begin(); it != debtKeys.end(); it++){
				tempMap[it->first] = redisCommand(c, "HGET %s %s", debtNames->element[i]->str, debtKeys[it->first].c_str())->str;
			}
			debtData.push_back(tempMap);
		}

		redisFree(c);
		return debtData;
	}

	std::vector<std::map<IncomeValues, std::string> > getIncomeInfo(std::string accountID){
		redisContext *c = redisConnect(database.c_str(), port);
		std::vector<std::map<IncomeValues, std::string> > incomeData;
		redisReply *incomeNames;

		incomeNames = redisCommand(c, "KEYS %s", incomeInfo(accountID,"*").c_str());

		for(unsigned int i=0; i < incomeNames->elements; i++){
			std::map<IncomeValues, std::string> tempMap; //purposely re-define the variable at the loop scope each iteration
			std::cout<<incomeNames->element[i]->str<<std::endl;
			for ( std::map<IncomeValues, std::string>::iterator it = incomeKeys.begin(); it != incomeKeys.end(); it++){
				std::cout<<incomeKeys[it->first]<<std::endl;
				tempMap[it->first] = redisCommand(c, "HGET %s %s", incomeNames->element[i]->str, incomeKeys[it->first].c_str())->str;
			}
			incomeData.push_back(tempMap);
		}

		redisFree(c);
		return incomeData;
	}

}
