#ifndef ENUMS_H_
#define ENUMS_H_

namespace Enums{

	namespace Account_Values {
		enum AccountValues{
			name, email, phone, monthlyEmail, emailOnUpdate
		};
	}

	namespace Debt_Values {
		enum DebtValues {
			debt_source, balance, apr, minimum_payment, extra_payment, due_date, payment_scheduled, payment_processed
		};
	}

	namespace Income_Values {
		enum IncomeValues {
			income_source, amount, savings
		};
	}
}

#endif
