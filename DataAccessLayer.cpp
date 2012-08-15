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
#include <vector>
#include <map>
#include <hiredis/hiredis.h>
#include "DataAccessLayer.h"
#include "utils/TurnLeftLib/enumparser.h"

using namespace DAL::Redis_Keys;

using namespace DAL::Account_Values;
using namespace DAL::Debt_Values;
using namespace DAL::Income_Values;

namespace DAL{

	redisContext *c = redisConnect("localhost",6379); //TODO Is this ok?  This means that the connection is always open...
	bool notInitialized = true;

	std::map<AccountValues, std::string> accountKeys;
	std::map<DebtValues, std::string> debtKeys;
	std::map<IncomeValues, std::string> incomeKeys;

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

			notInitialized = false;
		}
	}


	/**TODO Create a clean way to check the output of redisCommands
	 * for failure and implement error handling on all redisCommand calls*/

	//TODO should I make some DRY optimizations to the hashNames to prevent typo slip-ups?


	/* Function used for generating a generic ID.
	 * First checks to see if there's an unregistered ID
	 * If there isn't, it creates a new one.*/
	std::string createID(std::string unregisteredList, std::string incrementingID){
		redisReply *reply;

		reply = redisCommand(c, ("lpop "+ unregisteredList).c_str()); //attempt to get an unregistered ID
		std::stringstream out;

		if(reply->type == REDIS_REPLY_NIL){ //if the returned ID was NIL (IE the list was emtpy)
			reply = redisCommand(c, ("incr "+ incrementingID).c_str()); //create a new ID from the id counter
			out << reply -> integer;
		} else {//if the list did return an ID
			out << reply -> str;
		}

		freeReplyObject(reply);

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
		//TODO check if email is already registered in idLookupHash!
		std::string accountID = createAccountID();//accountID for this user.

		for ( std::map<AccountValues, std::string>::iterator it = vals.begin(); it != vals.end(); it++)
		{
		   redisCommand(c, ("hset " + accountInfo(accountID) + " " + accountKeys[it->first] + " " + vals[it->first]).c_str());
		}

		redisCommand(c, ("hset idLookupHash " + vals[email] + " " + accountID).c_str());//table for getting accountID from email

		return accountID;
	}

	/*Adds a source of income to a given user in Redis*/
	std::string addIncome(std::string accountID, std::map<IncomeValues, std::string> &vals){
		std::string incomeID = createIncomeID(accountID);
		for ( std::map<IncomeValues, std::string>::iterator it = vals.begin(); it != vals.end(); it++)
		{
		   redisCommand(c, ("hset " + incomeInfo(accountID, incomeID) + " " + incomeKeys[it->first] + " " + vals[it->first]).c_str());
		}
		return incomeID;
	}

	/*Adds a source of debt to a given user in Redis*/
	std::string addDebt(std::string accountID, std::map<DebtValues, std::string> &vals){
		std::string debtID = createDebtID(accountID);
		for ( std::map<DebtValues, std::string>::iterator it = vals.begin(); it != vals.end(); it++)
		{
		   redisCommand(c, ("hset " + debtInfo(accountID, debtID) + " " + debtKeys[it->first] + " " + vals[it->first]).c_str());
		}
		return debtID;
	}

	//gets a user ID from an email address
	std::string getIDFromEmail(std::string email){
		return redisCommand(c, ("hget "+ idLookupHash() + " " + email).c_str())->str;
	}


	//TODO Create the delete user, delete income, and delete debt code

	/* Deletes a user from redis including:
	 * his account information
	 * his debt information
	 * his income information*/
	void deleteAccount(std::string accountID){
		redisReply *mainReply, *emailReply ;
		std::string item;
		std::string email;

		mainReply = redisCommand(c, ("KEYS account:"+accountID+":*").c_str());
		bool userExists = mainReply->elements > 0;

		if(userExists){
			emailReply = redisCommand(c,("HGET " + accountInfo(accountID) + " email").c_str());
			if(emailReply->type == REDIS_REPLY_STRING) {
				email = emailReply ->str;
			} else {
				//TODO: LOG AN ERROR!
			}

			//delete all user info
			for(unsigned int i=0; i < mainReply->elements; i++){
				item = mainReply->element[i] ->str;
				redisCommand(c, ("DEL "+item).c_str());
			}

			//add ID to unregistered
			redisCommand(c, ("LPUSH " + unregisteredUserIDs() + " "+accountID).c_str());

			//remove email from lookup table
			redisCommand(c, ("HDEL " + idLookupHash() + " "+email).c_str());
		}
	}

	/* Deletes a given income source for a user and adds the incomeID
	 * to that user's unregistered income id list.*/
	void deleteIncome(std::string accountID, std::string incomeID){
		redisCommand(c, ("DEL " + incomeInfo(accountID, incomeID)).c_str());
		redisCommand(c, ("LPUSH " + unregisteredIncomeIDs(accountID) +" " + incomeID).c_str());
	}

	/* deletes a given debt source for a user and adds the debtID
	 * to that user's unregistered debt id list.*/
	void deleteDebt(std::string accountID, std::string debtID){
		redisCommand(c, ("DEL " + debtInfo(accountID, debtID)).c_str());
		redisCommand(c, ("LPUSH " + unregisteredDebtIDs(accountID) +" " + debtID).c_str());
	}

	void updateAccount(std::string accountID, AccountValues field, std::string value){
		redisCommand(c,"HSET account:%s:info %s %s",accountID.c_str(),accountKeys[field].c_str(),value.c_str());
	}

	void updateDebt(std::string accountID, std::string debtID, DebtValues field, std::string value){
		redisCommand(c,"HSET account:%s:debt:%s %s %s",accountID.c_str(),debtID.c_str(),debtKeys[field].c_str(),value.c_str());
	}

	void updateIncome(std::string accountID, std::string incomeID, IncomeValues field, std::string value){
		redisCommand(c,"HSET account:%s:income:%s %s %s",accountID.c_str(),incomeID.c_str(),incomeKeys[field].c_str(),value.c_str());
	}

}
