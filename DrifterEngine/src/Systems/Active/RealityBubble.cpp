#include "pch.h"
#include "RealityBubble.h"
#include "Components/Components.h"
#include "Components/Tags.h"
#include "Spatial/Conversions.h"
#include "Spatial/Helpers.h"
#include "Utility/DebugInfo.h"

void drft::system::RealityBubble::init()
{
	registry->on_construct<component::Actor>().connect<&RealityBubble::onActorAddOrUpdate>(this);
	registry->on_update<component::Actor>().connect<&RealityBubble::onActorAddOrUpdate>(this);
	registry->on_destroy<component::Actor>().connect<&RealityBubble::onActorRemove>(this);

	registry->on_construct<component::tag::Active>().connect<&RealityBubble::onActiveAdd>(this);
	registry->on_destroy<component::tag::Active>().connect<&RealityBubble::onActiveRemove>(this);
}

void drft::system::RealityBubble::update(const float)
{
	auto cameraView = registry->view<component::Camera, component::Position>(entt::exclude<component::Prototype>);
	for (auto&& [entity, camera, position] : cameraView.each())
	{
		_cameraPosition = spatial::toTileSpace(position.position);
	}

	auto actorView = registry->view<component::Actor, component::Position>(entt::exclude<component::Prototype>);
	for (auto&& [entity, actor, pos] : actorView.each())
	{
		auto actorPosition = spatial::toTileSpace(pos.position);
		auto distance = spatial::distance(_cameraPosition, actorPosition);

		if (distance > REALITY_RADIUS)
		{
			registry->remove<component::tag::Active>(entity);
		}
		else
		{
			if (registry->any_of<component::tag::Active>(entity)) continue;
			registry->emplace<component::tag::Active>(entity);
		}
	}

	auto& debug = registry->ctx().get<util::DebugInfo>();
	std::string dataStr = std::to_string(_activeActors);
	debug.addString("Actors Active", dataStr);

}

void drft::system::RealityBubble::onActorAddOrUpdate(entt::registry& registry, entt::entity entity)
{
	if (registry.any_of<component::Prototype>(entity)) return;
	if (!registry.any_of<component::Position>(entity)) return;

	auto pos = spatial::toChunkCoordinate(registry.get<component::Position>(entity).position);
	auto distance = spatial::distance(_cameraPosition, pos);

	if (distance > REALITY_RADIUS)
	{
		registry.remove<component::tag::Active>(entity);
	}
	else
	{
		if (registry.any_of<component::tag::Active>(entity)) return;
		registry.emplace<component::tag::Active>(entity);
	}
}

void drft::system::RealityBubble::onActorRemove(entt::registry& registry, entt::entity entity)
{
	if (registry.any_of<component::Prototype>(entity)) return;
	if (!registry.any_of<component::Position, component::tag::Active>(entity)) return;

	registry.remove<component::tag::Active>(entity);
}

void drft::system::RealityBubble::onActiveAdd(entt::registry& registry, entt::entity entity)
{
	++_activeActors;
	if (registry.any_of<component::Prototype>(entity)
		|| !registry.all_of<component::Position, component::Actor>(entity))
	{
		registry.remove<component::tag::Active>(entity);
	}
	auto pos = spatial::toChunkCoordinate(registry.get<component::Position>(entity).position);
	auto distance = spatial::distance(_cameraPosition, pos);

	if (distance > REALITY_RADIUS)
	{
		registry.remove<component::tag::Active>(entity);
	}
}

void drft::system::RealityBubble::onActiveRemove(entt::registry& registry, entt::entity entity)
{
	--_activeActors;
}
