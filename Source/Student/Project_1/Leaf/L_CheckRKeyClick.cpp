#include <pch.h>
#include "L_CheckRKeyClick.h"
#include "../Agent/BehaviorAgent.h"

void L_CheckRKeyClick::on_update(float dt)
{
  const auto rKeyState = InputHandler::get_current_state(KBKeys::R);
  auto& bb = agent->get_blackboard();

  if (rKeyState == InputHandler::InputState::PRESSED)
  {
    bb.set_value("Paused", false);
    on_success();
  }
  else
  {
    on_failure();
  }

  display_leaf_text();
}