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
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "Value.h"
#include "JString.h"
#include "Number.h"
#include "Array.h"
#include "Object.h"
#include "Bool.h"
#include "Null.h"

namespace json
{

Value::Value()
{
}

Value::~Value()
{
}

Value* Value::Clone()
{
    return Clone(this);
}

Value* Value::Clone(const Value* v)
{
    switch( v->getType() )
    {
        case ARRAY:
            return new Array( dynamic_cast<const Array*>(v) );
        case BOOL:
            return new Bool( dynamic_cast<const Bool*>(v) );
        case STRING:
            return new String( dynamic_cast<const String*>(v) );
        case NULL_TYPE:
            return new Null();
        case OBJECT:
            return new Object( dynamic_cast<const Object*>(v) );
        case NUMBER:
            return new Number( dynamic_cast<const Number*>(v) );
        default:
            break;
    }
    std::cerr << "Error" << std::endl;
    return NULL;
}

bool Value::operator==(const Value* obj)
{
    if( obj == NULL )
    {
        return false;
    }
    if( getType() != obj->getType() )
    {
        return false;
    }
    switch( getType() )
    {
        case ARRAY:
        {
            Array* a1 = dynamic_cast<Array*>(this);
            const Array* a2 = dynamic_cast<const Array*>(obj);
            return *a1 == a2;
        }
        case BOOL:
        {
            Bool* b1 = dynamic_cast<Bool*>(this);
            const Bool* b2 = dynamic_cast<const Bool*>(obj);
            return *b1 == b2;
        }
        case STRING:
        {
            String* a1 = dynamic_cast<String*>(this);
            const String* a2 = dynamic_cast<const String*>(obj);
            return *a1 == a2;
        }
        case NULL_TYPE:
            return true;
        case OBJECT:
        {
            Object* a1 = dynamic_cast<Object*>(this);
            const Object* a2 = dynamic_cast<const Object*>(obj);
            return *a1 == a2;
        }
        case NUMBER:
        {
            Number* a1 = dynamic_cast<Number*>(this);
            const Number* a2 = dynamic_cast<const Number*>(obj);
            return *a1 == a2;
        }
        default:
            break;
    }
    return false;
}

bool Value::operator!=(const Value* obj)
{
    return !(*this == obj);
}

Value* Value::parse(uint8_t*& b, size_t& max, uint32_t& line)
{
    Value* rtn = NULL;
    skip(b, max, line);
    if( max == 0 )
    {
        std::cerr << "Out of buffer" << std::endl;
        return NULL;
    }
    if (*b == '"')
    {
        rtn = String::parse(b, max, line);
    }
    else if ((*b >= '0' && *b <= '9') || *b == '-')
    {
        rtn = Number::parse(b, max, line);
    }
    else if (*b == '[')
    {
        rtn = Array::parse(b, max, line);
    }
    else if (*b == '{')
    {
        rtn = Object::parse(b, max, line);
    }
    else if( *b == 'n' || *b == 'N')
    {
        rtn = Null::parse(b, max, line);
    }
    else if( (rtn = Bool::parse(b, max, line)) != NULL )
    {

    }
    else
    {
        std::cerr << "Type mismatch on line: " << line << std::endl;
    }
    return rtn;
}

Value* Value::find(const char* path) const
{
    if( path == NULL )
    {
        return NULL;
    }

    size_t size = strlen(path);
    if( size > 2048 )
    {
        std::cerr << "ERROR Value::find - You are not for real! strlen is too long !" << std::endl;
        return NULL;
    }
    char* tmp = strdup(path);
    char* save = NULL;
    char* part = strtok_r(tmp, "/\0", &save);
    SPATH spath;
    while( part != NULL )
    {
        spath.push_back(part);
        part = strtok_r(NULL, "/\0", &save);
    }
    free( tmp );
    return _find( spath );
}

}// namespace json
