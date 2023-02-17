#include "pch.h"
#include "Action.h"

float drft::action::attempt(entt::handle entity, std::unique_ptr<drft::action::Action> action)
{
	auto nextAction = std::move(action);

	float actionCost = 0.0f;
	while (nextAction != nullptr)
	{
		actionCost = nextAction->getCost();
		nextAction = nextAction->execute(entity);
	}

	return actionCost;
}
