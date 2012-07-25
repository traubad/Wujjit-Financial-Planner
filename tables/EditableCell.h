/*
 * EditableCell.h
 *
 *  Created on: Jul 23, 2012
 *      Author: adamtraub
 */

#ifndef EDITABLECELL_H_
#define EDITABLECELL_H_
#include <Wt/WStackedWidget>
#include <Wt/WSignal>

	class EditableCell : public Wt::WStackedWidget
	{
		private:
			void get_input();
			void cancel_update();
			Wt::Signal <EditableCell*> _stage_for_update;
		public:
			EditableCell(std::string original_value, Wt::WContainerWidget* parent =0);
			Wt::Signal <EditableCell*>&  updatedByUser();
			void display_input();
	};

#endif /* EDITABLECELL_H_ */
