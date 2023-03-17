#include "pch.h"
#include "Cell.h"

void drft::spatial::Cell::placeEntity(entt::entity entity, int layer)
{
	_entities[layer].push_back(entity);
}

entt::entity drft::spatial::Cell::removeEntity(entt::entity entity, int layer)
{
	entt::entity result = entt::null;

	if (!_entities.contains(layer)) return result;
	auto &entities = _entities[layer];
	
	auto found = std::find(entities.begin(), entities.end(), entity);
	if (found == entities.end()) return result;
	result = *found;
	entities.erase(found);
	if (entities.empty()) _entities.erase(layer);

	return result;
}

const std::vector<entt::entity>& drft::spatial::Cell::at(int layer) const
{
	std::vector<entt::entity> result;

	if (!_entities.contains(layer)) return result;

	return _entities.at(layer);
}

bool drft::spatial::Cell::empty()
{
	return _entities.empty();
}
