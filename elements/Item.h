#ifndef _ITEM_H_
#define _ITEM_H_

#include <string>
#include <iostream>

#include "ItemBase.h"

class Item : public ItemBase
{
public:
    Item(const char* n) : _name(n) { _hash = hash(_name); }
    virtual ~Item() {}

    const std::string& getName() { return _name; }

    virtual bool operator == (const ItemBase& oth_b) const override
    { const auto& oth = fromBase(oth_b); return _hash == oth._hash; };

    virtual bool operator < (const ItemBase& oth_b) const override
    { const auto& oth = fromBase(oth_b); return _hash < oth._hash;  }

    std::ostream& output(std::ostream& out) const { return out << _hash << ": " << _name; }
    static size_t hash(const std::string& s) { static std::hash<std::string> hash_f; return hash_f(s); }

private:
    const Item& fromBase(const ItemBase& base) const { return dynamic_cast<const Item&>(base); }

private:
    size_t      _hash;
    std::string _name;
};

inline std::ostream& operator << (std::ostream& out, const Item& item) { return item.output(out); }

#endif // _ITEM_H_