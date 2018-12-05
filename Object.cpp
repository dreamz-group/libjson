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

#include "Object.h"
#include "JString.h"

namespace json
{

Object::Object()
{
}

Object::~Object()
{
    json::Object::VALUES::iterator itr = _items.begin();
    for (; itr != _items.end(); ++itr)
    {
        delete itr->second;
    }
}

type_t Object::getType() const
{
    return OBJECT;
}

void Object::Add(const std::string& name, const std::string& value)
{
    _items.push_back(VALUE(name, new json::String(value.c_str())));
}

void Object::Add(const std::string& name, const char* value)
{
    _items.push_back(VALUE(name, new json::String(value)));
}

void Object::Add(const std::string& name, Value* value)
{
    _items.push_back(VALUE(name, value));
}

bool Object::value_pair(Object* obj, uint8_t*& b, size_t& max, uint32_t& line, bool& more)
{
    // Parse id
    skip(b, max, line);
    std::string id;
    if (!String::parse_string(id, b, max, line))
    {
        return false;
    }
    skip(b, max, line);
    if (*b != ':' || max == 0)
    {
        std::cerr << "Parse error missing : at line:" << line << std::endl;
        return false;
    }
    ++b; --max;

    Value* rtn = Value::parse(b, max, line);
    // TODO bool, null
    if (rtn == NULL)
    {
        return false;
    }
    obj->_items.push_back(VALUE(id, rtn));
    skip(b, max, line);
    if (*b != ',' || max == 0)
    {
        more = false;
    }
    else
    {
        ++b; --max;
    }
    return true;
}

Value* Object::parse(uint8_t*& b, size_t& max,  uint32_t& line)
{
    skip(b, max, line);
    if (max == 0 || *b != '{')
    {
        std::cerr << "Parse error missing { at line:" << line << std::endl;
        return NULL;
    }
    ++b; --max;
    skip(b, max, line);
    if( max == 0 )
    {
        std::cerr << "No end of object found, before end of buffer" << std::endl;
        return NULL;
    }

    Object* obj = new Object();
    if (*b == '}')
    {
        ++b; --max;
        return obj;
    }
    bool more = true;
    while (more)
    {
        if (!obj->value_pair(obj, b, max, line, more))
        {
            delete obj;
            return NULL;
        }
    }
    if (*b != '}' || max==0 )
    {
        std::cerr << "Parse error missing } at line:" << line << std::endl;
        delete obj;
        return NULL;
    }
    ++b; --max;
    return obj;
}

std::string Object::str() const
{
    std::stringstream os;
    os << "{" ;
    json::Object::VALUES::const_iterator itr = this->_items.begin();
    while (itr != this->_items.end())
    {
        os << "\"" << itr->first << "\" : ";
        if( itr->second->getType() == json::STRING)
        {
            os << "\"" << itr->second->str() << "\"";
        }
        else
        {
            os << itr->second->str();
        }
        ++itr;
        if (itr != this->_items.end())
        {
            os << "," ;
        }
    }
    os << "}";
    return os.str();
}

Value* Object::_find( SPATH& spath) const
{
    if( spath.size() == 0 )
    {
        return NULL;
    }

    json::Object::VALUES::const_iterator itr = _items.begin();
    for(; itr != _items.end(); ++itr )
    {
        if( itr->first != spath[0] )
        {
            continue;
        }
        if( spath.size() == 1 )
        {
            return itr->second;
        }
        spath.erase( spath.begin() );
        return itr->second->_find(spath);
    }
    return NULL;
}

std::ostream& operator<<(std::ostream& os, const Object* obj)
{
    os << "{ ";
    json::Object::VALUES::const_iterator itr = obj->_items.begin();
    while (itr != obj->_items.end())
    {
        os << "\"" << itr->first << "\" : " << itr->second;
        ++itr;
        if (itr != obj->_items.end())
        {
            os << ", ";
        }
    }

    os << " }";
    return os;
}

Value* Object::operator[](const char* key)
{
    json::Object::VALUES::iterator itr = _items.begin();
    for (; itr != _items.end(); ++itr)
    {
        if (itr->first == key)
        {
            return itr->second;
        }
    }
    return NULL;
}

bool Object::key(unsigned int index, std::string& out)
{
    if( _items.size() > index )
    {
        VALUE& v = _items[index];
        out = v.first;
        return true;
    }
    return false;
}

Value* Object::operator[](unsigned int index)
{
    if( _items.size() > index )
    {
        VALUE v = _items[index];
        return v.second;
    }
    return NULL;
}

}// namespace json
