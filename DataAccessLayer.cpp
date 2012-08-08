/*
 * DataAccessLayer.cpp
 *
 *  Created on: Aug 1, 2012
 *      Author: adamtraub
 */

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <hiredis/hiredis.h>
#include "DataAccessLayer.h"

using namespace DAL::redisKeys;

namespace DAL{

	redisContext *c = redisConnect("localhost",6379); //TODO Is this ok?  This means that the connection is always open...

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
		}else{//if the list did return an ID
			out << reply -> str;
		}

		freeReplyObject(reply);

		return out.str();
	}

	/*Creates a unique ID for a given user account
	 *returns the newly created user's id*/
	std::string createUserID(){
		return createID(unregisteredUserIDs(),lastUserID());
	}

	/*Creates a unique ID for a given source of income for a given user*/
	std::string createIncomeID(std::string userID){
		return createID(unregisteredIncomeIDs(userID),lastIncomeID(userID));
	}

	/*Creates a unique ID for a given source of debt for a given user*/
	std::string createDebtID(std::string userID){
		return createID(unregisteredDebtIDs(userID),lastDebtID(userID));
	}

	/*Adds a user to redis*/
	std::string createUser(std::string email, std::string name, std::string phone, bool monthlyEmail, bool emailOnUpdate){

		//TODO check if email is already registered in idLookupHash!

		std::string userID = createUserID();//userID for this user.
		std::string hashName = accountInfo(userID); //name of hash that will store this user's account information

		redisCommand(c, ("hset " + hashName + " name "          + name).c_str());
		redisCommand(c, ("hset " + hashName + " email "         + email).c_str());
		redisCommand(c, ("hset " + hashName + " phone "         + phone).c_str());
		redisCommand(c, ("hset " + hashName + " monthlyEmail "  + ( monthlyEmail ? "yes":"no")).c_str());
		redisCommand(c, ("hset " + hashName + " emailOnUpdate " + (emailOnUpdate ? "yes":"no")).c_str());

		redisCommand(c, ("hset idLookupHash " + email + " " + userID).c_str());//table for getting userID from email

		return userID;
	}

	/*add a source of income for a given user*/
	std::string addIncome(std::string userID, std::string sourceName, std::string amount, std::string savings){

		std::string incomeID = createIncomeID(userID);
		std::string hashName = incomeInfo(userID, incomeID);

		redisCommand(c, ("hset " + hashName + " source "  + sourceName).c_str());
		redisCommand(c, ("hset " + hashName + " amount "  + amount).c_str());
		redisCommand(c, ("hset " + hashName + " savings " + savings).c_str());

		return incomeID;
	}

	/*add a source of debt for a given user*/
	std::string addDebt(std::string userID, std::string accountName, std::string balance, std::string apr,
					std::string minimumPayment, std::string extraPayment, std::string dueDate,
					bool paymentScheduled, bool paymentProcessed){

		std::string debtID = createDebtID(userID);
		std::string hashName = debtInfo(userID, debtID);

		redisCommand(c, ("hset " + hashName + " account "          + accountName).c_str());
		redisCommand(c, ("hset " + hashName + " balance "          + balance).c_str());
		redisCommand(c, ("hset " + hashName + " apr "              + apr).c_str());
		redisCommand(c, ("hset " + hashName + " minimumPayment "   + minimumPayment).c_str());
		redisCommand(c, ("hset " + hashName + " extraPayment "     + extraPayment).c_str());
		redisCommand(c, ("hset " + hashName + " dueDate "          + dueDate).c_str());
		redisCommand(c, ("hset " + hashName + " paymentScheduled " + (paymentScheduled ? "yes":"no")).c_str());
		redisCommand(c, ("hset " + hashName + " paymentProcessed " + (paymentProcessed ? "yes":"no")).c_str());

		return debtID;
	}

	//gets a user ID from an email address
	std::string getIDFromEmail(std::string email){
		return redisCommand(c, ("hget "+ idLookupHash() + " " + email).c_str())->str;
	}







	    /* Suggest using an array with enumerated types here to make your life easier, especially since it's a known
	     * quantity of strings:
	     *
	     * namespace Debt_Values {
	     *     enum DebtValues {
	     *         account, balance, apr, minimum_payment, extra_payment, due_date, payment_scheduled, payment_processed
	     *     };
	     *     const int NUM_DEBT_VALUES = 8;
	     * }
	     *
	     * std::string addDebt(std::string[] debtValues) {
	     *  //...
	     *  for (int i = 0; i < NUM_DEBT_VALUES; i++)
	     *  {
	     *      redisCommand(c, ("hset" + hashName + " account " + debtValues[i]).c_str());
	     *  }
	     *
	     *
	     *
	     *
	     */










	//TODO Create the delete user, delete income, and delete debt code

	/* Deletes a user from redis including:
	 * his account information
	 * his debt information
	 * his income information*/
	void deleteUser(std::string userID){
		redisReply *reply;
		std::string item;
		std::string email = redisCommand(c,("HGET " + accountInfo(userID) + " email").c_str())->str;

		reply = redisCommand(c, ("KEYS user:"+userID+":*").c_str());
		bool userExists = reply->elements > 0;

		if(userExists){
			//delete all user info
			for(int i=0; i < reply->elements; i++){
				item = reply->element[i] ->str;
				redisCommand(c, ("DEL "+item).c_str());
			}

			//add ID to unregistered
			redisCommand(c, ("LPUSH " + unregisteredUserIDs() + " "+userID).c_str());

			//remove email from lookup table
			redisCommand(c, ("HDEL " + idLookupHash() + " "+email).c_str());
		}
	}

	/* Deletes a given income source for a user and adds the incomeID
	 * to that user's unregistered income id list.*/
	void deleteIncome(std::string userID, std::string incomeID){
		redisCommand(c, ("DEL " + incomeInfo(userID, incomeID)).c_str());
		redisCommand(c, ("LPUSH " + unregisteredIncomeIDs(userID)).c_str());
	}

	/* deletes a given debt source for a user and adds the debtID
	 * to that user's unregistered debt id list.*/
	void deleteDebt(std::string userID, std::string debtID){
		redisCommand(c, ("DEL " + debtInfo(userID, debtID)).c_str());
		redisCommand(c, ("LPUSH " + unregisteredDebtIDs(userID)).c_str());
	}
}
