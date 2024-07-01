#include <pch.h>
#include "L_CheckWKeyClick.h"
#include "../Agent/BehaviorAgent.h"

void L_CheckWKeyClick::on_update(float dt)
{
  const auto wKeyState = InputHandler::get_current_state(KBKeys::W);

  if (wKeyState == InputHandler::InputState::PRESSED)
  {
    const auto& currPos = agent->get_position();
    const Vec3 newPos(currPos.x + 10, currPos.y, currPos.z);

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