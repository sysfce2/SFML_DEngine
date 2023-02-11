#include "pch.h"
#include "ActionHandler.h"
#include "Components/Components.h"
#include "Events/Events.h"
#include "Actions/Move.h"

void drft::system::ActionHandler::init()
{
	
}

void drft::system::ActionHandler::update(const float dt)
{
	auto view = registry->view<component::Input, component::Actor, component::MyTurn>(entt::exclude<component::Prototype>);
	action::Type actionType = action::Type::ACT;
	std::vector<entt::entity> inputsToDelete;

	for (auto [entity, input, actor] : view.each())
	{
		auto& nextAction = input.desiredAction;
		
		float actionCost = 100.f;
		while (nextAction != nullptr)
		{
			actionCost = nextAction->getCost();
			actionType = nextAction->getType();
			nextAction = nextAction->execute(*registry, entity);
		}
		
		switch (actionType)
		{
		case action::Type::MOVE:
			actionCost *= (1.f/actor.moveSpeed);
			break;
		case action::Type::ACT:
			actionCost *= (1.f/actor.actSpeed);
			break;
		default:
			actionCost *= (1.f / actor.actSpeed);
			break;
		}
		inputsToDelete.push_back(entity);
		registry->ctx().get<entt::dispatcher&>().trigger(events::SpendActionPoints{ (int)(actionCost) });
	}
	for (auto input : inputsToDelete)
	{
		registry->remove<component::Input>(input);
	}
}

