#include "dsa/ds/btree.hpp"

#include <gtest/gtest.h>

TEST(Btree, CreateInsertFindClear)
{
    dsa::BTree<int> btree;
    EXPECT_TRUE(btree.empty());

    btree.insert(10);
    EXPECT_EQ(1, btree.size());

    btree.clear();
    EXPECT_EQ(0, btree.size());

    EXPECT_TRUE(btree.insert(10));
    EXPECT_TRUE(btree.insert(11));
    EXPECT_TRUE(btree.insert(9));
    EXPECT_TRUE(btree.insert(8));
    EXPECT_TRUE(btree.insert(15));
    EXPECT_TRUE(btree.insert(3));

    EXPECT_EQ(6, btree.size());

    EXPECT_TRUE(btree.contains(9));
    EXPECT_FALSE(btree.contains(99));
}

TEST(BTree, Depth)
{
    dsa::BTree<int> btree;
    btree.insert(10);
    btree.insert(11);
    btree.insert(12);
    btree.insert(13);
    btree.insert(14);
    btree.insert(15);

    EXPECT_EQ(6, btree.depth());
}