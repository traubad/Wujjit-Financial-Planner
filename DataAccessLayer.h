/*
 * DataAccessLayer.h
 *
 *  Created on: Aug 1, 2012
 *      Author: adamtraub
 */

#ifndef DATAACCESSLAYER_H_
#define DATAACCESSLAYER_H_


namespace DAL{

	std::string createID(std::string unregisteredList, std::string incrementingID);
	std::string createUserID();
	std::string createIncomeID(std::string userID);
	std::string createDebtID(std::string userID);
	std::string createUser(std::string email, std::string name, std::string phone="", bool monthlyEmail=true, bool emailOnUpdate=true);
	std::string addIncome(std::string userID, std::string sourceName, std::string amount, std::string savings);

	std::string addDebt(std::string userID, std::string accountName, std::string balance, std::string apr,
					std::string minimumPayment, std::string extraPayment, std::string dueDate = "1",
					bool paymentScheduled = false, bool paymentProcessed = false);

	std::string getIDFromEmail(std::string email);

	void deleteUser(std::string userID);
	void deleteIncome(std::string userID, std::string incomeID);
	void deleteDebt(std::string userID, std::string debtID);

	namespace redisKeys{
		inline std::string idLookupHash(){ return "idLookupHash"; }

		inline std::string lastUserID(){ return "lastUserID"; }
		inline std::string lastDebtID(std::string userID){ return "user:"+userID+":lastDebtID"; }
		inline std::string lastIncomeID(std::string userID){ return "user:"+userID+":lastIncomeID"; }

		inline std::string unregisteredUserIDs() { return "unregisteredUserIDs"; }
		inline std::string unregisteredDebtIDs(std::string userID)   { return "user:"+userID+":unregisteredDebtIDs"; }
		inline std::string unregisteredIncomeIDs(std::string userID) { return "user:"+userID+":unregisteredIncomeIDs"; }

		inline std::string accountInfo(std::string userID) { return "user:"+userID+":account"; }
		inline std::string debtInfo(std::string userID, std::string debtID) { return "user:" + userID + ":debt:" + debtID; }
		inline std::string incomeInfo(std::string userID, std::string incomeID) { return "user:" + userID + ":income:" + incomeID; }

	}

}
#endif
