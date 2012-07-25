/*
 * EditableTable.h
 *
 *  Created on: Jul 23, 2012
 *      Author: adamtraub
 */

#ifndef EDITABLETABLE_H_
#define EDITABLETABLE_H_
	#include "EditableCell.h"
	#include <Wt/WTable>

	class EditableTable : public Wt::WTable{
		private:
			Wt::WPushButton* submit_button;
			std::vector <EditableCell*> stagedForUpdate;
			void stageForUpdate(EditableCell* update_cell);
			void updateDatabase();
		public:
			EditableTable(Wt::WContainerWidget* parent =0);
			Wt::WPushButton* submitButton() const;
	};

#endif /* EDITABLETABLE_H_ */
