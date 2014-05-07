#ifndef _TEST_H_
#define _TEST_H_

#include "Node.h"

template<typename T>
void test1(T first, T last)
{
    Node<T> root(0);
    auto* n = &root;
    n->addChild(first);
    while ( first < last )
    {
        if (n = n->getNode(first++))
            n->addChild(first);
        else
            break;
    }

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
    //      10
    //     /
    //    5
    root.addChild(5);
    //      10
    //     /
    //    5
    //     \
    //      10
    auto* n = root.getNode(5);
    if (n)
        n->addChild(10);
    //      10
    //     /
    //    5
    //     \
    //      10
    //     /
    //    5
    if(n = root.getNode({5,10}))
        n->addChild(5);

    //      10
    //     /  \
    //    5    15
    //     \
    //      10
    //     /
    //    5
    root.addChild(15);

    //      10
    //     /  \
    //    5    15
    //     \     \
    //      10    15
    //     /
    //    5
    if(n = root.getNode(15))
        n->addChild(15);

    //      10
    //     /  \
    //    5    15
    //     \     \
    //      10    15
    //     /     /
    //    5    10
    if(n = root.getNode({15,15}))
        n->addChild(10);

    // failed
    if (n = root.getNode({100, 500}))
        n->addChild(1515);

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
    if (n = root.getNode({15, 15, 10}))
    {
        if(n = n->getNode(1111))
        {
            n->addChild(22);
            auto* i = root.getNode(15);
            if (i)
                n->moveChild(22, i);
        }
    }

    root.print();
}

#endif // _TEST_H_