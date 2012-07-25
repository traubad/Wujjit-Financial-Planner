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
#include "EditableCell.h"
#include "IncomeTable.h"

using std::stringstream;

//class IncomeTable
IncomeTable::IncomeTable(Wt::WContainerWidget *parent)
: EditableTable(parent)
{
	int columns = 4;
	int rows = 5;
	stringstream strm;
	setHeaderCount(1);

	//populate header
	for (int c=1;c<columns+1;c++){
		strm.str("");
		strm << "Column: " << c;
		elementAt(0,c)->addWidget(new Wt::WText(strm.str()));
	}

	//populate table
	for (int row = 1; row < rows+1; row++) {
		for (int column = 1; column < columns+1; ++column) {
			strm.str("");
			strm << "Item: "<< row <<","<<column;
			elementAt(row,column)->addWidget(new EditableCell(strm.str()));
		}
	}
}
void IncomeTable::getData()
{
}

