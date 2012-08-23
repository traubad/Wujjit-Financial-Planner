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
    /* In whatever loop you use to add EditableCells to the table, you need the following:
     * EditableCell* newCell = new EditableCell(
     *                  //args);
     *
     * // do other stuff
     * newCell->updatedByUser().connect(this, &EditableTable::stageForUpdate);
     *
     * What this does is connect the actual signal in the EdtiableCell class to the stageForUpdate
     * method of this class. The stageForUpdate method of this class is the one you should be watching.
     *
     * There may be an additional issue since we have many signals connecting to one slot. 
     * If you're still having trouble, int eh Wt FAQs page, there is a FAQ explicitly about that; 
     * you just have to change some syntax around and instantiate a... Wt::WSignalMapper I think. 
     * That's a bit more complex, but generally the same thing.
     *
     */
}

void EditableTable::stageForUpdate(EditableCell* updated_cell)
{
    stagedForUpdate.push_back(updated_cell);
}

void EditableTable::updateDatabase()
{
	for (unsigned int i=0; i < stagedForUpdate.size(); i++){
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
