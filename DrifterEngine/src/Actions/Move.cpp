#include "pch.h"
#include "Move.h"
#include "Spatial/WorldGrid.h"
#include "Components/Components.h"
#include "Actions/Attack.h"

using namespace drft::action;

std::unique_ptr<Action> drft::action::Move::execute(entt::registry& registry, const entt::entity actor)
{
	auto& pos = registry.get<component::Position>(actor);
	auto& grid = registry.ctx().get<spatial::WorldGrid&>();

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

		return nullptr;
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

			return nullptr;
		}
	}


	return std::make_unique<action::Attack>(direction);
}

std::unique_ptr<Action> drft::action::Move::clone() const
{
	return std::make_unique<Move>(direction);
}
