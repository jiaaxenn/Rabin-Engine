#pragma once
#include "BehaviorNode.h"

class D_TargetDist5 : public BaseNode<D_TargetDist5>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};