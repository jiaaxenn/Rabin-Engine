#pragma once
#include "BehaviorNode.h"

class D_TargetDist50 : public BaseNode<D_TargetDist50>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};