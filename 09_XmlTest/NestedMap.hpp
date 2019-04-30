// Reference: https://bitbucket.org/tunnuz/json/
// Also in GitHub: https://github.com/tunnuz/json

#ifndef __HEARTLAND_NestedMap_hpp__
#define __HEARTLAND_NestedMap_hpp__

#include <iostream>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>

using namespace std;

// Forward declaration
class Value;

typedef map<string, Value> NestedMap;

enum ValueType
{
    STRING,
    NESTEDMAP, // NestedMap
    NIL        // a null
};

class Value
{
public:
    Value() : type_t(NIL) { }
    Value(const char* s) : string_v(s), type_t(STRING) { }
    Value(const string& s) : string_v(s), type_t(STRING) { }
    Value(const NestedMap& n) : nestedMap_v(n), type_t(NESTEDMAP) { }
    Value(ValueType t) : type_t(t) { }

    Value(const Value& v);
    Value& operator=(const Value& v);

    /** Type query. */
    ValueType type() const {return type_t;}

    Value& operator[] (const std::string& key);
    const Value& operator[] (const std::string& key) const;

    pair<NestedMap::iterator, bool> insert(const pair<string, Value>& v);
    NestedMap::iterator begin();
    NestedMap::const_iterator begin() const;
    NestedMap::iterator end();
    NestedMap::const_iterator end() const;
    size_t size(void) const;
    void clear(void);

    bool operator== (const string& rhs);
    bool operator!= (const string& rhs);

    operator std::string () const { return string_v; }
    operator NestedMap () const { return nestedMap_v; }

    std::string as_string() const { return string_v; }


protected:
    std::string         string_v;
    NestedMap           nestedMap_v;

    ValueType           type_t;
};

char *strcpy(char *destination, Value &source);
void *memcpy(void *destination, const Value &source, size_t num);
std::ostream& operator<<(std::ostream&, const Value&);
std::ostream& operator<<(std::ostream&, const NestedMap&);

#endif /* __HEARTLAND_NestedMap_hpp__ */
