#include "pch.h"
#include "ActionHandler.h"
#include "Components/Components.h"
#include "Events/Events.h"


void drft::system::ActionHandler::init()
{
	
}

void drft::system::ActionHandler::update(const float dt)
{
	auto view = registry->view<component::Input, component::MyTurn>(entt::exclude<component::Prototype>);

	for (auto [entity, input] : view.each())
	{
		auto& nextAction = input.desiredAction;
		
		while (nextAction != nullptr)
		{
			nextAction = nextAction->execute(*registry, entity);
		}

		registry->remove<component::Input>(entity);
		registry->ctx().get<entt::dispatcher&>().trigger(events::SpendActionPoints{ 100 });
	}
}

