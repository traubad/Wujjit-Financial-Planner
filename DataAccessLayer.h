/*
 * DataAccessLayer.h
 *
 *  Created on: Aug 1, 2012
 *      Author: adamtraub
 */

#ifndef DATAACCESSLAYER_H_
#define DATAACCESSLAYER_H_

#include <vector>
#include <map>
namespace DAL{

	namespace Account_Values {
		enum AccountValues{
			name, email, phone, monthlyEmail, emailOnUpdate
		};
	}

	namespace Debt_Values {
		enum DebtValues {
			debt_source, balance, apr, minimum_payment, extra_payment, due_date, payment_scheduled, payment_processed
		};
	}

	namespace Income_Values {
		enum IncomeValues {
			income_source, amount, savings
		};
	}

	void initialize();

	std::string createID(std::string unregisteredList, std::string incrementingID);

	std::string createAccountID();
	std::string createDebtID(std::string accountID);
	std::string createIncomeID(std::string accountID);

	std::string createAccount(std::map<Account_Values::AccountValues, std::string> &vals);
	std::string addDebt(std::string accountID, std::map<Debt_Values::DebtValues, std::string> &vals);
	std::string addIncome(std::string accountID, std::map<Income_Values::IncomeValues, std::string> &vals);

	std::string getIDFromEmail(std::string email);
	//void update(std::string accountID, DAL::Field_Categories::categories category, std::string field, std::string value, std::string subID="");


	void deleteAccount(std::string accountID);
	void deleteIncome(std::string accountID, std::string incomeID);
	void deleteDebt(std::string accountID, std::string debtID);

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
