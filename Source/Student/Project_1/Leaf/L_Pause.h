#pragma once
#include "BehaviorNode.h"

class L_Pause : public BaseNode<L_Pause>
{
protected:
  virtual void on_update(float dt);
};