#include "pch.h"
#include "Cell.h"

void drft::spatial::Cell::placeEntity(entt::entity entity, int layer)
{
	_entities[layer].insert(entity);
}

entt::entity drft::spatial::Cell::removeEntity(entt::entity entity, int layer)
{
	entt::entity result = entt::null;

	if (!_entities.contains(layer)) return result;
	auto &s = _entities[layer];
	if (!s.contains(entity)) return result;

	result = *( s.find(entity) );

	s.erase(result);
	if (s.empty()) _entities.erase(layer);

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
