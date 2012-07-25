/*
 * Wujjit.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: adamtraub
 */

#include "Wujjit.h"
#include "tables/IncomeTable.h"
#include <Wt/WPushButton>

Wujjit::Wujjit(const Wt::WEnvironment &env) : Wt::WApplication(env){
	IncomeTable* income_table = new IncomeTable();
	root()->addWidget(income_table);
	root()->addWidget(income_table->submitButton());
}
