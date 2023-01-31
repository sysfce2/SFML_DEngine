#include "pch.h"
#include "Actions.h"
#include "Components/Components.h"

using namespace drft::action;

ActionResult drft::action::move(entt::registry& registry, entt::entity actor, sf::Vector2i direction, spatial::WorldGrid& grid)
{
	auto& pos = registry.get<component::Position>(actor);
	sf::Vector2f targetPosition = pos.position;
	targetPosition += spatial::toWorldSpace(direction);

	if (spatial::toLayer(pos.depth) == spatial::Layer::Item)
	{
		grid.moveEntity(actor, spatial::toTileSpace(pos.position), spatial::toTileSpace(targetPosition), spatial::Layer::Item);
		registry.patch<component::Position>(actor, 
			[&](auto& p) 
			{
				p.position = targetPosition;
				p.depth = pos.depth;
			}
		);

		return { ActionType::None, spatial::toTileSpace(targetPosition)};
	}
	else
	{
		auto potentialBlockers = grid.entitiesAt(spatial::toTileSpace(targetPosition), spatial::toLayer(pos.depth));
		if (potentialBlockers.empty())
		{
			grid.moveEntity(actor, spatial::toTileSpace(pos.position), spatial::toTileSpace(targetPosition), spatial::toLayer(pos.depth));
			registry.patch<component::Position>(actor,
				[&](auto& p)
				{
					p.position = targetPosition;
					p.depth = pos.depth;
				}
			);

			return { ActionType::None, spatial::toTileSpace(targetPosition) };
		}
	}

	return { ActionType::Attack, spatial::toTileSpace(targetPosition) };
}

ActionResult drft::action::attack(entt::registry& registry, entt::entity actor, sf::Vector2i targetPosition, spatial::WorldGrid& grid)
{
	auto targets = grid.entitiesAt(targetPosition, spatial::Layer::Actor);
	for (auto&& target : targets)
	{
		std::cout << "Target id: " << (int)target << std::endl;
	}
	std::cout << "Attack (" << targetPosition.x << ", " << targetPosition.y << ")" << std::endl;

	return { ActionType::Swap, targetPosition };
}
