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
	if (registry.any_of<component::Camera>(entity)) return; // Don't need to add the camera to the grid

	auto& pos = registry.get<component::Position>(entity);
	auto& grid = registry.ctx().get<spatial::WorldGrid&>();

	grid.placeEntity(entity, spatial::toTileSpace(pos.position), spatial::toLayer(pos.depth));
}

void drft::system::WorldGridResolver::OnPositionRemove(entt::registry& registry, entt::entity entity)
{
	if (registry.any_of<component::Camera>(entity)) return; // Don't need to remove camera from the grid - not there

	auto& pos = registry.get<component::Position>(entity);
	auto& grid = registry.ctx().get<spatial::WorldGrid&>();

	grid.removeEntity(entity, spatial::toTileSpace(pos.position), spatial::toLayer(pos.depth));
}
