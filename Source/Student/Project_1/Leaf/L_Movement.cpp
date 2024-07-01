#include <pch.h>
#include "L_Movement.h"
#include "../Agent/BehaviorAgent.h"

void L_Movement::on_enter()
{
  const auto& bb = agent->get_blackboard();
  targetPoint = bb.get_value<Vec3>("Moved Position");

  BehaviorNode::on_leaf_enter();
}

void L_Movement::on_update(float dt)
{
  const auto result = agent->move_toward_point(targetPoint, dt);

  if (result == true)
  {
    on_success();
  }

  display_leaf_text();
}