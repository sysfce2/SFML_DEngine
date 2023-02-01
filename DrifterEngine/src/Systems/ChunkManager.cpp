#include "pch.h"
#include "ChunkManager.h"
#include "Spatial/WorldGrid.h"
#include "Components/Components.h"
#include "Utility/TestEntities.h"

using namespace drft::system;

void drft::system::ChunkManager::init()
{

}

void drft::system::ChunkManager::update(const float dt)
{
	auto cameraView = registry.view<const component::Camera, const component::Position>();
	sf::Vector2i newPosition = { 0,0 };

	for (auto [entity, cam, pos] : cameraView.each())
	{
		newPosition = spatial::toChunkCoordinate(pos.position);
	}



	if (newPosition != _currentPosition)
	{
		auto newChunks = getChunksInRadius(newPosition, _activeChunkRadius);
		auto& activeChunks = _grid.get().getActiveChunks();

		// get chunks in newChunks that aren't in activeChunks
		auto diff = compareChunkList(activeChunks, newChunks);

		for (auto& chunk : diff)
		{
			if (!_chunkStates.contains({ chunk.x, chunk.y }))
			{
				_toBuild.push(chunk);
				continue;
			}
			auto state = _chunkStates[{chunk.x, chunk.y}];
			switch (state)
			{
			case ChunkState::Built:
				_chunkStates[{chunk.x, chunk.y}] = ChunkState::Active;
				break;
			case ChunkState::Saved:
				_toLoad.push(chunk);
				break;
			case ChunkState::Loaded:
				_chunkStates[{chunk.x, chunk.y}] = ChunkState::Active;
				break;
			default:
				std::cout << "ChunkState " << (int)state << " can not be handled yet." << std::endl;
				break;
			}

		}


		for (auto& chunk : activeChunks)
		{
			std::cout << "Chunk (" << chunk.x << ", " << chunk.y << ")" << " is active" << std::endl;
			float distance = std::hypotf((chunk.x - newPosition.x), (chunk.y - newPosition.y));
			if (distance > _toSaveRadius)
			{
				std::cout << "----> To save (" << chunk.x << ", " << chunk.y << ")" << std::endl;
				_toSave.push(chunk);
			}
		}
		std::cout << "\n";

		_currentPosition = newPosition;
	}


	if (!_toSave.empty())
	{
		save(_toSave.front());
	}
	if (!_toBuild.empty())
	{
		build(_toBuild.front());
	}
	if (!_toLoad.empty())
	{
		build(_toLoad.front());
	}
}

std::vector<sf::Vector2i> drft::system::ChunkManager::getChunksInRadius(const sf::Vector2i centerPosition, const float radius)
{
	std::vector<sf::Vector2i> result;
	for (int y = centerPosition.y - _activeChunkRadius; y <= centerPosition.y + _activeChunkRadius; ++y)
	{
		for (int x = centerPosition.x - _activeChunkRadius; x <= centerPosition.x + _activeChunkRadius; ++x)
		{
			float distance = std::hypotf((centerPosition.x - x), (centerPosition.y - y));
			if (distance <= radius)
			{
				result.push_back({ x,y });
			}
		}
	}

	return result;
}

std::vector<sf::Vector2i> drft::system::ChunkManager::compareChunkList(const std::vector<sf::Vector2i>& inVec, const std::vector<sf::Vector2i>& compVec)
{
	std::vector<sf::Vector2i> result;

	for (auto& comp : compVec)
	{
		auto coord = std::find(inVec.begin(), inVec.end(), comp);
		if (coord == inVec.end())
		{
			result.push_back(comp);
		}
	}

	return result;
}

void drft::system::ChunkManager::build(sf::Vector2i chunkCoordinate)
{
	_chunkStates[{ chunkCoordinate.x, chunkCoordinate.y }] = ChunkState::Building;
	std::cout << "Building chunk (" << chunkCoordinate.x << ", " << chunkCoordinate.y << ") ... \n" << std::endl;

	auto origin = spatial::toTileSpace(chunkCoordinate);
	util::buildTestTrees(registry, 100, { origin.x, origin.y, spatial::CHUNK_WIDTH, spatial::CHUNK_HEIGHT });
	_chunkStates.at({ chunkCoordinate.x, chunkCoordinate.y }) = ChunkState::Built;
	_toBuild.pop();
}

void drft::system::ChunkManager::load(sf::Vector2i chunkCoordinate)
{
	_chunkStates[{ chunkCoordinate.x, chunkCoordinate.y }] = ChunkState::Loading;
	std::cout << "Loading chunk (" << chunkCoordinate.x << ", " << chunkCoordinate.y << ") ... \n" << std::endl;
	_chunkStates.at({ chunkCoordinate.x, chunkCoordinate.y }) = ChunkState::Loaded;
	_toLoad.pop();
}

void drft::system::ChunkManager::save(sf::Vector2i chunkCoordinate)
{
	_chunkStates[{ chunkCoordinate.x, chunkCoordinate.y }] = ChunkState::Saving;
	std::cout << "Saving chunk (" << chunkCoordinate.x << ", " << chunkCoordinate.y << ") ... \n" << std::endl;
	_chunkStates.at({ chunkCoordinate.x, chunkCoordinate.y }) = ChunkState::Saved;
	_grid.get().removeChunk(_toSave.front());
	_toSave.pop();
}
