#pragma once

#include <cstdint>
#include <functional>
#include <queue>

namespace dsa
{

template <class T> class BTree
{
private:
    struct BTreeNode
    {
        BTreeNode *left;
        BTreeNode *right;
        T val;

        explicit BTreeNode(const T &v) : left(nullptr), right(nullptr), val(v) {}
        BTreeNode(BTreeNode *l, BTreeNode *r, const T &v) : left(l), right(r), val(v) {}
    };

private:
    BTreeNode *root;
    uint64_t btree_size;

    bool in_order_find(BTreeNode *node, const T &val)
    {
        // recursive iteration through the tree
        if (node == nullptr)
        {
            return false;
        }

        if (val == node->val)
            return true;

        return in_order_find(node->left, val) || in_order_find(node->right, val);
    }

    static uint64_t tree_high(BTreeNode *node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        return std::max(tree_high(node->left), tree_high(node->right)) + 1;
    }

public:
    BTree() : root(nullptr), btree_size(0) {}
    ~BTree() { clear(); }

    [[nodiscard]] bool empty() const { return size() == 0; }
    [[nodiscard]] uint64_t size() const { return btree_size; }

    bool insert(const T &val)
    {
        auto *node = new BTreeNode(val);

        if (root == nullptr)
        {
            root = node;
            ++btree_size;
            return true;
        }

        auto *iter = root;
        while (iter)
        {
            if (iter->val == val)
            {
                return false;
            }

            if (val > iter->val)
            {
                if (iter->right == nullptr)
                {
                    iter->right = node;
                    ++btree_size;
                    return true;
                }
                else
                {
                    iter = iter->right;
                }
            }
            else
            {
                if (iter->left == nullptr)
                {
                    iter->left = node;
                    ++btree_size;
                    return true;
                }
                else
                {
                    iter = iter->left;
                }
            }
        }

        return false;
    }

    void clear()
    {
        // iterative loop through the tree
        if (root == nullptr)
        {
            return;
        }

        std::queue<BTreeNode *> queue;
        queue.push(root);
        while (!queue.empty())
        {
            auto *node = queue.front();
            queue.pop();

            if (node->left != nullptr)
            {
                queue.push(node->left);
            }

            if (node->right != nullptr)
            {
                queue.push(node->right);
            }

            delete node;
            --btree_size;
        }

        root = nullptr;
    }

    bool contains(const T &val) { return in_order_find(root, val); }

    [[nodiscard]] uint64_t depth() const {return  tree_high(root);}
};

} // namespace dsa