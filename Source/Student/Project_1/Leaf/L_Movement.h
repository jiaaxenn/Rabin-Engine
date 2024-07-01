#pragma once
#include "BehaviorNode.h"

class L_Movement : public BaseNode<L_Movement>
{
protected:
  virtual void on_enter() override;
  virtual void on_update(float dt) override;

private:
  Vec3 targetPoint;
};