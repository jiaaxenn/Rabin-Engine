#include <pch.h>
#include "L_CheckAKeyClick.h"
#include "../Agent/BehaviorAgent.h"

void L_CheckAKeyClick::on_update(float dt)
{
  const auto aKeyState = InputHandler::get_current_state(KBKeys::A);

  if (aKeyState == InputHandler::InputState::PRESSED)
  {
    const auto& currPos = agent->get_position();
    const Vec3 newPos(currPos.x, currPos.y, currPos.z - 10);

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