/*
    This file is part of libjson.

    MIT License
    Copyright (c) 2018 dreamz-group

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#include <assert.h>
#include <math.h>
#include <sstream>
#include <limits.h>
#include <string.h>

#include "Number.h"

namespace json
{


Number::Number(uint64_t v)
{
    _type = UINT64;
    _value.ui = v;
}

Number::Number(int64_t v)
{
    _type = INT64;
    _value.i = v;
}

Number::Number(uint32_t v)
{
    _type = UINT64;
    _value.ui = v;
}

Number::Number(int32_t v)
{
    _type = INT64;
    _value.i = v;
}

Number::Number(double v)
{
    _type = DOUBLE;
    _value.d = v;
}

Number::Number(const Number* v)
{
    *this = v;
}

Number* Number::operator=(const Number* v)
{
    _type = v->_type;
    memcpy(&_value, &v->_value, sizeof(_Number_Value));
    return this;
}

bool Number::operator==(const Number* v) const
{
    return memcmp(&_value, &v->_value, sizeof(_Number_Value)) == 0;
}

Number::~Number()
{
}

uint64_t Number::value()
{
    assert( _type == UINT64 );
    return _value.ui;
}

type_t Number::getType() const
{
    return NUMBER;
}

Number::operator int() const
{
    if( _type == INT64 && _value.i < INT_MAX )
    {
        return (int)_value.i;
    }
    if( _type == UINT64 && _value.ui < INT_MAX )
    {
        return (int)_value.ui;
    }
    std::cerr << "Can not be casted to int!" << std::endl;
    return INT_MAX;
}


Number::operator double() const
{
    if( _type == Number::DOUBLE )
    {
        return _value.d;
    }
    int t = *this;
    if( t != INT_MAX)
    {
        return (double)t;
    }
    return NAN;
}

Value* Number::parse(uint8_t*& b, size_t& max, uint32_t& line)
{
    uint64_t value = 0;
    bool     dec = false;
    bool     neg = false;
    double   dbl = 0;
    if( max == 0 )
    {
        std::cerr << "Out of buffer" << std::endl;
        return NULL;
    }
    if (*b == '-')
    {
        neg = true;
        ++b; --max;
    }

    if (!digit(b, max, value))
    {
        std::cerr << "Not a number at line:" << line << std::endl;
        return NULL;
    }

    if( max == 0 )
    {
        std::cerr << "Out of buffer" << std::endl;
        return NULL;
    }

    if (*b == '.')
    {
        ++b; --max;
        dbl = (double)value;
        uint8_t* t = b;
        dec = true;
        value = 0;
        if ( max == 0 || !digit(b, max, value))
        {
            std::cerr << "Must be atleast one digit after '.' at line:" << line << std::endl;
            return NULL;
        }
        if( max == 0 )
        {
            std::cerr << "Out of buffer" << std::endl;
            return NULL;
        }
        dbl += (double)value / pow(10, b - t);
    }
    if (*b == 'e' || *b == 'E')
    {
        std::cerr << "Number in format x.yEn not implemented at line:" << line << std::endl;
        return NULL;
    }
    if (dec)
    {
        return new Number(neg ? -dbl : dbl);
    }
    return neg ? new Number(-(int64_t)value) : new Number(value);
}

std::string Number::str() const
{
    std::stringstream sstr;
    sstr << this;
    return sstr.str();
}

std::ostream& operator<<(std::ostream& os, const Number* obj)
{
    switch (obj->_type)
    {
    case Number::UINT64:
        os << obj->_value.ui;
        break;
    case Number::INT64:
        os << obj->_value.i;
        break;
    case Number::DOUBLE:
        os << obj->_value.d;
        break;
    default:
        os << "Invalid number type:" << obj->_type << std::endl;
        break;
    }
    return os;
}

Value* Number::_find( SPATH& spath) const
{
    return NULL;
}

}// namespace json
