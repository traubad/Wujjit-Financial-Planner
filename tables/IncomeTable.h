/*
 * IncomeTable.h
 *
 *  Created on: Jul 18, 2012
 *      Author: adamtraub
 */

#ifndef INCOMETABLE_H_
#define INCOMETABLE_H_

	#include "EditableTable.h"

	class IncomeTable : public EditableTable {
		protected:
			void getData();
			public:
				IncomeTable(Wt::WContainerWidget *parent=0);
};

#endif /* INCOMETABLE_H_ */
