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

}// namespace json
