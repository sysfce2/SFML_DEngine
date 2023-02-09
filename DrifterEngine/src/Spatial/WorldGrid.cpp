#include "pch.h"
#include "WorldChunk.h"
#include "WorldGrid.h"

void drft::spatial::WorldGrid::placeEntity(const entt::entity& entity, const sf::Vector2i worldPosition, Layer layer)
{
	auto chunkCoordinate = toChunkCoordinate(worldPosition);
	auto localPosition = toLocalChunkSpace(worldPosition);
	auto keyablePair = std::make_pair(chunkCoordinate.x, chunkCoordinate.y);

	if (!_chunks.contains(keyablePair))
	{
		_chunks[keyablePair] = std::make_unique<WorldChunk>(CHUNK_WIDTH, CHUNK_HEIGHT);
		_activeChunks.push_back(chunkCoordinate);
	}
	_chunks[keyablePair]->placeEntity(entity, localPosition, (int)layer);
	_entityPositions[entity] = { worldPosition.x, worldPosition.y, (int)layer };
}

entt::entity drft::spatial::WorldGrid::removeEntity(const entt::entity& entity)
{
	if (entity == entt::null) return entity;

	entt::entity result = entt::null;

	sf::Vector2i worldPosition;
	worldPosition.x = _entityPositions[entity].x;
	worldPosition.y = _entityPositions[entity].y;
	int layer = _entityPositions[entity].z;

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
	if (_chunks.at(keyablePair)->empty())
	{
		auto it = std::find(_activeChunks.begin(), _activeChunks.end(), chunkCoordinate);
		if (it != _activeChunks.end())
		{
			_activeChunks.erase(it);
		}
		_chunks.at(keyablePair).release();
		_chunks.erase(keyablePair);
	}

	return result;
}

bool drft::spatial::WorldGrid::moveEntity(const entt::entity entity, const sf::Vector2i toWorldPosition, Layer layer)
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

const drft::spatial::Layer drft::spatial::WorldGrid::getLayer(const entt::entity entity) const
{
	auto vec3 = _entityPositions.at(entity);
	return (Layer)vec3.z;
}

std::vector<entt::entity> drft::spatial::WorldGrid::entitiesAt(const sf::Vector2i worldPosition, const Layer layer)
{
	std::vector<entt::entity> result;

	auto chunkCoordinate = toChunkCoordinate(worldPosition);
	auto localPosition = toLocalChunkSpace(worldPosition);
	auto keyablePair = std::make_pair(chunkCoordinate.x, chunkCoordinate.y);

	if (!_chunks.contains(keyablePair)) return result;

	result = _chunks.at(keyablePair)->entitiesAt(localPosition, (int)layer);

	return result;
}

const std::vector<sf::Vector2i>& drft::spatial::WorldGrid::getActiveChunks()
{
	return _activeChunks;
}

void drft::spatial::WorldGrid::removeChunk(const sf::Vector2i coordinate)
{
	if (_chunks.contains({ coordinate.x, coordinate.y }))
	{
		_chunks.at({ coordinate.x, coordinate.y })->clear();
		_chunks.erase({ coordinate.x, coordinate.y });
		auto it = std::find(_activeChunks.begin(), _activeChunks.end(), coordinate);
		if (it != _activeChunks.end())
		{
			_activeChunks.erase(it);
		}
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


