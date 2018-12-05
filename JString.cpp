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
#include <string.h>
#include "JString.h"

namespace json
{


String::String()
{
}

String::String(const std::string& value)
{
    _value = value;
}

String::String(const char* value)
{
    _value = "";
    size_t len = strlen(value);
    uint8_t* t = (uint8_t*) value;
    Format(t, len, _value);
}

String::~String()
{
}

type_t String::getType() const
{
    return STRING;
}

Value* String::parse(uint8_t*& b, size_t& max, uint32_t& line)
{
    String* rtn = new String();
    if (parse_string(rtn->_value, b, max, line))
    {
        return rtn;
    }
    delete rtn;
    return NULL;
}

void String::Format(uint8_t*& b, size_t& max, std::string& id)
{
    while ( max &&
            ( (*b == ' ')  ||
              (*b == '!')  ||
              (*b == '\n') ||
              (*b == '\t') ||
              (*b == '\r') ||           
              (*b >= '#' && *b <= '~') ||            
              ( (*b & 0xC0 ) == 0xC0 ) // multichar 2 - byte
            )
           ) 
    {
        if( (*b & 0xC0) == 0xC0 )
        {
            id += *b; ++b; --max;
        }
        if( *b == '\\')
        {            
            id += *(char*)b; ++b; --max;
        }
        id += *(char*)b;
        ++b; --max;
    }
}

bool String::parse_string(std::string& id, uint8_t*& b, size_t& max, uint32_t& line)
{
    if (*b != '\"' || max == 0)
    {
        std::cerr << "Parse error missing start \" of id at line:" << line << std::endl;
        return false;
    }
    ++b; --max;
    id = "";
    Format(b, max, id);
    if (*b != '\"' || max == 0)
    {
        std::cerr << "Parse error missing end \" of id at line:" << line << " on char '" << *(char*)b << "'" << std::endl;
        return false;
    }
    ++b; --max;
    return true;
}

String& String::operator=(char* str)
{
    _value = "";
    size_t len = strlen(str);
    uint8_t* t = (uint8_t*)str;
    Format(t, len, _value);    
    return *this;
}
String& String::operator=(std::string& str)
{
    _value = "";
    size_t len = str.length();
    uint8_t* t = (uint8_t*)str.c_str();
    Format(t, len, _value);    
    return *this;
}

std::ostream& operator<<(std::ostream& os, const String* obj)
{
    os << "\"" << obj->_value << "\"";
    return os;
}

Value* String::_find( SPATH& spath) const
{
    return NULL;
}

}// namespace json
