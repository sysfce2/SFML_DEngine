#pragma once
#include "ActionTypes.h"
#include "Spatial/WorldGrid.h"

namespace drft::action
{
	struct ActionResult
	{
		ActionType type = ActionType::None;
		sf::Vector2i target = { 0,0 };
	};

	ActionResult move(entt::registry&, entt::entity, sf::Vector2i direction, spatial::WorldGrid& grid);
	ActionResult attack(entt::registry&, entt::entity, sf::Vector2i targetPosition, spatial::WorldGrid& grid);


}