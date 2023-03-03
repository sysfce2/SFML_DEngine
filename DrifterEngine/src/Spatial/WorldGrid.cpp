#include "pch.h"
#include "WorldChunk.h"
#include "WorldGrid.h"
#include "Spatial/Conversions.h"

void drft::spatial::WorldGrid::placeEntity(const entt::entity& entity, const sf::Vector2i worldPosition, int layer)
{
	auto chunkCoordinate = toChunkCoordinate(worldPosition);
	auto localPosition = toLocalChunkSpace(worldPosition);
	auto keyablePair = std::make_pair(chunkCoordinate.x, chunkCoordinate.y);

	if (!_chunks.contains(keyablePair))
	{
		_chunks[keyablePair] = std::make_unique<WorldChunk>(CHUNK_WIDTH, CHUNK_HEIGHT);
	}
	_chunks[keyablePair]->placeEntity(entity, localPosition, layer);
	_entityPositions[entity] = { worldPosition.x, worldPosition.y, layer };
}

entt::entity drft::spatial::WorldGrid::removeEntity(const entt::entity& entity)
{
	if (entity == entt::null) return entity;

	entt::entity result = entt::null;

	sf::Vector2i worldPosition = getPosition(entity);
	int layer = static_cast<int>(getLayer(entity));

	auto chunkCoordinate = toChunkCoordinate(worldPosition);
	auto localPosition = toLocalChunkSpace(worldPosition);
	auto keyablePair = std::make_pair(chunkCoordinate.x, chunkCoordinate.y);

	if (!_chunks.contains(keyablePair))
	{
		assert(false);
		return result;
	}
	result = _chunks.at(keyablePair)->removeEntity(entity, localPosition, layer);
	_entityPositions.erase(entity);

	return result;
}

bool drft::spatial::WorldGrid::moveEntity(const entt::entity entity, const sf::Vector2i toWorldPosition, int layer)
{
	this->removeEntity(entity);
	this->placeEntity(entity, toWorldPosition, layer);

	return true;
}

const sf::Vector2i drft::spatial::WorldGrid::getPosition(const entt::entity entity) const
{
	auto vec3 = _entityPositions.at(entity);
	return { vec3.x, vec3.y };
}

const int drft::spatial::WorldGrid::getLayer(const entt::entity entity) const
{
	auto vec3 = _entityPositions.at(entity);
	return vec3.z;
}

std::vector<entt::entity> drft::spatial::WorldGrid::entitiesAt(const sf::Vector2i worldPosition, const int layer)
{
	std::vector<entt::entity> result;

	auto chunkCoordinate = toChunkCoordinate(worldPosition);
	auto localPosition = toLocalChunkSpace(worldPosition);
	auto keyablePair = std::make_pair(chunkCoordinate.x, chunkCoordinate.y);

	if (!_chunks.contains(keyablePair)) return result;

	result = _chunks.at(keyablePair)->entitiesAt(localPosition, layer);

	return result;
}

void drft::spatial::WorldGrid::removeChunk(const sf::Vector2i coordinate)
{
	if (_chunks.contains({ coordinate.x, coordinate.y }))
	{
		//_chunks.at({ coordinate.x, coordinate.y })->clear();
		_chunks.erase( {coordinate.x, coordinate.y} );
	}
}

std::vector<entt::entity> drft::spatial::WorldGrid::getAllEntities(const sf::Vector2i coordinate)
{
	if (_chunks.contains({ coordinate.x, coordinate.y }))
	{
		return _chunks.at({ coordinate.x, coordinate.y })->getAllEntities();
	}
	return std::vector<entt::entity>();
}
