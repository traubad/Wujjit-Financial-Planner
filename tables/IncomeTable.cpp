/*
 * IncomeTable.cpp
 *
 *  Created on: Jul 18, 2012
 *      Author: adamtraub
 */

#include <Wt/WApplication>
#include <Wt/WTable>
#include <Wt/WText>
#include <sstream>
#include "../DataAccessLayer.h"
#include "EditableCell.h"
#include "IncomeTable.h"
#include "../Declarations.h"

using namespace Enums::Income_Values;
using std::stringstream;

//class IncomeTable
IncomeTable::IncomeTable(Wt::WContainerWidget *parent)
: EditableTable(parent)
{
	stringstream strm;
	std::string id = "1";
	std::cout<<"SOMETHING WILL BREAK"<<std::endl;
	std::vector<std::map<Income_Values::IncomeValues, std::string> > incomeData = DAL::getIncomeInfo(id);
	std::cout<<"SOMETHING BROKE"<<std::endl;

	int columns = 3;
	int rows = 3;

	setHeaderCount(1);
	elementAt(0,income_source)->addWidget(new Wt::WText("Income Source"));
	elementAt(0,amount)->addWidget(new Wt::WText("Amount"));
	elementAt(0,savings)->addWidget(new Wt::WText("Savings"));

	/*
	for (int row = 1; row < incomeData.size()+1; row++){
		elementAt(row,amount)->addWidget(new EditableCell("test"));
	}


	//populate table
	for (int row = 1; row < rows+1; row++) {
		for (int column = 0; column < columns; column++) {
			strm.str("");
			strm << "Item: "<< row <<","<<column;
			elementAt(row,column)->addWidget(new EditableCell(strm.str()));
		}
	}
	*/
}

