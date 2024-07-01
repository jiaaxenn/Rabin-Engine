#pragma once
#include "BehaviorNode.h"

class D_Pause : public BaseNode<D_Pause>
{
protected:
  virtual void on_update(float dt);
};