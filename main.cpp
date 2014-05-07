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
    Item(int t) : _test(t) { /*std::cout << "Item construct" << std::endl;*/ }
    virtual ~Item() { /*std::cout << "Item destruct" << std::endl;*/ }

    virtual bool operator == (const ItemBase& oth_b) const override
    { const auto& oth = fromBase(oth_b); return _test == oth._test; };

    virtual bool operator < (const ItemBase& oth_b) const override
    { const auto& oth = fromBase(oth_b); return _test < oth._test;  }

    std::ostream& print(std::ostream& out) const { return out << _test; }

private:
    const Item& fromBase(const ItemBase& base) const { return dynamic_cast<const Item&>(base); }

private:
    int _test;
};

std::ostream& operator << (std::ostream& out, const Item& item) { return item.print(out); }

int main(int argc, char const *argv[])
{
    // test1 (0, 10);
    test2();
}