#pragma once

#include "dsa/ds/graph.hpp"

#include <functional>
#include <queue>
#include <set>

namespace dsa
{

template <typename TNode> void bfs(TNode *node, std::function<void(TNode *, void *)> *callback, void *context)
{
    std::set<TNode *> visited;

    std::queue<TNode *> queue;
    queue.push(node);

    while (!queue.empty())
    {
        auto *curr_node = queue.front();
        queue.pop();

        // call the callback
        (*callback)(curr_node, context);

        visited.insert(curr_node);
        for (auto *connection : curr_node->connections)
        {
            if (visited.find(connection) == visited.end())
            {
                queue.push(connection);
            }
        }
    }
}
} // namespace dsa