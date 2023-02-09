#include "pch.h"
#include "WorldGridResolver.h"
#include "Components/Components.h"
#include "Spatial/WorldGrid.h"
#include "Events/Events.h"


void drft::system::WorldGridResolver::init()
{
	auto& dispatcher = registry->ctx().get<entt::dispatcher>();

	registry->on_construct<component::Position>().connect<&WorldGridResolver::OnPositionAdd>(this);
	registry->on_update<component::Position>().connect<&WorldGridResolver::OnPositionUpdate>(this);
	registry->on_destroy<component::Position>().connect<&WorldGridResolver::OnPositionRemove>(this);
}

void drft::system::WorldGridResolver::OnPositionAdd(entt::registry& registry, entt::entity entity)
{
	if (registry.any_of<component::Camera>(entity)) return; // Don't need to add the camera to the grid
	if (registry.any_of<component::Prototype>(entity)) return;

	auto& pos = registry.get<component::Position>(entity);
	auto& grid = registry.ctx().get<spatial::WorldGrid&>();

	grid.placeEntity(entity, spatial::toTileSpace(pos.position), spatial::toLayer(pos.depth));
}

void drft::system::WorldGridResolver::OnPositionUpdate(entt::registry& registry, entt::entity entity)
{
	if (registry.any_of<component::Camera>(entity)) return; // Don't need to remove camera from the grid - not there
	if (registry.any_of<component::Prototype>(entity)) return;

	auto& pos = registry.get<component::Position>(entity);
	auto& grid = registry.ctx().get<spatial::WorldGrid&>();

	grid.moveEntity(entity, spatial::toTileSpace(pos.position), spatial::toLayer(pos.depth));
}

void drft::system::WorldGridResolver::OnPositionRemove(entt::registry& registry, entt::entity entity)
{
	if (registry.any_of<component::Camera>(entity)) return; // Don't need to remove camera from the grid - not there
	if (registry.any_of<component::Prototype>(entity)) return;

	auto& pos = registry.get<component::Position>(entity);
	auto& grid = registry.ctx().get<spatial::WorldGrid&>();

	grid.removeEntity(entity);
}
