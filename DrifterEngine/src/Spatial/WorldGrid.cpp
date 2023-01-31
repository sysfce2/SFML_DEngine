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
}

entt::entity drft::spatial::WorldGrid::removeEntity(const entt::entity& entity, const sf::Vector2i worldPosition, Layer layer)
{
	entt::entity result = entt::null;

	auto chunkCoordinate = toChunkCoordinate(worldPosition);
	auto localPosition = toLocalChunkSpace(worldPosition);
	auto keyablePair = std::make_pair(chunkCoordinate.x, chunkCoordinate.y);

	if (!_chunks.contains(keyablePair)) return result;
	result = _chunks.at(keyablePair)->removeEntity(entity, localPosition, (int)layer);
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

bool drft::spatial::WorldGrid::moveEntity(const entt::entity entity, const sf::Vector2i fromWorldPosition, const sf::Vector2i toWorldPosition, Layer layer)
{
	auto chunkCoordinate = toChunkCoordinate(fromWorldPosition);
	auto localPosition = toLocalChunkSpace(fromWorldPosition);
	auto keyablePair = std::make_pair(chunkCoordinate.x, chunkCoordinate.y);

	if (!_chunks.contains(keyablePair)) return false;

	_chunks[keyablePair]->removeEntity(entity, localPosition, (int)layer);
	this->placeEntity(entity, toWorldPosition, layer);

	return true;
}

std::vector<entt::entity> drft::spatial::WorldGrid::entitiesAt(sf::Vector2i worldPosition, Layer layer)
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

void drft::spatial::WorldGrid::clearChunk(sf::Vector2i coordinate)
{
	if (_chunks.contains({ coordinate.x, coordinate.y }))
	{
		_chunks.at({ coordinate.x, coordinate.y })->clear();
		_chunks.erase({ coordinate.x, coordinate.y });
	}
}

std::vector<entt::entity> drft::spatial::WorldGrid::getAllEntities(sf::Vector2i coordinate)
{
	if (_chunks.contains({ coordinate.x, coordinate.y }))
	{
		return _chunks.at({ coordinate.x, coordinate.y })->getAllEntities();
	}
	return std::vector<entt::entity>();
}


