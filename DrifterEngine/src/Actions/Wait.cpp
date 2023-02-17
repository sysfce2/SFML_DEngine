#include "pch.h"
#include "Wait.h"

using namespace drft::action;

std::unique_ptr<Action> drft::action::Wait::execute(entt::handle& entity)
{
    return nullptr;
}

float drft::action::Wait::getCost()
{
    return 100.0f;
}

std::unique_ptr<Action> drft::action::Wait::clone() const
{
    return std::make_unique<Wait>();
}
