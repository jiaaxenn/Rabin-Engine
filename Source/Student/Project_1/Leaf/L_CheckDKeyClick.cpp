#include <pch.h>
#include "L_CheckDKeyClick.h"
#include "../Agent/BehaviorAgent.h"

void L_CheckDKeyClick::on_update(float dt)
{
  const auto dKeyState = InputHandler::get_current_state(KBKeys::D);

  if (dKeyState == InputHandler::InputState::PRESSED)
  {
    const auto& currPos = agent->get_position();
    const Vec3 newPos(currPos.x, currPos.y, currPos.z + 10);

    auto& bb = agent->get_blackboard();
    bb.set_value("Moved Position", newPos);

    on_success();
  }
  else
  {
    on_failure();
  }

  display_leaf_text();
}