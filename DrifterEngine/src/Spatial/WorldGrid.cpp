#include "pch.h"
#include "WorldChunk.h"
#include "WorldGrid.h"


drft::spatial::WorldGrid::WorldGrid(sf::Vector2i chunkDimensions, sf::Vector2i tileDimensions)
	:	_chunkWidth(chunkDimensions.x), _chunkHeight(chunkDimensions.y),
		_tileWidth(tileDimensions.x), _tileHeight(tileDimensions.y) {}

void drft::spatial::WorldGrid::placeEntity(const entt::entity& entity, const sf::Vector2i worldPosition, Layer layer)
{
	int xChunk = worldPosition.x / _chunkWidth;
	int yChunk = worldPosition.y / _chunkHeight;
	int xPos = std::abs(worldPosition.x % _chunkWidth);
	int yPos = std::abs(worldPosition.y % _chunkHeight);

	if (!_chunks.contains({ xChunk, yChunk }))
	{
		_chunks[{ xChunk, yChunk }] = std::make_unique<WorldChunk>(_chunkWidth, _chunkHeight);
	}
	_chunks[{xChunk, yChunk}]->placeEntity(entity, { xPos, yPos }, (int)layer);
}

entt::entity drft::spatial::WorldGrid::removeEntity(const entt::entity& entity, const sf::Vector2i worldPosition, Layer layer)
{
	entt::entity result = entt::null;

	int xChunk = worldPosition.x / _chunkWidth;
	int yChunk = worldPosition.y / _chunkHeight;
	int xPos = std::abs(worldPosition.x % _chunkWidth);
	int yPos = std::abs(worldPosition.y % _chunkHeight);

	if (!_chunks.contains({ xChunk,yChunk })) return result;
	result = _chunks.at({ xChunk, yChunk })->removeEntity(entity, { xPos, yPos }, (int)layer);

	return result;
}

bool drft::spatial::WorldGrid::moveEntity(const entt::entity entity, const sf::Vector2i fromWorldPosition, const sf::Vector2i toWorldPosition, Layer layer)
{
	int from_xChunk = fromWorldPosition.x / _chunkWidth;
	int from_yChunk = fromWorldPosition.y / _chunkHeight;
	int from_xPos	= std::abs(fromWorldPosition.x % _chunkWidth);
	int from_yPos	= std::abs(fromWorldPosition.y % _chunkHeight);

	if (!_chunks.contains({ from_xChunk, from_yChunk })) return false;

	_chunks[{from_xChunk, from_yChunk}]->removeEntity(entity, { from_xPos, from_yPos }, (int)layer);
	this->placeEntity(entity, toWorldPosition, layer);

	return true;
}

std::vector<entt::entity> drft::spatial::WorldGrid::entitiesAt(sf::Vector2i worldPosition, Layer layer)
{
	std::vector<entt::entity> result;

	int xChunk = worldPosition.x / _chunkWidth;
	int yChunk = worldPosition.y / _chunkHeight;
	int xPos = std::abs(worldPosition.x % _chunkWidth);
	int yPos = std::abs(worldPosition.y % _chunkHeight);

	if (!_chunks.contains({ xChunk, yChunk })) return result;

	result = _chunks.at({ xChunk, yChunk })->entitiesAt({ xPos, yPos }, (int)layer);

	return result;
}


