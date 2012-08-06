/*
 * DataAccessLayer.cpp
 *
 *  Created on: Aug 1, 2012
 *      Author: adamtraub
 */

#include <stdio.h>
#include <sstream>
#include <hiredis/hiredis.h>
#include "DataAccessLayer.h"


namespace DAL{

	redisContext *c = redisConnect("localhost",6379);

//TODO Create a clean way to check the output of redisCommands for failure and implement error handling on all redisCommands


	/* Function used for generating a generic ID.
	 * First checks to see if there's an unregistered ID
	 * If there isn't, it creates a new one.*/
	std::string createID(std::string unregisteredList, std::string incrementingID){
		redisReply *reply;

		reply = redisCommand(c, (char*) ("lpop "+ unregisteredList)); //check unregistered ID's

		if(reply->type == REDIS_REPLY_NIL){ //if there are no id's from the unregistered list
			reply = redisCommand(c, (char*) ("incr "+ incrementingID)); //create a new ID from the id counter
		}

		std::string output = reply -> str;
		freeReplyObject(reply);

		return output;
	}

	/*Creates a unique ID for a given user account
	 * returns the newly created user's id*/
	std::string createUserID(){
		return createID("unregisteredUserIDs","lastUserID");
	}

	/*creates a unique ID for a given source of income for a given user*/
	std::string createIncomeID(std::string userID){
		return createID(userID+":unregisteredIncomeIDs",userID+":lastIncomeID");
	}

	/*creates a unique ID for a given source of debt for a given user*/
	std::string createDebtID(std::string userID){
		return createID(userID+":unregisteredDebtIDs",userID+":lastDebtID");
	}

	/* Adds a user to redis*/
	std::string createUser(std::string email, std::string name, std::string phone, bool monthlyEmail, bool emailOnUpdate){

		//TODO check if email is already registered in idLookupHash!

		std::string userID = createUserID();//userID for this user.
		std::string hashName = "user:"+userID+":account"; //name of hash that will store this user's account information

		redisCommand(c, (char*) ("hset " + hashName + " name "          + name));
		redisCommand(c, (char*) ("hset " + hashName + " email "         + email));
		redisCommand(c, (char*) ("hset " + hashName + " phone "         + phone));
		redisCommand(c, (char*) ("hset " + hashName + " monthlyEmail "  + ( monthlyEmail ? "yes":"no")));
		redisCommand(c, (char*) ("hset " + hashName + " emailOnUpdate " + (emailOnUpdate ? "yes":"no")));

		redisCommand(c, (char*) ("hset idLookupHash " + email + " " + "userID"));//table for getting userID from email

		return userID;
	}

	/*add a source of income for a given user*/
	std::string addIncome(std::string userID, std::string sourceName, std::string amount, std::string savings){

		std::string incomeID = createIncomeID(userID);
		std::string hashName = "user:" + userID + ":income:" + incomeID;

		redisCommand(c,  (char*)("hset " + hashName + " source "  + sourceName));
		redisCommand(c,  (char*)("hset " + hashName + " amount "  + amount));
		redisCommand(c,  (char*)("hset " + hashName + " savings " + savings));

		return incomeID;
	}

	/*add a source of debt for a given user*/
	std::string addDebt(std::string userID, std::string accountName, std::string balance, std::string apr,
					std::string minimumPayment, std::string extraPayment, std::string dueDate,
					bool paymentScheduled, bool paymentProcessed){

		std::string debtID = createDebtID(userID);
		std::string hashName = "user:" + userID + ":debt:" + debtID;

		redisCommand(c, (char*) ("hset " + hashName + " account "          + accountName));
		redisCommand(c, (char*) ("hset " + hashName + " balance "          + balance));
		redisCommand(c, (char*) ("hset " + hashName + " apr "              + apr));
		redisCommand(c, (char*) ("hset " + hashName + " minimumPayment "   + minimumPayment));
		redisCommand(c, (char*) ("hset " + hashName + " extraPayment "     + extraPayment));
		redisCommand(c, (char*) ("hset " + hashName + " dueDate "          + dueDate));
		redisCommand(c, (char*) ("hset " + hashName + " paymentScheduled " + (paymentScheduled ? "yes":"no")));
		redisCommand(c, (char*) ("hset " + hashName + " paymentProcessed " + (paymentProcessed ? "yes":"no")));

		return debtID;
	}

	//gets a user ID from an email address
	std::string getIDFromEmail(std::string email){
		return redisCommand(c, (char*) ("hget idLookupHash " + email))->str;
	}

}
