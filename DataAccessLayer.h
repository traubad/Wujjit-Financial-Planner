/*
 * DataAccessLayer.h
 *
 *  Created on: Aug 1, 2012
 *      Author: adamtraub
 */

#ifndef DATAACCESSLAYER_H_
#define DATAACCESSLAYER_H_

#include "Declarations.h"

using namespace Enums;

namespace DAL{

	void initialize();
    
	std::string createID(std::string unregisteredList, std::string incrementingID);

	std::string createAccountID();
	std::string createDebtID(std::string accountID);
	std::string createIncomeID(std::string accountID);

    std::string createAccount(AcctValueMap& vals);
	std::string addDebt(std::string accountID, DebtValueMap &vals);
	std::string addIncome(std::string accountID, IncomeValueMap &vals);

	std::string getIDFromEmail(std::string email);

	void deleteAccount(std::string accountID);
	void deleteIncome(std::string accountID, std::string incomeID);
	void deleteDebt(std::string accountID, std::string debtID);

	bool updateAccount(std::string accountID, AcctValueMap &vals);
	void updateDebt(std::string accountID, std::string debtID, DebtValueMap &vals);
	void updateIncome(std::string accountID, std::string incomeID, IncomeValueMap &vals);

    AcctValueMap getAccountInfo(std::string accountID);
    DebtValMapVec getDebtInfo(std::string debtID);
    IncomeValMapVec getIncomeInfo(std::string accountID);
	
    std::string hGetAndCorrect(redisContext *C, std::string hash, std::string key);


	namespace Redis_Keys{

		inline std::string idLookupHash(){ return "idLookupHash"; }

		inline std::string lastUserID(){ return "lastAccountID"; }
		inline std::string lastDebtID(std::string accountID){ return "account:"+accountID+":lastDebtID"; }
		inline std::string lastIncomeID(std::string accountID){ return "account:"+accountID+":lastIncomeID"; }

		inline std::string unregisteredUserIDs() { return "unregisteredAccountIDs"; }
		inline std::string unregisteredDebtIDs(std::string accountID)   { return "account:"+accountID+":unregisteredDebtIDs"; }
		inline std::string unregisteredIncomeIDs(std::string accountID) { return "account:"+accountID+":unregisteredIncomeIDs"; }

		inline std::string accountInfo(std::string accountID) { return "account:"+accountID+":info"; }
		inline std::string debtInfo(std::string accountID, std::string debtID) { return "account:" + accountID + ":debt:" + debtID; }
		inline std::string incomeInfo(std::string accountID, std::string incomeID) { return "account:" + accountID + ":income:" + incomeID; }
	}

}
#endif
