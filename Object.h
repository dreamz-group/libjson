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
#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <vector>

#include "Value.h"

namespace json
{

class Object : public virtual Value
{
public:
    Object();
    Object(const Object* o);

    virtual ~Object();

    virtual type_t getType() const;
    virtual std::string str() const;

    static Value* parse(uint8_t*& b, size_t& max, uint32_t& line);
    // Give a key as index for values.
    Value* operator[](const char* key);
    // Give integer as index to values.
    Value* operator[](unsigned int index);
    Object* operator=(const Object* o);

    // Give integer as index to get a key;
    bool key(unsigned int index, std::string& out);

    void Add(const std::string& name, Value* value);
    void Add(const std::string& name, const char* value);
    void Add(const std::string& name, const std::string& value);
    void Remove(const std::string& name);

protected:
    typedef std::pair<std::string, Value*> VALUE;
    typedef std::vector<VALUE> VALUES;

    VALUES _items;
    bool value_pair(Object* obj, uint8_t*& b, size_t& max, uint32_t& line, bool& more);

    virtual Value* _find( SPATH& spath) const;
private:
    friend std::ostream& operator<<(std::ostream& os, const Object* obj);
};
} // namespace json
#endif // __OBJECT_H__
