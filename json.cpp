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

#include "json.h"

namespace json
{

Parse::Parse()
{
}

Value* Parse::read(uint8_t* b, size_t max)
{
    uint32_t line = 1;
    if( b == NULL || max == 0)
    {
        std::cerr << "Error can not convert NULL to json!" << std::endl;
        return NULL;
    }
    Value::skip(b, max, line);
    if( *b == '\0' || max == 0 )
    {
        std::cerr << "No data in file to read!" << std::endl;
        return NULL;
    }
    if( *b == '[' )
    {
        return Array::parse(b, max, line);
    }
    return Object::parse(b, max, line);
}

std::ostream& operator<<(std::ostream& os, const json::Value* obj)
{
    if ( obj == NULL )
    {
        os << "null";
        return os;
    }
    switch (obj->getType())
    {
    case json::OBJECT:
        os << dynamic_cast<const json::Object*>(obj);
        break;
    case json::ARRAY:
        os << dynamic_cast<const json::Array*>(obj);
        break;
    case json::NUMBER:
        os << dynamic_cast<const json::Number*>(obj);
        break;
    case json::STRING:
        os << dynamic_cast<const json::String*>(obj);
        break;
    case json::BOOL:
        os << dynamic_cast<const json::Bool*>(obj);
        break;
    case json::NULL_TYPE:
        os << dynamic_cast<const json::Null*>(obj);
        break;
    default:
        os << "Invalid object " << obj->getType() << std::endl;
        break;
    }
    return os;
}

} // namespace json

