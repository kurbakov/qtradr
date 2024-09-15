#include "dsa/algo/dfs.hpp"

#include <gtest/gtest.h>

TEST(DFS, GetAllNodes)
{
    /*
     build the following graph
          1
        /  \
      2     3
      |     |
      4     5
    */

    dsa::DirectedGraph<int> graph;
    auto* v1 = graph.addVertex(1);
    auto* v2 = graph.addVertex(2);
    auto* v3 = graph.addVertex(3);
    auto* v4 = graph.addVertex(4);
    auto* v5 = graph.addVertex(5);

    graph.addEdge(v1, v2);
    graph.addEdge(v1, v3);
    graph.addEdge(v2, v4);
    graph.addEdge(v3, v5);

    struct container {
        std::vector<int> res;
    } cont;

    std::function<void(dsa::DirectedGraph<int>::GNode*, void*)> callback = [](dsa::DirectedGraph<int>::GNode* node, void* context){
        auto* data_store = reinterpret_cast<container*>(context);
        data_store->res.push_back(node->val);
    };

    dsa::dfs(v1, &callback, &cont);

    std::vector<int> expected{1, 2, 4, 3, 5};

    EXPECT_EQ(expected.size(), cont.res.size());
}