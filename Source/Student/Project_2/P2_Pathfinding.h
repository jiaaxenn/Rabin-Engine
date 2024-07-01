#pragma once
#include "Misc/PathfindingDetails.hpp"
#include <map>

class AStarPather
{
public:
    /* 
        The class should be default constructible, so you may need to define a constructor.
        If needed, you can modify the framework where the class is constructed in the
        initialize functions of ProjectTwo and ProjectThree.
    */

    /* ************************************************** */
    // DO NOT MODIFY THESE SIGNATURES
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);
    /* ************************************************** */

    /*
        You should create whatever functions, variables, or classes you need.
        It doesn't all need to be in this header and cpp, structure it whatever way
        makes sense to you.
    */
  enum onList
  {
    None,
    Open,
    Close
  };

  class Node
  {
  public:
    Node() = default;
    Node(GridPos pos, Node* parent = nullptr, float g_cost = 0.0f, float f_cost = 0.0f, onList list = None) : m_gridPos(pos), parent(parent), m_givenCost(g_cost), m_finalCost(f_cost), m_list(list) {}
    float m_finalCost;
    float m_givenCost;
    onList m_list;
    GridPos m_gridPos;
    Node* parent;
    std::vector<Node*> m_neighbours;
  };

  float HeuristicCost(const GridPos& a, const GridPos& b, Heuristic mode);
  void NeighbourCalculation(Node* parent, GridPos goal, Heuristic mode, float weight);
  void MapCreation();

  std::vector<std::vector<std::unique_ptr<Node>>> m_map;

  struct NodeComparator {
    bool operator()(const Node* lhs, const Node* rhs) const {
      return lhs->m_finalCost > rhs->m_finalCost;
    }
  };

  std::map<Node*, float> m_openList;
};