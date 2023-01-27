#include "pch.h"
#include "WorldGridResolver.h"
#include "Components/Components.h"
#include "Spatial/WorldGrid.h"


void drft::system::WorldGridResolver::init()
{
	registry.on_construct<component::Position>().connect<&WorldGridResolver::OnPositionAdd>(this);
	registry.on_destroy<component::Position>().connect<&WorldGridResolver::OnPositionRemove>(this);
}

void drft::system::WorldGridResolver::OnPositionAdd(entt::registry& registry, entt::entity entity)
{
	auto& pos = registry.get<component::Position>(entity);

	world.placeEntity(entity, {(int)pos.x, (int)pos.y}, (spatial::Layer)pos.depth);
}

void drft::system::WorldGridResolver::OnPositionRemove(entt::registry& registry, entt::entity entity)
{

	auto& pos = registry.get<component::Position>(entity);

	world.removeEntity(entity, {(int)pos.x, (int)pos.y}, (spatial::Layer)pos.depth);
}
