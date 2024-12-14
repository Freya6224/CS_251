#pragma once

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

/// @brief Simple directed graph using an adjacency list.
/// @tparam VertexT vertex type
/// @tparam WeightT edge weight type
template <typename VertexT, typename WeightT>
class graph {
 private:
  // TODO_STUDENT
  unordered_map<VertexT, map<VertexT, WeightT>> list;

 public:
  /// Default constructor
  graph() {
  }

  /// @brief Add the vertex `v` to the graph, must typically be O(1).
  /// @param v
  /// @return true if successfully added; false if it existed already
  bool addVertex(VertexT v) {
    if (list.count(v) != 0) {
      return false;
    }
    list[v] = map<VertexT, WeightT>();
    return true;
  }

  /// @brief Add or overwrite directed edge in the graph, must typically be
  /// O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight edge weight / label
  /// @return true if successfully added or overwritten;
  ///         false if either vertices isn't in graph
  bool addEdge(VertexT from, VertexT to, WeightT weight) {
    // TODO_STUDENT
    if (list.count(from) != 0 && list.count(to) != 0) {
      list[from][to] = weight;
      return true;
    }
    return false;
  }

  /// @brief Maybe get the weight associated with a given edge, must typically
  /// be O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight output parameter
  /// @return true if the edge exists, and `weight` is set;
  ///         false if the edge does not exist
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
    // TODO_STUDENT
    if (list.count(from) != 0 && list.count(to) != 0 &&
        list.at(from).count(to) != 0) {
      weight = list.at(from).at(to);
      return true;
    } else {
      return false;
    }

    return false;
  }

  /// @brief Get the out-neighbors of `v`. Must run in at most O(|V|).
  /// @param v
  /// @return vertices that v has an edge to
  set<VertexT> neighbors(VertexT v) const {
    set<VertexT> S;
    if (list.count(v)) {
      for (const auto& theEdge : list.at(v)) {
        S.insert(theEdge.first);
      }
    }

    return S;
  }

  /// @brief Return a vector containing all vertices in the graph
  vector<VertexT> getVertices() const {
    vector<VertexT> vertices;
    for (auto vert : list) {
      vertices.push_back(vert.first);
    }
    return vertices;
  }

  /// @brief Get the number of vertices in the graph. Runs in O(1).
  size_t numVertices() const {
    return list.size();
  }

  /// @brief Get the number of directed edges in the graph. Runs in at most
  /// O(|V|), but should be O(1).
  size_t numEdges() const {
    size_t numEdges = 0;
    for (const auto& vert : list) {
      numEdges += (vert.second.size());
    }
    return numEdges;
  }
};
