#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <cassert>

#include "Node.h"
#include "Item.h"

template<typename T>
void test1(T first, T last)
{
    T count = last - first;
    Node<T> root(first++);
    auto* n = &root;
    while ( first < last )
    {
        n = n->addChild(first++);
        assert(n && n->size() == 1);
    }

    assert(root.size() == count);
    root.print();
}

void test2()
{
    /*
                10
               /  \
              5    15
               \     \
                10    15
               /     /
              5    10
    */

    //      10
    Node<int> root(10);
    assert(root.size() == 1);

    //      10
    //     /
    //    5
    root.addChild(5);
    assert(root.size() == 2);

    //      10
    //     /
    //    5
    //     \
    //      10
    auto* n = root.getNode(5);
    assert(n && n->size() == 1);
    n->addChild(10);

    //      10
    //     /
    //    5
    //     \
    //      10
    //     /
    //    5
    n = root.getNode({5,10});
    assert(n && n->size() == 1);
    n->addChild(5);

    //      10
    //     /  \
    //    5    15
    //     \
    //      10
    //     /
    //    5
    root.addChild(15);
    assert(root.size() == 5);

    //      10
    //     /  \
    //    5    15
    //     \     \
    //      10    15
    //     /
    //    5
    n = root.getNode(15);
    assert(n && n->size() == 1);
    n->addChild(15);

    //      10
    //     /  \
    //    5    15
    //     \     \
    //      10    15
    //     /     /
    //    5    10
    n = root.getNode({15,15});
    assert(n && n->size() == 1);
    n->addChild(10);

    // failed
    n = root.getNode({100, 500});
    assert(!n);

    //      10
    //        \
    //         15
    //           \
    //            15
    //           /
    //         10
    //        /
    //       5
    //        \
    //         10
    //        /
    //       5
    root.moveChild(5, root.getNode({15, 15, 10}));
    assert(root.size() == 7);

    //      10______
    //        \     \
    //         15    1111
    //           \
    //            15
    //           /
    //         10
    //        /
    //       5
    //        \
    //         10
    //        /
    //       5
    root.addChild(1111);
    assert(root.size() == 8);

    //      10______
    //        \     \
    //         15    1111
    //           \
    //            15
    //           /
    //         10
    //        /  \
    //       5    1111
    //        \
    //         10
    //        /
    //       5
    root.copyChild(1111, root.getNode({15, 15, 10}));
    assert(root.size() == 9);

    //      10______________
    //        \             \
    //         15_____       1111
    //           \    \
    //            15   22
    //           /
    //         10
    //        /  \
    //       5    1111
    //        \
    //         10
    //        /
    //       5
    n = root.getNode({15, 15, 10});
    assert(n && n->size() == 5);
    n = n->getNode(1111);
    assert(n && n->size() == 1);
    n->addChild(22);
    assert(n->size() == 2);
    auto* i = root.getNode(15);
    assert(i && i->size() == 8);
    n->moveChild(22, i);
    assert(n->size() == 1);


    assert(root.size() == 10);
    root.print();
}

void test3()
{
    Node<Item> tree("a");
    auto* n = &tree;

    n = n->addChild("b");
    assert(n && tree.size() == 2);
    n = n->addChild("c");
    assert(n && tree.size() == 3);
    tree.print();
}

void test()
{
    test1(0, 10);
    test2();
    test3();
    printf("-- All tests passed --\n");
}

#endif // _TEST_H_
