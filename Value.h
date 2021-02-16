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
#ifndef __VALUE_H__
#define __VALUE_H__

#include <stdint.h>
#include <iostream>
#include <vector>

namespace json
{

typedef enum
{
    NDEF,
    OBJECT,
    ARRAY,
    STRING,
    NUMBER,
    BOOL,
    NULL_TYPE
} type_t;

class Value
{
public:
    Value();
    virtual ~Value();
    virtual type_t getType() const = 0;
    friend std::ostream& operator<<(std::ostream& os, const Value* obj);
    static inline void skip(uint8_t*& b, size_t& max, uint32_t& line)
    {
        while ((*b == ' ' || *b == '\t' || *b == '\n' || *b == '\r') && max != 0 )
        {
            if (*b == '\n')
            {
                ++line;
            }
            ++b; --max;
        }
    }
    static Value* parse(uint8_t*& b, size_t& max, uint32_t& line);
    virtual std::string str() const = 0;
    Value* find(const char* path) const;

    typedef std::vector<std::string> SPATH;
    virtual Value* _find( SPATH& spath) const = 0;
    bool operator==(const Value* obj) const;
    bool operator!=(const Value* obj) const;
    Value* Clone() const;
protected:
    static Value* Clone(const Value* v);
};
} // namespace json
#endif // __NUMBER_H__
