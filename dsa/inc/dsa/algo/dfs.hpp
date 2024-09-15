#pragma once

#include "dsa/ds/graph.hpp"

#include <functional>
#include <set>
#include <stack>
#include <vector>

namespace dsa
{

template <typename TNode> void dfs(TNode *node, std::function<void(TNode *, void *)> *callback, void *context)
{
    std::set<TNode *> visited;

    std::stack<TNode *> stack;
    stack.push(node);

    while (!stack.empty())
    {
        auto *curr_node = stack.top();
        stack.pop();

        // call the callback
        (*callback)(curr_node, context);

        visited.insert(curr_node);
        for (auto &connection : curr_node->connections)
        {
            if (visited.find(connection) == visited.end())
            {
                stack.push(connection);
            }
        }
    }
}

} // namespace dsa
