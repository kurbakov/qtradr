#include "dsa/ds/list.hpp"

#include <gtest/gtest.h>

TEST(SLList, PushPopSize)
{
    dsa::SingleLinkedList<int> list;
    EXPECT_TRUE(list.empty());

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    EXPECT_FALSE(list.empty());
    EXPECT_EQ(3, list.size());

    int val;
    list.pop_front(val);
    EXPECT_EQ(1, val);
    EXPECT_EQ(2, list.size());

    list.pop_front(val);
    EXPECT_EQ(2, val);
    EXPECT_EQ(1, list.size());

    list.pop_front(val);
    EXPECT_EQ(3, val);
    EXPECT_EQ(0, list.size());
    EXPECT_TRUE(list.empty());
}

TEST(SLList, Clear)
{
    dsa::SingleLinkedList<int> list;
    EXPECT_TRUE(list.empty());

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    EXPECT_FALSE(list.empty());
    EXPECT_EQ(3, list.size());

    list.clear();

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(0, list.size());
}
