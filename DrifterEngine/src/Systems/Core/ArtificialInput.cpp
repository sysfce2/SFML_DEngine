#include "pch.h"
#include "ArtificialInput.h"
#include "Components/Components.h"
#include "Components/Tags.h"
#include "Random/RandomNumberGenerator.h"
#include "Spatial/Helpers.h"
#include "Spatial/Conversions.h"
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
			moveToTarget({ *registry, entity }, ai.target);
		}
		else
		{
			randomMove({ *registry, entity });
		}
	}
}

entt::entity drft::system::ArtificialInput::findTarget(entt::handle entity) const
{
	if (!entity.all_of<component::Faction>())
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
		if (FactionSystem::resolveRelationship(faction.name, otherfaction.name) == Relationship::Enemy)
		{
			float distance = spatial::distance(spatial::toTileSpace(pos.position), spatial::toTileSpace(otherPos.position));
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
	int randx = rng::RandomNumberGenerator::intInRange(-1, 1);
	int randy = rng::RandomNumberGenerator::intInRange(-1, 1);

	if (randx == 0 and randy == 0)
	{
		entity.emplace<component::action::Wait>();
	}
	else
	{
		entity.emplace<component::action::Move>(sf::Vector2i(randx, randy));
	}
}

void drft::system::ArtificialInput::moveToTarget(entt::handle entity, entt::entity target) const
{
	auto targetPos = registry->get<component::Position>(target);
	auto myPos = entity.get<component::Position>();

	auto delta = spatial::toTileSpace(myPos.position - targetPos.position);
	int xMove = delta.x == 0 ? 0 : -(delta.x / abs(delta.x));
	int yMove = delta.y == 0 ? 0 : -(delta.y / abs(delta.y));

	entity.emplace<component::action::Move>(sf::Vector2i{ xMove, yMove });
}
