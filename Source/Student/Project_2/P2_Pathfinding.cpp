#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall()
{
    return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}

bool ProjectTwo::implemented_jps_plus()
{
    return false;
}
#pragma endregion

bool AStarPather::initialize()
{
    // handle any one-time setup requirements you have

    /*
        If you want to do any map-preprocessing, you'll need to listen
        for the map change message.  It'll look something like this:

        Callback cb = std::bind(&AStarPather::your_function_name, this);
        Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

        There are other alternatives to using std::bind, so feel free to mix it up.
        Callback is just a typedef for std::function<void(void)>, so any std::invoke'able
        object that std::function can wrap will suffice.
    */
  Callback cb = std::bind(&AStarPather::MapCreation, this);
  Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

    return true; // return false if any errors actually occur, to stop engine initialization
}

void AStarPather::shutdown()
{
  m_map.clear();
  m_openList.clear();
}

void AStarPather::MapCreation()
{
  m_openList.clear();
  m_map.clear();

  int height = terrain->get_map_height();
  int width = terrain->get_map_width();
  m_map.reserve(height);

  for (int i = 0; i < height; ++i) {
    m_map.emplace_back();
    m_map[i].reserve(width);

    for (int j = 0; j < width; ++j) {
      m_map[i].emplace_back(std::make_unique<Node>());
      m_map[i][j]->m_gridPos.row = i;
      m_map[i][j]->m_gridPos.col = j;
    }
  }
}

PathResult AStarPather::compute_path(PathRequest &request)
{
  GridPos start = terrain->get_grid_position(request.start);
  GridPos goal = terrain->get_grid_position(request.goal);

  if (!terrain->is_valid_grid_position(start) || !terrain->is_valid_grid_position(goal) || terrain->is_wall(start) || terrain->is_wall(goal))
  {
    return PathResult::IMPOSSIBLE;
  }

  if (request.newRequest)
  {
    MapCreation();

    Node* startNode = m_map[start.row][start.col].get();
    startNode->m_gridPos.row = start.row;
    startNode->m_gridPos.col = start.col;
    startNode->parent = nullptr;
    startNode->m_givenCost = 0;
    startNode->m_finalCost = startNode->m_givenCost + (HeuristicCost(start, goal, request.settings.heuristic) * request.settings.weight);
    startNode->m_list = Open;
    m_openList.insert({ startNode, startNode->m_finalCost });
  }

  if (request.settings.debugColoring)
  {
    terrain->set_color(start, Colors::Orange);
    terrain->set_color(goal, Colors::Orange);
  }
  
  while (!m_openList.empty())
  {
    auto it = std::min_element(m_openList.begin(), m_openList.end(), [](const auto& lhs, const auto& rhs) {return lhs.second < rhs.second;});
    Node* currNode = it->first;

    if (currNode->m_gridPos == goal) {
      Node* pathNode = currNode;
      while (pathNode) {
        request.path.push_back(terrain->get_world_position(pathNode->m_gridPos));
        pathNode = pathNode->parent;
      }
      std::reverse(request.path.begin(), request.path.end());

      return PathResult::COMPLETE;
    }

    m_openList.erase(currNode);
    currNode->m_list = Close;
    if (request.settings.debugColoring)
      terrain->set_color(currNode->m_gridPos, Colors::Yellow);

    NeighbourCalculation(currNode, goal, request.settings.heuristic, request.settings.weight);
    
    for (auto neighbour: currNode->m_neighbours)
    {
      if (neighbour == nullptr)
        continue;

      if (neighbour->m_list == None)
      {
        m_openList.insert({ neighbour, neighbour->m_finalCost });
        neighbour->m_list = Open;
        if (request.settings.debugColoring)
          terrain->set_color(neighbour->m_gridPos, Colors::Blue);
      }
      else if (neighbour->m_list == Open)
      {
        auto member = m_openList.find(neighbour);
        if (member != m_openList.end() && member->second > neighbour->m_finalCost)
        {
          if (member->second > neighbour->m_finalCost)
          {
            auto extraction = m_openList.extract(member);
            extraction.key() = neighbour;
            extraction.mapped() = neighbour->m_finalCost;
            m_openList.insert(std::move(extraction));
            neighbour->m_list = Open;

            if (request.settings.debugColoring)
              terrain->set_color(neighbour->m_gridPos, Colors::Blue);
          }
        }
      }
    }

    if (request.settings.singleStep == true)
    {
      return PathResult::PROCESSING;
    }
  }
  return PathResult::IMPOSSIBLE;
}

float AStarPather::HeuristicCost(const GridPos& a, const GridPos& b, Heuristic mode)
{
  float dx = static_cast<float>(abs(a.row - b.row));
  float dy = static_cast<float>(abs(a.col - b.col));

  switch (mode)
  {
  case Heuristic::EUCLIDEAN:
    return sqrtf(dx * dx + dy * dy);
  case Heuristic::MANHATTAN:
    return dx + dy;
  case Heuristic::OCTILE:
    return (dx + dy) + (sqrtf(2.0f) - 2.0f) * std::min(dx, dy);
  case Heuristic::CHEBYSHEV:
    return std::max(dx, dy);
  case Heuristic::INCONSISTENT:
    if ((int)(dx + dy) % 2 > 0)
      return sqrtf(dx * dx + dy * dy);
    return 0.0f;
  default:
    return 0.0f;
  }
}

void AStarPather::NeighbourCalculation(Node* parent, GridPos goal, Heuristic mode, float weight)
{

  std::vector<std::pair<int, int>> directions = {
      {-1, 0}, // Up
      {1, 0},  // Down
      {0, -1}, // Left
      {0, 1},  // Right
      {-1, -1}, // Top-left diagonal
      {-1, 1}, // Top-right diagonal
      {1, -1}, // Bottom-left diagonal
      {1, 1} // Bottom-right diagonal
  };

  std::vector<float> costs = {
      1.0f, // Up
      1.0f, // Down
      1.0f, // Left
      1.0f, // Right
      1.4f, // Top-left diagonal
      1.4f, // Top-right diagonal
      1.4f, // Bottom-left diagonal
      1.4f, // Bottom-right diagonal
  };

  for (size_t i = 0; i < directions.size(); ++i)
  {
    int newRow = parent->m_gridPos.row + directions[i].first;
    int newCol = parent->m_gridPos.col + directions[i].second;

    if (terrain->is_valid_grid_position(newRow, newCol) && !terrain->is_wall(newRow, newCol) && m_map[newRow][newCol].get() != parent->parent)
    {
      if (i >= 4) { // Check for diagonals
        int adjRow1 = parent->m_gridPos.row + directions[i].first;
        int adjCol1 = parent->m_gridPos.col;
        int adjRow2 = parent->m_gridPos.row;
        int adjCol2 = parent->m_gridPos.col + directions[i].second;

        if (terrain->is_wall(adjRow1, adjCol1) || terrain->is_wall(adjRow2, adjCol2))
          continue;
      }
      
      auto neighbour = m_map[newRow][newCol].get();

      if (neighbour->m_list == Close)
        continue;

      float tentative_givenCost = parent->m_givenCost + costs[i];
      if (neighbour->m_list == None || tentative_givenCost < neighbour->m_givenCost)
      {
        neighbour->parent = parent;
        neighbour->m_givenCost = tentative_givenCost;
        neighbour->m_finalCost = neighbour->m_givenCost + (HeuristicCost(GridPos{ newRow, newCol }, goal, mode) * weight);
        parent->m_neighbours.push_back(neighbour);
      }
    }
  }
}