#include <pch.h>
#include "L_CheckPKeyClick.h"
#include "../Agent/BehaviorAgent.h"

void L_CheckPKeyClick::on_update(float dt)
{
  const auto pKeyState = InputHandler::get_current_state(KBKeys::P);
  auto& bb = agent->get_blackboard();

  if (pKeyState == InputHandler::InputState::PRESSED)
  {
    bb.set_value("Paused", true);
    on_success();
  }
  else
  {
    on_failure();
  }

  display_leaf_text();
}