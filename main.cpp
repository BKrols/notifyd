//-----------------------------------------------------------------------------
#include <new>
#include <iostream>

#include "ItemBase.h"
#include "Node.h"
#include "Test.h"
//-----------------------------------------------------------------------------

class Item : public ItemBase
{
public:
    Item(const char* n) : _name(n) { _hash = std::hash<std::string>()(_name); }
    virtual ~Item() { /*std::cout << "Item destruct" << std::endl;*/ }

    virtual bool operator == (const ItemBase& oth_b) const override
    { const auto& oth = fromBase(oth_b); return _hash == oth._hash; };

    virtual bool operator < (const ItemBase& oth_b) const override
    { const auto& oth = fromBase(oth_b); return _hash < oth._hash;  }

    std::ostream& output(std::ostream& out) const { return out << _name; }

private:
    const Item& fromBase(const ItemBase& base) const { return dynamic_cast<const Item&>(base); }

private:
    size_t      _hash;
    std::string _name;
};

std::ostream& operator << (std::ostream& out, const Item& item) { return item.output(out); }

int main(int argc, char const *argv[])
{
    test1 (0, 10);
    test2();
    return 0;
}