#include <pch.h>
#include "D_TargetDist5.h"

void D_TargetDist5::on_enter()
{
  Vec3 targetDist;
  bool targetWithinDist = false;

  // get a list of all current agents
  const auto& allAgents = agents->get_all_agents();

  // and our agent's position
  const auto& currPos = agent->get_position();

  for (const auto& a : allAgents)
  {
    // make sure it's not our agent
    if (a != agent)
    {
      const auto& agentPos = a->get_position();
      const float distance = Vec3::Distance(currPos, agentPos);

      if (distance <= 5.0f)
      {
        targetWithinDist = true;
        std::cout << "Within dist\n";
        std::cout << "____________________\n";
      }
    }
  }

  if (targetWithinDist == true)
  {
    BehaviorNode::on_enter();
  }
  else // couldn't find a viable agent
  {
    on_failure();
  }
}

void D_TargetDist5::on_update(float dt)
{
  BehaviorNode* child = children.front();

  child->tick(dt);

  // assume same status as child
  set_status(child->get_status());
  set_result(child->get_result());
}
