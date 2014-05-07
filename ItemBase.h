#ifndef _ITEM_BASE_H
#define _ITEM_BASE_H

class ItemBase
{
public:
	virtual ~ItemBase() {}
    virtual bool operator == (const ItemBase&) const = 0;
    virtual bool operator != (const ItemBase& oth) const final 	{ return !operator==(oth); }

    virtual bool operator < (const ItemBase&) const = 0;
    virtual bool operator <= (const ItemBase& oth) const final 	{ return operator<(oth) || operator==(oth); }
    virtual bool operator > (const ItemBase& oth) const final	{ return !operator<(oth) && !operator==(oth); }
    virtual bool operator >= (const ItemBase& oth) const final 	{ return operator>(oth) || operator==(oth); }
};

#endif // _ITEM_BASE_H