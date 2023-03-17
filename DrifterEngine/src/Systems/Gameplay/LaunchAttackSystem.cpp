#include "pch.h"
#include "LaunchAttackSystem.h"
#include "Spatial/WorldGrid.h"
#include "Spatial/Conversions.h"
#include "Components/Components.h"
#include "Components/Tags.h"
#include "Utility/EntityHelpers.h"

void drft::system::LaunchAttackSystem::init()
{
}

void drft::system::LaunchAttackSystem::update(const float dt)
{
	auto view = registry->view<component::action::LaunchAttack, component::tag::Active>();
	for (auto [entity, attack] : view.each())
	{
		const auto& grid = registry->ctx().get<spatial::WorldGrid&>();
		auto& posComp = registry->get<component::Position>(entity);
		sf::Vector2i targetPosition = spatial::toTileSpace(posComp.position) + attack.direction;

		// TODO: check all depths for entities
		const auto targets = grid.entitiesAt(targetPosition, posComp.depth);
		for (auto target : targets)
		{
			registry->emplace_or_replace<component::action::TakeDamage>(target, attack.damage);
		}
		
		int actionCost = util::getActionCost({ *registry, entity }, 100, util::ActionType::Act);
		registry->emplace_or_replace<component::action::SpendPoints>(entity, actionCost);
		registry->remove<component::action::LaunchAttack>(entity);
	}
}
