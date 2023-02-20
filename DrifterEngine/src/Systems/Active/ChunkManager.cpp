#include "pch.h"
#include "ChunkManager.h"
#include "Spatial/WorldGrid.h"
#include "Spatial/Conversions.h"
#include "Spatial/Helpers.h"
#include "Components/Components.h"
#include "Utility/TestEntities.h"
#include "Utility/DebugInfo.h"

using namespace drft::system;

void drft::system::ChunkManager::init()
{

}

void drft::system::ChunkManager::update(const float dt)
{
	auto cameraView = registry->view<const component::Camera, const component::Position>(entt::exclude<component::Prototype>);
	sf::Vector2i newPosition = { 0,0 };

	for (auto [entity, cam, pos] : cameraView.each())
	{
		newPosition = spatial::toChunkCoordinate(pos.position);
	}

	updateChunks(newPosition);

	process(_toBuild, &ChunkManager::build);
	process(_toLoad, &ChunkManager::load);
	process(_toSave, &ChunkManager::save);
}

void drft::system::ChunkManager::updateChunks(sf::Vector2i newPosition)
{
	auto activeCoords = spatial::getIntPointsInRadius(newPosition, _activeChunkRadius);

	// Ensure active chunks are active or will be built
	for (auto& coord : activeCoords)
	{
		ChunkState state = _chunks[{ coord.x, coord.y }].state;
		switch (state)
		{
		case ChunkState::None:
			setState(coord, ChunkState::ToBuild);
			_toBuild.insert({ coord.x, coord.y });
			break;
		case ChunkState::Active:
			// No need to do anything, already active
			break;
		case ChunkState::Building:
			break;
		case ChunkState::Loading:
			std::cout << "Loading state unhandled. Loading too slow?" << std::endl;
			break;
		case ChunkState::Saving:
			std::cout << "Saving state unhandled. Saving too slow?" << std::endl;
			break;
		case ChunkState::Built:
			setState(coord, ChunkState::Active);
			break;
		case ChunkState::Loaded:
			setState(coord, ChunkState::Active);
			break;
		case ChunkState::Saved:
			setState(coord, ChunkState::ToLoad);
			_toLoad.insert({ coord.x, coord.y });
			break;
		default:
			std::cout << "Chunk is in the middle of process " << (int)state << std::endl;
			break;
		}
	}

	// Then, scan for chunks to save
	for (auto [coord, chunk] : _chunks)
	{
		if (!(_chunks.at(coord).state == ChunkState::Active))
		{
			continue;
		}
		float distance = std::hypotf(static_cast<float>((newPosition.x - coord.first)),
									static_cast<float>((newPosition.y - coord.second)));
		if (distance > _toSaveRadius)
		{
			_toSave.insert({ coord.first, coord.second });
			_chunks.at(coord).state = ChunkState::ToSave;
		}
	}

	auto& debug = registry->ctx().get<util::DebugInfo>();
	std::string dataStr = std::to_string(_chunks.size());
	debug.addString("Chunks Active", dataStr);
}

void drft::system::ChunkManager::setState(sf::Vector2i coordinate, ChunkState state)
{
	_chunks.at({ coordinate.x, coordinate.y }).state = state;
}

bool drft::system::ChunkManager::build(sf::Vector2i chunkCoordinate)
{
	setState(chunkCoordinate, ChunkState::Building);

	auto origin = spatial::toTileSpace(chunkCoordinate);
	util::buildMany("Tree", 100, { origin.x, origin.y, spatial::CHUNK_WIDTH, spatial::CHUNK_HEIGHT }, *registry);
	util::buildMany("NPC", 2, { origin.x, origin.y, spatial::CHUNK_WIDTH, spatial::CHUNK_HEIGHT }, *registry);

	setState(chunkCoordinate, ChunkState::Built);
	return true;
}

bool drft::system::ChunkManager::load(sf::Vector2i chunkCoordinate)
{
	// 1. Load Async
	setState(chunkCoordinate, ChunkState::Loading);
	// 3. if future not finished, return false.
	// else
	// 4. Get registry created from async load
	// Copy into main registry
	setState(chunkCoordinate, ChunkState::Loaded);
	return true;
}

bool drft::system::ChunkManager::save(sf::Vector2i chunkCoordinate)
{
	// 1. Save Async
	setState(chunkCoordinate, ChunkState::Saving);
	// 3. if future not finished, return false.
	// else
	setState(chunkCoordinate, ChunkState::Saved);
	return true;
}

void drft::system::ChunkManager::process(std::set<std::pair<int, int>>& chunkSet, bool (ChunkManager::*func)(sf::Vector2i))
{
	if (!chunkSet.empty())
	{
		std::vector<std::pair<int,int>> toDelete;
		for (auto coord : chunkSet)
		{
			if ((this->*func)({coord.first, coord.second}))
			{
				toDelete.push_back(coord);
			}
		}
		for (auto coord : toDelete)
		{
			chunkSet.erase(coord);
		}
	}
}
