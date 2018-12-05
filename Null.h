#ifndef __NULL_H__
#define __NULL_H__

#include "Value.h"

namespace json
{

class Null : public virtual Value
{
public:
    virtual ~Null();
    virtual type_t getType() const;
    virtual std::string str() const;
    static Value* parse(uint8_t*& b, size_t& max, uint32_t& line);
    friend std::ostream& operator<<(std::ostream& os, const Null* obj);
    virtual Value* _find( SPATH& spath) const;
};

}
#endif //__NULL_H__