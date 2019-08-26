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

#ifndef __JSON_H__
#define __JSON_H__

#include "Value.h"
#include "Number.h"
#include "JString.h" 
#include "Bool.h"
#include "Object.h"
#include "Array.h"
#include "Null.h"

namespace json
{

class Parse
{
public:
    Parse();

    void parse_number(uint8_t*& b, uint32_t& line);
    void parse_array(uint8_t*& b, uint32_t& line);
    void parse_object(uint8_t*& b, uint32_t& line);
    Value* read(uint8_t* b, size_t max);
    static inline json::Object* Object(uint8_t* b, size_t max)
    {
        Parse p;
        json::Value* v = p.read(b, max);
        if( v == NULL )
        {
            return NULL;
        }
        return dynamic_cast<json::Object*>(v);
    }
    static inline json::Array* Array(uint8_t* b, size_t max)
    {
        Parse p;
        json::Value* v = p.read(b, max);
        if( v == NULL )
        {
	    return NULL;
        }
        return dynamic_cast<json::Array*>(v);
    }                              
};
} //namespace json

#endif // __JSON_H__
