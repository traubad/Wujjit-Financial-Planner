#ifndef TL_EXCEPTIONS_UNPARSEABLE_ENUM_H_
#define TL_EXCEPTIONS_UNPARSEABLE_ENUM_H_

#include "../TurnLeft.h"
#include <exception>

namespace TurnLeft{
namespace Exceptions{

class UnparseableEnumException : public std::exception
{
private:
    const char* errMsg;

public:
    UnparseableEnumException (const std::string& value);
    inline const char* what() const throw()
    {
        return this->errMsg;
    }
};


}//namespace Exceptions
}//namespace TurnLeft
#endif
