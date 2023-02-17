#include "pch.h"
#include "Move.h"
#include "Spatial/WorldGrid.h"
#include "Spatial/Conversions.h"
#include "Components/Components.h"
#include "Actions/Attack.h"
#include "Actions/Wait.h"

using namespace drft::action;

std::unique_ptr<Action> drft::action::Move::execute(entt::handle& entity)
{
	if (direction == sf::Vector2i(0, 0))
	{
		return std::make_unique<Wait>();
	}
	auto& pos = entity.get<component::Position>();
	auto& grid = entity.registry()->ctx().get<spatial::WorldGrid&>();

	sf::Vector2f targetPosition = pos.position;
	targetPosition += spatial::toWorldSpace(direction);

	// TODO: add more robust collision checking to account for objects on different layers

	auto potentialBlockers = grid.entitiesAt(spatial::toTileSpace(targetPosition), pos.depth);

	if (potentialBlockers.empty() || spatial::toLayer(pos.depth) == spatial::Layer::Item)
	{
		entity.patch<component::Position>(
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
		return std::make_unique<Attack>(potentialBlockers);
	}
	
	return nullptr;
}

std::unique_ptr<Action> drft::action::Move::clone() const
{
	return std::make_unique<Move>(direction);
}
