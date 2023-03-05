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

std::vector<entt::entity> drft::spatial::Cell::at(int layer)
{
	std::vector<entt::entity> result;

	if (!_entities.contains(layer)) return result;
	std::copy(_entities[layer].begin(), _entities[layer].end(), std::back_inserter(result));

	return result;
}

bool drft::spatial::Cell::empty()
{
	if (_entities.empty()) return true;
	return false;
}
