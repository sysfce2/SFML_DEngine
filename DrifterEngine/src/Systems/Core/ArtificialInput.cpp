#include "pch.h"
#include "ArtificialInput.h"
#include "Components/Components.h"
#include "Components/Tags.h"
#include "Random/RandomNumberGenerator.h"
#include "Spatial/Helpers.h"
#include "Systems/Gameplay/FactionSystem.h"


void drft::system::ArtificialInput::init()
{
}

void drft::system::ArtificialInput::update(const float dt)
{
	auto view = registry->view<component::AI, const component::Position, component::tag::CurrentActor>();
	for (auto [entity, ai, myPos] : view.each())
	{

		ai.target = findTarget({ *registry, entity });
		if (ai.target != entt::null && ai.goals.contains("kill_target"))
		{
			
		}
		else
		{
			randomMove({ *registry, entity });
		}
	}
}

entt::entity drft::system::ArtificialInput::findTarget(entt::handle entity) const
{
	if (!entity.all_of<component::AI, component::Faction, component::Position>())
	{
		return entt::null;
	}

	auto faction = entity.get<component::Faction>();
	auto ai = entity.get<component::AI>();
	auto pos = entity.get<component::Position>();

	float closestRange = 1000;
	entt::entity closestTarget = entt::null;

	auto factionView = registry->view<const component::Faction, const component::Position, component::tag::Active>();
	for (auto [otherEnt, otherfaction, otherPos] : factionView.each())
	{
		if (FactionSystem::resolveRelationship(faction.name, faction.name) == Relationship::Enemy)
		{
			float distance = spatial::distance(pos.position, otherPos.position);
			if (distance < ai.sightRange && distance < closestRange)
			{
				closestRange = distance;
				closestTarget = otherEnt;
			}
		}
	}
	if (closestTarget != entt::null)
	{
		return closestTarget;
	}
	return entt::null;
}

void drft::system::ArtificialInput::randomMove(entt::handle entity) const
{
	int randx = rng::RandomNumberGenerator::generateInRange(-1l, 1l);
	int randy = rng::RandomNumberGenerator::generateInRange(-1l, 1l);

	if (randx == 0 and randy == 0)
	{
		entity.emplace<component::action::Wait>(sf::Vector2i(randx, randy));
	}
	else
	{
		entity.emplace<component::action::Move>(sf::Vector2i(randx, randy));
	}
}
