#ifndef _NODE_H_
#define _NODE_H_

#include <ostream>
#include <memory>
#include <vector>
#include <iomanip>

// TODO check Node* pointers
template <typename ITEM>
class Node
{
private:
    typedef std::vector<std::shared_ptr<Node>> childs_t; // TODO unique_ptr?
    typedef typename childs_t::iterator childs_iterator;

public:
    class iterator
    {
    public:
        iterator() : _curr(0), _curr_has_it(false) {}
        iterator(childs_t* curr, const childs_iterator& it, bool curr_has_it = false)
            : _curr(curr), _it(it), _curr_has_it(curr_has_it) {}

        iterator& operator=(const iterator& oth)
        { _curr = oth._curr; _it = oth._it; _curr_has_it = oth._curr_has_it; return *this; }

        bool operator == ( const iterator& oth ) const
        { return _curr == oth._curr && _it == oth._it && _curr_has_it == oth._curr_has_it; }
        bool operator != ( const iterator& oth ) const { return ! operator == ( oth ); }

        typename childs_iterator::value_type* operator->() { return _it.operator->(); }
        typename childs_iterator::value_type& operator*() { return _it.operator*(); }

        childs_t*& elems() { return _curr; }
        childs_iterator& pos() { return _it; }
        bool has_elem() { return _curr_has_it; }
    private:
        // Node* _n;
        childs_t*       _curr;
        childs_iterator _it;
        bool            _curr_has_it;
    };

    static iterator end() { static childs_t end; return iterator(&end, end.end()); };
public:
    Node(const ITEM& item) : _node(new ITEM(item)), _parent(0)
    {
        std::cout << "Node(const ITEM&& item) construct: " << this << std::endl;
    }
    Node(const ITEM& item, Node* parent) : _node(new ITEM(item)), _parent(parent)
    {
        std::cout << "Node(const ITEM&& item, const Node* parent) construct: " << this << std::endl;
    }
    ~Node()
    {
        std::cout << "Node destruct:" << this << std::endl;
    }

    void addChild(const ITEM& child)
    {
        iterator it = find(child);
        if (/*it != end() && */!it.has_elem())
            it.elems()->emplace(it.pos(), new Node<ITEM>(child, this));
    }

    void removeChild(const ITEM& child)
    {
        iterator it = find(child);
        if (/*it != end() && */it.has_elem())
            it.elems()->erase(it.pos());
    }

    void moveChild(const ITEM& child, Node* new_parent)
    {
        if (new_parent)
        {
            auto old_it = find(child);
            if (/*old_it != end() && */old_it.has_elem())
            {
                auto new_it = new_parent->find(child);
                if(/*new_it != end() &&*/!new_it.has_elem() )
                {
                    new_it.pos() = new_it.elems()->insert(new_it.pos(), *old_it);
                    (*new_it)->_parent = new_parent;
                }

                old_it.elems()->erase(old_it.pos());
            }
        }
    }

    void copyChild(const ITEM& child, Node* another_parent)
    {
        if (another_parent)
        {
            auto one_it = find(child);
            if (/*old_it != end() && */one_it.has_elem())
                another_parent->addChild(child);
        }
    }

    void print(int space_count = 1)
    {
        std::cout << std::setw(space_count) << '|'  << "___________________________" << std::endl;
        std::cout << std::setw(space_count) << '|'  << "node:   " << this << " = " << *_node << std::endl;
        std::cout << std::setw(space_count) << '|'  << "parent: " << _parent << " = " << ((_parent) ? *_parent->_node : 0) << std::endl;

        std::cout << std::setw(space_count) << '|'  << "childs: " << _left_childs.size() + _right_childs.size() << std::endl;
        for (auto i = _left_childs.begin(); i != _left_childs.end(); ++i)
            (*i)->print(space_count+1);
        for (auto i = _right_childs.begin(); i != _right_childs.end(); ++i)
            (*i)->print(space_count+1);
        // std::cout << std::setw(space_count) << '|'  << "-------------------------------------" << std::endl;
    }

    Node* getNode(const std::vector<ITEM>& params)
    {
        Node* ret = this;
        for ( const auto& i : params )
        {
            ret = ret->getNode(i);
            if (!ret)
                return 0;
        }

        return ret;
    }

    Node* getNode(const ITEM& child)
    {
        iterator it = find(child);
        return ( /*it != end() && */it.has_elem() ) ? it->get() : 0;
    }

private:
    childs_t* getChilds(const ITEM& child)
    {
        return (child < *_node) ? &_left_childs : &_right_childs;
    }

    iterator find(const ITEM& child)
    {
        // returns it in ordered place
        childs_t* childs = getChilds(child);
        if ( childs->empty() || child < *(*childs->begin())->_node )
            return iterator( childs, childs->begin() );
        else if ( child > *( *(childs->end() - 1) )->_node )
            return iterator( childs, childs->end() );

        size_t first = 0;
        size_t last = childs->size();
        while(first < last)
        {
            auto middle = first + (last - first)/2;
            if (child <= *childs->operator[](middle)->_node)
                last = middle;
            else
                first = middle + 1;
        }

        return iterator( childs, childs->begin() + last,
            (child == *childs->operator[](last)->_node) ? true : false );
    }

private:
    std::shared_ptr<ITEM> _node; // new TODO unique_ptr?

    Node* _parent;  // only ptr
    childs_t _left_childs; // ordered vector. pointers to childs < parent
    childs_t _right_childs; // ordered vector. pointers to childs >= parent

};

#endif // _NODE_H_