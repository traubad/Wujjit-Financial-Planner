/*
 * Declarations.h
 *
 *  Created on: Aug 22, 2012
 *      Author: adamtraub
 */

#ifndef DECLARATIONS_H_
#define DECLARATIONS_H_

#include <map>
#include <string>
#include <vector>
#include "Enums.h"

class redisContext;
class redisReply;

namespace Wt
{
    class WApplication;
    class WEnvironment;
    class WContainerWidget;
    class WStackedWidget;
    class WString;
    class WText;
    class WAnchor;
}

typedef std::map <Account_Values::AccountValues, std::string> AcctValueMap;

typedef std::map <Debt_Values::DebtValues, std::string> DebtValueMap;
typedef std::map <Income_Values::IncomeValues, std::string> IncomeValueMap;
typedef std::vector <AcctValueMap> AcctValMapVec;
typedef std::vector <DebtValueMap> DebtValMapVec;
typedef std::vector <IncomeValueMap> IncomeValMapVec;
#endif /* DECLARATIONS_H_ */
