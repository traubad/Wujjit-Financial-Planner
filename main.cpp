
#include "Wujjit.h"
#include <hiredis/hiredis.h>
#include <iostream>
#include "DataAccessLayer.h"
#include "Declarations.h"

using namespace Enums::Income_Values;

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
	return new Wujjit(env);
}


int main(int argc, char **argv)
{
	std::cout<<"BEGIN"<<std::endl;
/*
	Wt::ApplicationCreator app(&createApplication);
	return Wt::WRun(argc, argv, app);

	redisReply *reply;
	redisContext *c = redisConnect("localhost",6379);

	reply = redisCommand(c,"get foo");
	cout << reply->str << endl;
	freeReplyObject(reply);

	printf("\n\n");

	//DAL::addUser("HelloWorld@HW.net","Hello World");

	reply = redisCommand(c,"keys *");
	for(int i=0; i < reply->elements; i++){
		cout << reply->element[i]->str << endl;
	}

	freeReplyObject(reply);

	printf("\n\n");

	reply = redisCommand(c,"SMEMBERS users");
	for(int i=0; i < reply->elements; i++){
		cout << reply->element[i]->str << endl;
	}

	freeReplyObject(reply);

	redisFree(c);


	DAL::initialize();

	std::map<DebtValues, std::string> debtArgs;
	debtArgs[debt_source] = "small debt";
	debtArgs[balance] = "100";
	debtArgs[apr] = "0.1";
	debtArgs[minimum_payment] = "10";
	debtArgs[extra_payment] = "0";
	debtArgs[due_date] = "1";
	debtArgs[payment_scheduled] = "no";
	debtArgs[payment_processed] = "no";

	std::map<IncomeValues, std::string> incomeArgs;
	incomeArgs[income_source] = "criminal wrong doings";
	incomeArgs[amount] = "200";

	std::map<AccountValues, std::string> accountArgs;
	accountArgs[name]="Charlie";
	accountArgs[phone] = "555 555 5555";
	accountArgs[monthlyEmail] = "yes";
	accountArgs[emailOnUpdate] = "yes";
	accountArgs[email]="test2@test.test";

//	std::string id = DAL::createAccount(accountArgs);

//	std::string debtID = DAL::addDebt(id, debtArgs);
//	std::string incomeID = DAL::addIncome(id, incomeArgs);

	accountArgs[name]="Hillary";
	accountArgs[phone] = "555 555 5555";
	accountArgs[monthlyEmail] = "yes";
	accountArgs[emailOnUpdate] = "yes";
	accountArgs[email]="test33@test.test";

	incomeArgs[income_source] = "engineering";
	incomeArgs[savings] = "50";
	incomeArgs[amount] = "500";

	debtArgs[debt_source] = "scary debt";
	debtArgs[balance] = "10000";
	debtArgs[apr] = "0.8";
	debtArgs[minimum_payment] = "100";
	debtArgs[extra_payment] = "40";
	debtArgs[due_date] = "15";
	debtArgs[payment_scheduled] = "no";
	debtArgs[payment_processed] = "no";

//	DAL::createAccount(accountArgs);
//	DAL::addDebt(id, debtArgs);
//	DAL::addIncome(id, incomeArgs);



//	DAL::deleteAccount(id);
//	DAL::deleteIncome(id, incomeID);
//	DAL::deleteDebt(id, debtID);

	//std::map<AccountValues, std::string> newAccountArgs;
	//newAccountArgs[email]="test4@test.test";
//	newAccountArgs[phone] = "444 444 4444";
	//DAL::updateAccount("2",newAccountArgs);

//	std::map<DebtValues, std::string> newDebtArgs;
//	newDebtArgs[due_date]="5";
//	DAL::updateDebt(id,debtID,newDebtArgs);

//	std::map<IncomeValues, std::string> newIncomeArgs;
//	newIncomeArgs[income_source]="That's rather complicated";
//	newIncomeArgs[amount]="7500";
//	newIncomeArgs[savings]="5";
//	DAL::updateIncome(id,debtID,newIncomeArgs);

//	DAL::updateIncome(id,incomeID,income_source,"stealing things");
//	DAL::updateDebt(id,debtID,debt_source,"bad debt");

	std::string id = "1";

	std::map<AccountValues, std::string> accountInfo = DAL::getAccountInfo(id);
	for ( std::map<AccountValues, std::string>::iterator it = accountInfo.begin(); it != accountInfo.end(); it++){
		std::cout<<accountInfo[it->first]<<std::endl;
	}
	std::cout<<"\n";

	std::vector<std::map<DebtValues, std::string> > debtData = DAL::getDebtInfo(id);

	for(unsigned int i=0; i< debtData.size(); i++){
		for ( std::map<DebtValues, std::string>::iterator it = debtData.at(i).begin(); it != debtData.at(i).end(); it++){
			std::cout<<debtData.at(i)[it->first]<<std::endl;
		}
		std::cout<<"\n";
	}
*/

	std::cout<<"Made it past the comments!"<<std::endl;

	std::string id = "1";
	std::vector<std::map<IncomeValues, std::string> > incomeData = DAL::getIncomeInfo(id);

	std::cout<<"built the vector of maps!"<<std::endl;

	int DEBUG = 0;

	for(unsigned int i=0; i< incomeData.size(); i++){

		for ( std::map<IncomeValues, std::string>::iterator it = incomeData.at(i).begin(); it != incomeData.at(i).end(); it++){
			std::cout<<incomeData.at(i)[it->first]<<std::endl;
		}
		std::cout<<"\n";
	}

	return 0;
}
