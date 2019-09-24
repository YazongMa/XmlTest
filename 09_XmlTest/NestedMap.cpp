#include "NestedMap.hpp"

Value::Value(const Value &v)
{
    switch (v.type())
    {
    case STRING:
        string_v = v.string_v;
        type_t = STRING;
        break;

    case NESTEDMAP:
        nestedMap_v = v.nestedMap_v;
        type_t = NESTEDMAP;
        break;

    case NIL:
        type_t = NIL;
        break;
    }
}

Value &Value::operator=(const Value &v)
{
    switch (v.type())
    {
    case STRING:
        string_v = v.string_v;
        type_t = STRING;
        break;

    case NESTEDMAP:
        nestedMap_v = v.nestedMap_v;
        type_t = NESTEDMAP;
        break;

    case NIL:
        type_t = NIL;
        break;
    }

    return *this;
}

Value &Value::operator[](const string &key)
{
    if (type_t != NESTEDMAP && type() != NIL)
    {
        throw std::logic_error("Error occurred when using operator[]: not a NesetedMap");
    }

    if (type() == NIL)
    {
        type_t = NESTEDMAP;
    }

    return nestedMap_v[key];
}

const Value &Value::operator[](const string &key) const
{
    if (type_t != NESTEDMAP && type() != NIL)
    {
        throw std::logic_error("Error occurred when using operator[]: not a NesetedMap");
    }

    return nestedMap_v.at(key);
}

pair<NestedMap::iterator, bool> Value::insert(const pair<string, Value> &v)
{
    if (type_t != NESTEDMAP && type() != NIL)
    {
        throw std::logic_error("Error occurred when using insert(): not a NesetedMap");
    }

    return nestedMap_v.insert(v);
}

NestedMap::iterator Value::begin()
{
    if (type_t != NESTEDMAP && type() != NIL)
    {
        throw std::logic_error("Error occurred when using insert(): not a NesetedMap");
    }

    return nestedMap_v.begin();
}

NestedMap::const_iterator Value::begin() const
{
    if (type_t != NESTEDMAP && type() != NIL)
    {
        throw std::logic_error("Error occurred when using insert(): not a NesetedMap");
    }

    return nestedMap_v.begin();
}

NestedMap::iterator Value::end()
{
    if (type_t != NESTEDMAP && type() != NIL)
    {
        throw std::logic_error("Error occurred when using insert(): not a NesetedMap");
    }

    return nestedMap_v.end();
}

NestedMap::const_iterator Value::end() const
{
    if (type_t != NESTEDMAP && type() != NIL)
    {
        throw std::logic_error("Error occurred when using insert(): not a NesetedMap");
    }

    return nestedMap_v.end();
}

size_t Value::size(void) const
{
    if (type_t == STRING)
    {
        return string_v.size();
    }
    else if (type_t == NESTEDMAP || type() == NIL)
    {
        return nestedMap_v.size();
    }

    return 0;
}

void Value::clear(void)
{
    nestedMap_v.clear();
}

bool Value::operator==(const string &rhs)
{
    return string_v == rhs;
}

bool Value::operator!=(const string &rhs)
{
    return !(*this == rhs);
}

char *strcpy(char *destination, Value &source)
{
    if (source.type() == STRING)
    {
        strcpy(destination, source.as_string().c_str());
    }
    else
    {
        destination[0] = '\0';
    }

    return destination;
}

void *memcpy(void *destination, const Value &source, size_t num)
{
    if (source.type() == STRING)
    {
        memcpy(destination, source.as_string().c_str(), num);
    }

    return destination;
}

/** Indentation counter */
unsigned int ind = 0;

/** Print correct indentation before printing anything */
void indent(ostream &os)
{
    for (unsigned int i = 0; i < ind; i++)
        os << "\t";
}

ostream &operator<<(ostream &os, const Value &v)
{
    switch (v.type())
    {
    case STRING:
        if (ind == 0)
        {
            os << (string)v;
        }
        else
        {
            os << '"' << (string)v << '"';
        }
        break;

    case NESTEDMAP:
        os << (NestedMap)v;
        break;

    case NIL:
        os << "null";
        break;
    }

    return os;
}

ostream &operator<<(ostream &os, const NestedMap &n)
{
    if (n.size() == 0)
    {
        return os;
    }

    os << "{" << endl;

    ind++;
    std::map<std::string, Value>::const_iterator iter;
    for (iter = n.begin(); iter != n.end();)
    {
        indent(os);
        os << '"' << iter->first << '"' << ": " << iter->second;

        if (++iter != n.end())
            os << ",";

        os << endl;
    }
    ind--;
    indent(os);

    os << "}";

    return os;
}
