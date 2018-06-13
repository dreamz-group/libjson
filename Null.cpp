#include <sstream>

#include "Null.h"
namespace json
{

Null::~Null()
{
}

type_t Null::getType() const
{
    return NULL_TYPE;
}

Value* Null::parse(uint8_t*& b, uint32_t& line)
{
    if( (*b     == 'n' ||     *b == 'N') &&
        (*(b+1) == 'u' || *(b+1) == 'U') &&
        (*(b+2) == 'l' || *(b+2) == 'L') &&
        (*(b+3) == 'l' || *(b+3) == 'L'))
    {
        b += 4;    
        return new Null();
    }
    return NULL;
}

std::string Null::str() const
{
    return std::string("NULL");
}

std::ostream& operator<<(std::ostream& os, const Null* obj)
{
    os << "NULL";
    return os;
}

} // namespace json