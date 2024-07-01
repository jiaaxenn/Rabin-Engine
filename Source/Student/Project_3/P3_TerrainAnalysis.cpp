#include <pch.h>
#include "Terrain/TerrainAnalysis.h"
#include "Terrain/MapMath.h"
#include "Agent/AStarAgent.h"
#include "Terrain/MapLayer.h"
#include "Projects/ProjectThree.h"

#include <iostream>

bool ProjectThree::implemented_fog_of_war() const // extra credit
{
    return false;
}

float distance_to_closest_wall(int row, int col)
{
    /*
        Check the euclidean distance from the given cell to every other wall cell,
        with cells outside the map bounds treated as walls, and return the smallest
        distance.  Make use of the is_valid_grid_position and is_wall member
        functions in the global terrain to determine if a cell is within map bounds
        and a wall, respectively.
    */

    // WRITE YOUR CODE HERE
  float minDistance = std::numeric_limits<float>::max();
  int mapHeight = terrain->get_map_height();
  int mapWidth = terrain->get_map_width();

  // Check distance to the edges of the map
  float distanceToLeftEdge = (float)col + 1.f;
  float distanceToRightEdge = (float)(mapWidth - col);
  float distanceToBottomEdge = (float)row + 1.f;
  float distanceToTopEdge = (float)(mapHeight - row);

  minDistance = std::min({ minDistance, distanceToLeftEdge, distanceToRightEdge, distanceToTopEdge, distanceToBottomEdge });

  // Check distance to the walls
  for (int r = 0; r < mapHeight; ++r) {
    for (int c = 0; c < mapWidth; ++c) {
      if (terrain->is_wall(r, c)) {
        float distance = (float)std::sqrt(std::pow(r - row, 2) + std::pow(c - col, 2));
        minDistance = std::min(minDistance, distance);
      }
    }
  }

  return minDistance;
  
  /*float minDistance = std::numeric_limits<float>::max();
  for (int r = 0; r < terrain->get_map_height(); ++r) {
    for (int c = 0; c < terrain->get_map_width(); ++c) {
      if (terrain->is_wall(r, c) || !terrain->is_valid_grid_position(r, c)) {
        float distance = (float)std::sqrt(std::pow(r - row, 2) + std::pow(c - col, 2));
        minDistance = std::min(minDistance, distance);
      }
    }
  }
  return minDistance;*/
}

bool is_clear_path(int row0, int col0, int row1, int col1)
{
    /*
        Two cells (row0, col0) and (row1, col1) are visible to each other if a line
        between their centerpoints doesn't intersect the four boundary lines of every
        wall cell.  You should puff out the four boundary lines by a very tiny amount
        so that a diagonal line passing by the corner will intersect it.  Make use of the
        line_intersect helper function for the intersection test and the is_wall member
        function in the global terrain to determine if a cell is a wall or not.
    */

    // WRITE YOUR CODE HERE
  
  // Convert grid positions to center points
  Vec2 start((float)row0 + 0.5f, (float)col0 + 0.5f);
  Vec2 end((float)row1 + 0.5f, (float)col1 + 0.5f);

  // Determine the bounding box of the area that contains the two cells
  int minRow = std::min(row0, row1);
  int maxRow = std::max(row0, row1);
  int minCol = std::min(col0, col1);
  int maxCol = std::max(col0, col1);

  // Iterate through the bounding box
  for (int row = minRow; row <= maxRow; ++row)
  {
    for (int col = minCol; col <= maxCol; ++col)
    {
      if (terrain->is_wall(row, col))
      {
        // Get the boundary points of the wall cell, slightly puffed out
        Vec2 topLeft((float)row + 1.0f, (float)col);
        Vec2 topRight((float)row + 1.0f, (float)col + 1.0f);
        Vec2 bottomLeft((float)row, (float)col + 1.0f);
        Vec2 bottomRight((float)row, (float)col);

        // Check for intersections with all four sides of the wall cell
        if (line_intersect(start, end, topLeft, topRight) ||
          line_intersect(start, end, topRight, bottomRight) ||
          line_intersect(start, end, bottomRight, bottomLeft) ||
          line_intersect(start, end, bottomLeft, topLeft))
        {
          return false; // Path is blocked
        }
      }
    }
  }

  return true; // Path is clear
  
  
  //for (int row = row0; row <= row1; ++row)
  //{
  //  for (int col = col0; col <= col1; ++col)
  //  {
  //    if (terrain->is_wall(row, col))
  //    {
  //      // Get the boundary points of the wall cell
  //      Vec2 topLeft = Vec2(row + 0.51f, col - 0.51f);
  //      Vec2 topRight = Vec2(row + 0.51f, col + 0.51f);
  //      Vec2 bottomLeft = Vec2(row - 0.51f, col - 0.51f);
  //      Vec2 bottomRight = Vec2(row - 0.51f, col + 0.51f);

  //      if (line_intersect(Vec2((float)row0, (float)col0), Vec2((float)row1, (float)col1), topLeft, topRight) ||
  //        line_intersect(Vec2((float)row0, (float)col0), Vec2((float)row1, (float)col1), topRight, bottomRight) ||
  //        line_intersect(Vec2((float)row0, (float)col0), Vec2((float)row1, (float)col1), bottomRight, bottomLeft) ||
  //        line_intersect(Vec2((float)row0, (float)col0), Vec2((float)row1, (float)col1), bottomLeft, topLeft))
  //          return false; // Path is blocked
  //    }
  //  }
  //}

  //return true;
}

void analyze_openness(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the value 1 / (d * d),
        where d is the distance to the closest wall or edge.  Make use of the
        distance_to_closest_wall helper function.  Walls should not be marked.
    */

    // WRITE YOUR CODE HERE
  for (int row{}; row < terrain->get_map_height(); ++row) {
    for (int col{}; col < terrain->get_map_width(); ++col) {
      if (!terrain->is_wall(row, col)) {
        float distance = distance_to_closest_wall(row, col);
        float opennessValue = 1.0f / (distance * distance);
        layer.set_value(row, col, opennessValue);
      }
    }
  }
}

void analyze_visibility(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the number of cells that
        are visible to it, divided by 160 (a magic number that looks good).  Make sure
        to cap the value at 1.0 as well.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE
  for (int row = 0; row < terrain->get_map_height(); ++row)
  {
    for (int col = 0; col < terrain->get_map_width(); ++col)
    {
      if (terrain->is_wall(row, col))
      {
        layer.set_value(row, col, 0.0f);
        continue;
      }

      int numOfVisibleCells = 0;

      //inner loop -> to compare to every other cell
      for (int r = 0; r < terrain->get_map_height(); ++r)
      {
        for (int c = 0; c < terrain->get_map_width(); ++c)
        {
          if (!(r == row && c == col)) //if not comparing the same cell
          {
            if (is_clear_path(row, col, r, c))
            {
              ++numOfVisibleCells;
            }
          }
        }
      }

      float value = (float)numOfVisibleCells / 160.0f;
      if (value > 1.0f)
      {
        value = 1.0f;
      }
      layer.set_value(row, col, value);
    }
  }
  
  //int numOfVisibleCells{};
  ////outer loop -> cell to mark
  //for (int row{}; row < terrain->get_map_height(); ++row)
  //{
  //  for (int col{}; col < terrain->get_map_width(); ++col)
  //  {
  //    //inner loop -> to compare to every other cell
  //    for (int r{}; r < terrain->get_map_height(); ++r)
  //    {
  //      for (int c{}; c < terrain->get_map_width(); ++c)
  //      {
  //        if (r != row && c != col) //if not comparing same cell
  //        {
  //          if (is_clear_path(row, col, r, c))
  //            ++numOfVisibleCells;
  //        }
  //      }
  //    }

  //    float value = (float)numOfVisibleCells / 160;
  //    if (value > 1.f)
  //      value = 1.f;
  //    layer.set_value(row, col, value);
  //  }
  //}
}

static std::vector<std::pair<int, int>> GetNeighbours(int row, int col)
{
  std::vector<std::pair<int, int>> neighbours{};
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

  for (int i{}; i < directions.size(); ++i)
  {
    int neighbourRow = row + directions[i].first;
    int neighbourCol = col + directions[i].second;

    if (terrain->is_valid_grid_position(neighbourRow, neighbourCol) && !terrain->is_wall(neighbourRow, neighbourCol))
    {
      if (i >= 4) { // Check if diagonals is inbetween walls
        int adjRow1 = row + directions[i].first;
        int adjCol1 = col;
        int adjRow2 = row;
        int adjCol2 = col + directions[i].second;

        if (terrain->is_wall(adjRow1, adjCol1) || terrain->is_wall(adjRow2, adjCol2))
          continue;
      }

      neighbours.push_back({ neighbourRow, neighbourCol });
    }
  }

  return neighbours;
}

void analyze_visible_to_cell(MapLayer<float> &layer, int row, int col)
{
    /*
        For every cell in the given layer mark it with 1.0
        if it is visible to the given cell, 0.5 if it isn't visible but is next to a visible cell,
        or 0.0 otherwise.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE
  // First, clear the layer
  for (int r = 0; r < terrain->get_map_height(); ++r)
  {
    for (int c = 0; c < terrain->get_map_width(); ++c)
    {
      layer.set_value(r, c, 0.0f);
    }
  }

  // Mark cells that are visible from the given cell with 1.0f
  for (int r = 0; r < terrain->get_map_height(); ++r)
  {
    for (int c = 0; c < terrain->get_map_width(); ++c)
    {
      if (is_clear_path(row, col, r, c) && !terrain->is_wall(r, c))
      {
        layer.set_value(r, c, 1.0f);
      }
    }
  }

  // Now mark the neighbors of visible cells with 0.5f if they are not visible from the original cell
  for (int r = 0; r < terrain->get_map_height(); ++r)
  {
    for (int c = 0; c < terrain->get_map_width(); ++c)
    {
      if (layer.get_value(r, c) == 1.0f)
      {
        std::vector<std::pair<int, int>> neighbors = GetNeighbours(r, c);
        for (const auto& neighbor : neighbors)
        {
          int nr = neighbor.first;
          int nc = neighbor.second;
          if (terrain->is_valid_grid_position(nr, nc) && !terrain->is_wall(nr, nc) && layer.get_value(nr, nc) == 0.0f)
          {
            if (!is_clear_path(row, col, nr, nc))
            {
              layer.set_value(nr, nc, 0.5f);
            }
          }
        }
      }
    }
  }
  
  
  //for (int r{}; r < terrain->get_map_height(); ++r)
  //{
  //  for (int c{}; c < terrain->get_map_width(); ++c)
  //  {
  //    if (is_clear_path(row, col, r, c) && !terrain->is_wall(r, c))
  //    {
  //      layer.set_value(r, c, 1.f);

  //      //setting neighbours
  //      std::vector<std::pair<int, int>> neighbours = GetNeighbours(r, c);
  //      for each (std::pair<int, int> neighbour in neighbours)
  //      {
  //        if (!is_clear_path(row, col, neighbour.first, neighbour.second))
  //          layer.set_value(neighbour.first, neighbour.second, 0.5f);
  //      }
  //    }
  //    else
  //      layer.set_value(r, c, 0.f);
  //  }
  //}
}

void analyze_agent_vision(MapLayer<float> &layer, const Agent *agent)
{
    /*
        For every cell in the given layer that is visible to the given agent,
        mark it as 1.0, otherwise don't change the cell's current value.

        You must consider the direction the agent is facing.  All of the agent data is
        in three dimensions, but to simplify you should operate in two dimensions, the XZ plane.

        Take the dot product between the view vector and the vector from the agent to the cell,
        both normalized, and compare the cosines directly instead of taking the arccosine to
        avoid introducing floating-point inaccuracy (larger cosine means smaller angle).

        Give the agent a field of view slighter larger than 180 degrees.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE
  // Get agent's position and view direction
  Vec3 agentPos = agent->get_position();
  Vec3 agentDir = agent->get_forward_vector();

  Vec2 v(agentDir.x, agentDir.z);
  v.Normalize();

  Vec2 agentPos2D(agentPos.x, agentPos.z);

  float fovCosine = (float)std::cos((M_PI * 1.1) / 2.f); // Slightly larger than 180 degrees

  for (int row = 0; row < terrain->get_map_height(); ++row) 
  {
    for (int col = 0; col < terrain->get_map_width(); ++col) 
    {
      Vec3 cellPos3D = terrain->get_world_position(row, col);
      Vec2 cellPos2D(cellPos3D.x, cellPos3D.z);

      // Vector from agent to cell
      Vec2 u = cellPos2D - agentPos2D;
      u.Normalize();

      // Dot product between agent's view direction and vector to the cell
      float dotProduct = v.Dot(u);

      if (dotProduct >= fovCosine)
      {
        // Check if there are no obstacles blocking the view
        if (is_clear_path(terrain->get_grid_position(agentPos).row, terrain->get_grid_position(agentPos).col, row, col))
        {
          layer.set_value(row, col, 1.0f);
        }
      }
    }
  }
}

void propagate_solo_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        For every cell in the given layer:

            1) Get the value of each neighbor and apply decay factor
            2) Keep the highest value from step 1
            3) Linearly interpolate from the cell's current value to the value from step 2
               with the growing factor as a coefficient.  Make use of the lerp helper function.
            4) Store the value from step 3 in a temporary layer.
               A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */
    
    // WRITE YOUR CODE HERE
  float tempLayer[40][40]{};
  for (int row = 0; row < terrain->get_map_height(); ++row)
  {
    for (int col = 0; col < terrain->get_map_width(); ++col)
    {
      if (terrain->is_wall(row, col) || !terrain->is_valid_grid_position(row, col))
        continue;
      float highestValue{};

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

      for (int i{}; i < directions.size(); ++i)
      {
        int neighbourRow = row + directions[i].first;
        int neighbourCol = col + directions[i].second;

        if (terrain->is_valid_grid_position(neighbourRow, neighbourCol) && !terrain->is_wall(neighbourRow, neighbourCol))
        {
          float value = layer.get_value(neighbourRow, neighbourCol);

          if (i >= 4) { // Check if diagonals is inbetween walls
            int adjRow1 = row + directions[i].first;
            int adjCol1 = col;
            int adjRow2 = row;
            int adjCol2 = col + directions[i].second;

            if (terrain->is_wall(adjRow1, adjCol1) || terrain->is_wall(adjRow2, adjCol2))
              continue;

            value *= (float)exp(-sqrt(2) * decay);
          }
          else
            value *= (float)exp(-1 * decay);

          if (value > highestValue)
            highestValue = value;
        }
      }

      tempLayer[row][col] = lerp(layer.get_value(row, col), highestValue, growth);
    }
  }

  for (int row = 0; row < terrain->get_map_height(); ++row)
  {
    for (int col = 0; col < terrain->get_map_width(); ++col)
    {
      layer.set_value(row, col, tempLayer[row][col]);
    }
  }
}

void propagate_dual_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        Similar to the solo version, but the values range from -1.0 to 1.0, instead of 0.0 to 1.0

        For every cell in the given layer:

        1) Get the value of each neighbor and apply decay factor
        2) Keep the highest ABSOLUTE value from step 1
        3) Linearly interpolate from the cell's current value to the value from step 2
           with the growing factor as a coefficient.  Make use of the lerp helper function.
        4) Store the value from step 3 in a temporary layer.
           A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */

    // WRITE YOUR CODE HERE
  //float tempLayer[40][40]{};
  //for (int row = 0; row < terrain->get_map_height(); ++row)
  //{
  //  for (int col = 0; col < terrain->get_map_width(); ++col)
  //  {
  //    float currValue = layer.get_value(row, col);
  //    float highestValue{};

  //    std::vector<std::pair<int, int>> directions = {
  //    {-1, 0}, // Up
  //    {1, 0},  // Down
  //    {0, -1}, // Left
  //    {0, 1},  // Right
  //    {-1, -1}, // Top-left diagonal
  //    {-1, 1}, // Top-right diagonal
  //    {1, -1}, // Bottom-left diagonal
  //    {1, 1} // Bottom-right diagonal
  //    };

  //    for (int i{}; i < directions.size(); ++i)
  //    {
  //      int neighbourRow = row + directions[i].first;
  //      int neighbourCol = col + directions[i].second;

  //      if (terrain->is_valid_grid_position(neighbourRow, neighbourCol) && !terrain->is_wall(neighbourRow, neighbourCol))
  //      {
  //        float value = layer.get_value(neighbourRow, neighbourCol);

  //        if (i >= 4) { // Check if diagonals is inbetween walls
  //          int adjRow1 = row + directions[i].first;
  //          int adjCol1 = col;
  //          int adjRow2 = row;
  //          int adjCol2 = col + directions[i].second;

  //          if (terrain->is_wall(adjRow1, adjCol1) || terrain->is_wall(adjRow2, adjCol2))
  //            continue;

  //          value = (float)abs(value * exp(-sqrt(2) * decay));
  //        }
  //        else
  //          value = (float)abs(value * exp(-1 * decay));

  //        if (value > highestValue)
  //          highestValue = value;
  //      }
  //    }

  //    tempLayer[row][col] = lerp(layer.get_value(row, col), highestValue, growth);
  //  }
  //}

  //for (int row = 0; row < terrain->get_map_height(); ++row)
  //{
  //  for (int col = 0; col < terrain->get_map_width(); ++col)
  //  {
  //    layer.set_value(row, col, tempLayer[row][col]);
  //  }
  //}
}

void normalize_solo_occupancy(MapLayer<float> &layer)
{
    /*
        Determine the maximum value in the given layer, and then divide the value
        for every cell in the layer by that amount.  This will keep the values in the
        range of [0, 1].  Negative values should be left unmodified.
    */

    // WRITE YOUR CODE HERE
  float highestValue{};
  for (int row = 0; row < terrain->get_map_height(); ++row)
  {
    for (int col = 0; col < terrain->get_map_width(); ++col)
    {
      if (layer.get_value(row, col) > highestValue)
        highestValue = layer.get_value(row, col);
    }
  }

  if (highestValue > 0)
  {
    for (int row = 0; row < terrain->get_map_height(); ++row)
    {
      for (int col = 0; col < terrain->get_map_width(); ++col)
      {
        float value = layer.get_value(row, col);
        if (layer.get_value(row, col) > 0)
          layer.set_value(row, col, value / highestValue);
      }
    }
  }
}

void normalize_dual_occupancy(MapLayer<float> &layer)
{
    /*
        Similar to the solo version, but you need to track greatest positive value AND 
        the least (furthest from 0) negative value.

        For every cell in the given layer, if the value is currently positive divide it by the
        greatest positive value, or if the value is negative divide it by -1.0 * the least negative value
        (so that it remains a negative number).  This will keep the values in the range of [-1, 1].
    */

    // WRITE YOUR CODE HERE
  /*float highestValue{};
  float lowestValue{};
  for (int row = 0; row < terrain->get_map_height(); ++row)
  {
    for (int col = 0; col < terrain->get_map_width(); ++col)
    {
      if (layer.get_value(row, col) > highestValue)
        highestValue = layer.get_value(row, col);
      if (layer.get_value(row, col) < lowestValue)
        lowestValue = layer.get_value(row, col);
    }
  }

  for (int row = 0; row < terrain->get_map_height(); ++row)
  {
    for (int col = 0; col < terrain->get_map_width(); ++col)
    {
      if (layer.get_value(row, col) < 0)
        layer.set_value(row, col, layer.get_value(row, col) / (-1.0f * lowestValue));
      else
        layer.set_value(row, col, layer.get_value(row, col) / highestValue);
    }
  }*/
}

void enemy_field_of_view(MapLayer<float> &layer, float fovAngle, float closeDistance, float occupancyValue, AStarAgent *enemy)
{
    /*
        First, clear out the old values in the map layer by setting any negative value to 0.
        Then, for every cell in the layer that is within the field of view cone, from the
        enemy agent, mark it with the occupancy value.  Take the dot product between the view
        vector and the vector from the agent to the cell, both normalized, and compare the
        cosines directly instead of taking the arccosine to avoid introducing floating-point
        inaccuracy (larger cosine means smaller angle).

        If the tile is close enough to the enemy (less than closeDistance),
        you only check if it's visible to enemy.  Make use of the is_clear_path
        helper function.  Otherwise, you must consider the direction the enemy is facing too.
        This creates a radius around the enemy that the player can be detected within, as well
        as a fov cone.
    */

    // WRITE YOUR CODE HERE
  // Clear old values in the map layer
  for (int row = 0; row < terrain->get_map_height(); ++row)
  {
    for (int col = 0; col < terrain->get_map_width(); ++col)
    {
      if (layer.get_value(row, col) < 0)
        layer.set_value(row, col, 0.f);
    }
  }

  // Get enemy's position and view direction
  Vec3 enemyPos = enemy->get_position();
  Vec3 enemyDir = enemy->get_forward_vector();
  GridPos enemyPosGrid = terrain->get_grid_position(enemyPos);

  // Normalize the view direction vector (in the XZ plane)
  Vec2 enemyDir2D(enemyDir.x, enemyDir.z);
  enemyDir2D.Normalize();

  // Calculate the cosine of the half field of view angle
  float fovCosine = (float)std::cos((fovAngle / 2.0f) * (M_PI / 180.0f)); // Convert FOV angle to radians and divide by 2

  for (int row = 0; row < terrain->get_map_height(); ++row)
  {
    for (int col = 0; col < terrain->get_map_width(); ++col)
    {
      if (terrain->is_wall(row, col) || !terrain->is_valid_grid_position(row, col))
        continue;

      Vec3 cellPos3D = terrain->get_world_position(row, col);
      Vec2 cellPos2D(cellPos3D.x, cellPos3D.z);

      // Vector from enemy to cell
      Vec2 toCell2D = cellPos2D - Vec2(enemyPos.x, enemyPos.z);
      float distance = toCell2D.Length();

      // Normalize the vector to the cell
      toCell2D.Normalize();

      // Dot product between enemy's view direction and vector to the cell
      float dotProduct = enemyDir2D.Dot(toCell2D);

      // Check if the cell is within the enemy's field of view cone or within close distance
      if (distance < closeDistance || dotProduct > fovCosine)
      {
        // Check if there are no obstacles blocking the view
        if (is_clear_path(enemyPosGrid.row, enemyPosGrid.col, row, col))
          layer.set_value(row, col, occupancyValue);
      }
    }
  }
  
  //for (int row = 0; row < terrain->get_map_height(); ++row)
  //{
  //  for (int col = 0; col < terrain->get_map_width(); ++col)
  //  {
  //    if (layer.get_value(row, col) < 0)
  //      layer.set_value(row, col, 0.f);
  //  }
  //}

  //// Get enemy's position and view direction
  //Vec3 enemyPos = enemy->get_position();
  //Vec3 enemyDir = enemy->get_forward_vector();
  //GridPos enemyPosGrid = terrain->get_grid_position(enemyPos);

  //// Normalize the view direction vector (in the XZ plane)
  //Vec2 enemyDir2D(enemyDir.x, enemyDir.z);
  //enemyDir2D.Normalize();

  //// Calculate the cosine of the field of view angle
  //float fovCosine = std::cos(fovAngle);

  //for (int row = 0; row < terrain->get_map_height(); ++row) 
  //{
  //  for (int col = 0; col < terrain->get_map_width(); ++col) 
  //  {
  //    if (terrain->is_wall(row, col) || !terrain->is_valid_grid_position(row, col))
  //      continue;
  //    Vec3 cellPos3D = terrain->get_world_position(row, col);
  //    Vec2 cellPos2D(cellPos3D.x, cellPos3D.z);

  //    // Vector from enemy to cell
  //    Vec2 toCell2D = cellPos2D - Vec2(enemyPos.x, enemyPos.z);
  //    float distance = toCell2D.Length();

  //    // Normalize the vector to the cell
  //    toCell2D.Normalize();

  //    // Dot product between enemy's view direction and vector to the cell
  //    float dotProduct = enemyDir2D.Dot(toCell2D);

  //    // Check if the cell is within the enemy's field of view cone or within close distance
  //    if (distance < closeDistance || dotProduct > fovCosine)
  //      // Check if there are no obstacles blocking the view
  //      if (is_clear_path(enemyPosGrid.row, enemyPosGrid.col, row, col))
  //        layer.set_value(row, col, occupancyValue);
  //  }
  //}
}

bool enemy_find_player(MapLayer<float> &layer, AStarAgent *enemy, Agent *player)
{
    /*
        Check if the player's current tile has a negative value, ie in the fov cone
        or within a detection radius.
    */

    const auto &playerWorldPos = player->get_position();

    const auto playerGridPos = terrain->get_grid_position(playerWorldPos);

    // verify a valid position was returned
    if (terrain->is_valid_grid_position(playerGridPos) == true)
    {
        if (layer.get_value(playerGridPos) < 0.0f)
        {
            return true;
        }
    }

    // player isn't in the detection radius or fov cone, OR somehow off the map
    return false;
}

bool enemy_seek_player(MapLayer<float> &layer, AStarAgent *enemy)
{
    /*
        Attempt to find a cell with the highest nonzero value (normalization may
        not produce exactly 1.0 due to floating point error), and then set it as
        the new target, using enemy->path_to.

        If there are multiple cells with the same highest value, then pick the
        cell closest to the enemy.

        Return whether a target cell was found.
    */

    // WRITE YOUR CODE HERE
  float highestValue = -1.0f;  // Initialize to a value that any cell value will be higher than
  Vec3 targetPos;  // Position of the target cell
  bool targetFound = false;

  Vec3 enemyPos = enemy->get_position();
  Vec2 enemyPos2D(enemyPos.x, enemyPos.z);

  for (int row = 0; row < terrain->get_map_height(); ++row)
  {
    for (int col = 0; col < terrain->get_map_width(); ++col)
    {
      float cellValue = layer.get_value(row, col);
      if (cellValue > 0)  // Only consider nonzero values
      {
        Vec3 cellWorldPos = terrain->get_world_position(row, col);
        Vec2 cellPos2D(cellWorldPos.x, cellWorldPos.z);

        if (cellValue > highestValue)
        {
          highestValue = cellValue;
          targetPos = cellWorldPos;
          targetFound = true;
        }
        else if (cellValue == highestValue)
        {
          Vec2 currentTargetPos2D(targetPos.x, targetPos.z);
          if ((cellPos2D - enemyPos2D).Length() < (currentTargetPos2D - enemyPos2D).Length())
          {
            targetPos = cellWorldPos;
          }
        }
      }
    }
  }

  if (targetFound)
  {
    enemy->path_to(targetPos);
  }

  return targetFound;
  
  /*float highestValue{};
  Vec3 pos{};
  Vec2 pos2D{};
  bool targetFound{false};
  Vec3 enemyPos = enemy->get_position();
  Vec2 enemyPos2D(enemyPos.x, enemyPos.z);

  for (int row = 0; row < terrain->get_map_height(); ++row)
  {
    for (int col = 0; col < terrain->get_map_width(); ++col)
    {
      if (layer.get_value(row, col) > highestValue)
      {
        highestValue = layer.get_value(row, col);
        pos = terrain->get_world_position(row, col);
        pos2D = Vec2(pos.x, pos.z);
        targetFound = true;
      }
      else if (layer.get_value(row, col) == highestValue)
      {
        Vec2 vec1 = pos2D - enemyPos2D;
        Vec3 temp = terrain->get_world_position(row, col);
        Vec2 vec2 = Vec2(temp.x, temp.z) - enemyPos2D;
        if (vec2.Length() < vec1.Length())
        {
          pos2D = Vec2(temp.x, temp.z);
          pos = temp;
        }
      } 
    }
  }
  if (targetFound)
    enemy->path_to(pos);

  return targetFound;*/
}
