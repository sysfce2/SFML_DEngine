#include "pch.h"
#include "ActionHandler.h"
#include "Actions/Actions.h"
#include "Components/Components.h"


void drft::system::ActionHandler::init()
{
	
}

void drft::system::ActionHandler::update(const float dt)
{
	auto view = registry.view<component::Input>();

	for (auto [entity, input] : view.each())
	{
		action::ActionResult action = { (ActionType)input.desiredAction, {0,0} };
		while (action.type != ActionType::None)
		{
			// Returns a 'back-up' action
			action = performAction(entity, action);
		}

		// Input handled
		registry.remove<component::Input>(entity);
	}
}

drft::action::ActionResult drft::system::ActionHandler::performAction(entt::entity entity, action::ActionResult action)
{
	auto& grid = registry.ctx().get<spatial::WorldGrid&>();
	action::ActionResult result;
	switch (action.type)
	{
	case ActionType::MoveEast:
		result = action::move(registry, entity, { 1,0 }, grid);
		break;
	case ActionType::MoveNorth:
		result = action::move(registry, entity, { 0,-1 }, grid);
		break;
	case ActionType::MoveNorthEast:
		result = action::move(registry, entity, { 1,-1 }, grid);
		break;
	case ActionType::MoveNorthWest:
		result = action::move(registry, entity, { -1,-1 }, grid);
		break;
	case ActionType::MoveSouth:
		result = action::move(registry, entity, { 0,1 }, grid);
		break;
	case ActionType::MoveSouthEast:
		result = action::move(registry, entity, { 1,1 }, grid);
		break;
	case ActionType::MoveSouthWest:
		result = action::move(registry, entity, { -1,1 }, grid);
		break;
	case ActionType::MoveWest:
		result = action::move(registry, entity, { -1,0 }, grid);
		break;
	case ActionType::Attack:
		result = action::attack(registry, entity, action.target, grid);
		break;
	default:
		std::cout << "Action type: " << (int)action.type << " is not yet supported!" << std::endl;
		result = action::ActionResult();
		break;
	}
	return result;
}
