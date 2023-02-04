#include "pch.h"
#include "ActionHandler.h"
#include "Components/Components.h"


void drft::system::ActionHandler::init()
{
	
}

void drft::system::ActionHandler::update(const float dt)
{
	auto view = registry->view<component::Input>();

	for (auto [entity, input] : view.each())
	{
		_actionQueue.push({ entity, std::move(input.desiredAction) });
		registry->remove<component::Input>(entity);
	}

	while (!_actionQueue.empty())
	{
		entt::entity entity = _actionQueue.front().entity;
		auto nextAction = _actionQueue.front().action->execute(*registry, entity);
		_actionQueue.pop();
		if (nextAction)
		{
			_actionQueue.push({ entity, std::move(nextAction) });
		}
	}
}

