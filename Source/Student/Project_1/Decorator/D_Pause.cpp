#include <pch.h>
#include "D_Pause.h"

void D_Pause::on_update(float dt)
{
  auto& bb = agent->get_blackboard();
  const auto isPaused = bb.get_value<bool>("Paused");
  if (isPaused == true)
  {
    BehaviorNode* child = children.front();

    child->tick(dt);

    // assume same status as child
    set_status(child->get_status());
    set_result(child->get_result());
  }
}
