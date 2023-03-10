#include <pch.h>
#include "MovementSystem.h"
#include "Spatial/WorldGrid.h"
#include "Spatial/Conversions.h"
#include "Components/Components.h"
#include "Components/Tags.h"

void drft::system::MovementSystem::init()
{
}

void drft::system::MovementSystem::update(const float dt)
{
	auto moveView = registry->view<
		component::action::Move,
		component::tag::Active>();

	for (auto [entity, move] : moveView.each())
	{
		if (!registry->all_of<component::Position>(entity)) {
			registry->remove<
				component::action::Move,
				component::tag::Active>(entity);
			continue;
		}

		auto& grid = registry->ctx().get<spatial::WorldGrid&>();
		auto& posComp = registry->get<component::Position>(entity);

		registry->patch<component::Position>(entity,
			[&move](component::Position& pos)
			{
				pos.position.x += spatial::toWorldSpace(move.direction).x;
				pos.position.y += spatial::toWorldSpace(move.direction).y;
			}
		);

		registry->emplace_or_replace<component::SpendActionPoints>(entity, 100);
		registry->remove<component::action::Move>(entity);
	}
}
