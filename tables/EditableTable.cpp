/*
 * EditableTable.cpp
 *
 *  Created on: Jul 23, 2012
 *      Author: adamtraub
 */

#include <Wt/WPushButton>
#include <Wt/WContainerWidget>
#include "EditableTable.h"

EditableTable::EditableTable(Wt::WContainerWidget* parent)
: Wt::WTable(parent), stagedForUpdate()
{
    submit_button = new Wt::WPushButton("Update Information");
    submit_button->clicked().connect(this, &EditableTable::updateDatabase);
}

void EditableTable::stageForUpdate(EditableCell* updated_cell)
{
    stagedForUpdate.push_back(updated_cell);
}

void EditableTable::updateDatabase()
{
	for (int i=0; i < stagedForUpdate.size(); i++){
		//update the database with their new value.
		stagedForUpdate[i]->display_input();
	}

	std::cout<<"length of stagedForUpdate: "<<stagedForUpdate.size()<<std::endl;

    //erase all entries from vector (but not the pointers -- i think this is done with .erase method)
	//stagedForUpdate.erase(stagedForUpdate.begin(),stagedForUpdate.end());

}
Wt::WPushButton* EditableTable::submitButton() const
{
	return submit_button;
}
