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

}
#endif
