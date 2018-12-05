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
#include <sstream>
#include <stdio.h>

#include "Array.h"
#include "JString.h"

namespace json
{


Array::Array()
{
}

Array::~Array()
{
    json::Array::VALUES::iterator itr = _items.begin();
    for (; itr != _items.end(); ++itr)
    {
        delete *itr;
    }
}

type_t Array::getType() const
{
    return ARRAY;
}

Value* Array::parse(uint8_t*& b, size_t& max, uint32_t& line)
{
    skip(b, max, line);
    if (max == 0 || *b != '[')
    {
        std::cerr << "Parse error missing [ at line:" << line << std::endl;
        return NULL;
    }
    ++b; --max;
    skip(b, max, line);
    if( max == 0 )
    {
        std::cerr << "Out of buffer" << std::endl;
        return NULL;
    }

    Array* arr = new Array();
    if (*b == ']')
    {
        ++b; --max;
        return arr;
    }

    bool more = true;
    do
    {
        Value* rtn = Value::parse(b, max, line);
        if (rtn == NULL)
        {
            delete arr;
            return NULL;
        }
        arr->_items.push_back(rtn);
        skip(b, max, line);
        if( max == 0 )
        {
            std::cerr << "Out of buffer" << std::endl;
            return NULL;
        }

        if (*b != ',')
        {
            more = false;
        }
        else
        {
            ++b; --max;
        }
    } while (more);

    if (max == 0 || *b != ']')
    {
        std::cerr << "Parse error missing ] at line:" << line << std::endl;
        delete arr;
        return NULL;
    }
    ++b; --max;
    return arr;
}

std::string Array::str() const
{
    std::stringstream sstr;
    sstr << "[" ;
    json::Array::VALUES::const_iterator itr = _items.begin();
    while (itr != _items.end())
    {
        sstr << (*itr)->str();
        ++itr;
        if (itr != _items.end())
        {
            sstr << ",";
        }
    }
    sstr << "]";
    return sstr.str();
}

Value* Array::operator[](unsigned int index)
{
    if( _items.size() > index )
    {
        return _items[index];
    }
    return NULL;
}

void Array::Add(const std::string& value)
{
    _items.push_back(new json::String(value.c_str()));
}

void Array::Add(const char* value)
{
    _items.push_back(new json::String(value));
}

void Array::Add(Value* value)
{
    _items.push_back(value);
}

size_t Array::length()
{
    return _items.size();
}

Value* Array::_find( SPATH& spath) const 
{
    // Support [index] and [*] to match first found.
    std::string t = spath[0];
    if( t[0] != '[' || t[ t.length() - 1 ] != ']' )
    {
        return NULL;
    }
    spath.erase( spath.begin() );
    if( t.length() == 3 && t[1] == '*')
    {
        json::Array::VALUES::const_iterator itr = _items.begin();
        for(; itr != _items.end(); ++itr )
        {
            Value* v = (*itr)->_find(spath);
            if( v != NULL )
            {
                return v;
            }
        }
        return NULL;
    }
    t = t.substr(1, t.length() - 1 ); // remove [ and ]
    int i = -1;
    if( sscanf(t.c_str(), "%d", &i) != 1 || i <= -1)
    {
        return NULL;
    }

    if( (unsigned int)i >= _items.size( ))
    {
        return NULL;
    }
    if( spath.size() == 0 )
    {
        return _items[i];
    }
    return _items[i]->_find(spath);
}

std::ostream& operator<<(std::ostream& os, const Array* arr)
{
    os << "[ ";
    json::Array::VALUES::const_iterator itr = arr->_items.begin();
    while (itr != arr->_items.end())
    {
        os << *itr;
        ++itr;
        if (itr != arr->_items.end())
        {
            os << ", ";
        }
    }

    os << " ]";
    return os;
}
}// namespace json
