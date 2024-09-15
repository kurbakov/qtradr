#pragma once

#include <unordered_set>
#include <utility>
#include <vector>

namespace dsa
{

template <typename T> class DirectedGraph
{
public:
    struct GNode
    {
        std::vector<GNode *> connections;
        T val;

        explicit GNode(T v) : connections{}, val{v} {}
    };

    DirectedGraph() = default;

    ~DirectedGraph()
    {
        for (auto *node : m_node_collection)
        {
            delete node;
        }
    }

    GNode *addVertex(T val)
    {
        auto *node = new GNode(std::move(val));
        m_node_collection.insert(node);
        return node;
    }

    void addEdge(GNode *from, GNode *to) { from->connections.push_back(to); }

private:
    std::unordered_set<GNode *> m_node_collection;
};

} // namespace dsa