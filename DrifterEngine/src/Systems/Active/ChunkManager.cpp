#include "pch.h"
#include "ChunkManager.h"
#include "Spatial/WorldGrid.h"
#include "Spatial/Conversions.h"
#include "Spatial/Helpers.h"
#include "Components/Components.h"
#include "Utility/TestEntities.h"
#include "Utility/DebugInfo.h"

using namespace drft::system;
using namespace std::chrono_literals;

const auto WAIT_TIME = 1s; // How long to wait for async operations 

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

	process(_toBuild, &ChunkManager::build, dt);
	process(_toLoad, &ChunkManager::load, dt);
	process(_toSave, &ChunkManager::save, dt);
}

void drft::system::ChunkManager::updateChunks(sf::Vector2i newPosition)
{
	auto activeCoords = spatial::getIntPointsInRadius(newPosition, _activeChunkRadius);

	// Ensure active chunks are active or will be built
	for (auto& coord : activeCoords)
	{
		auto& chunk = _chunks[{ coord.x, coord.y }]; // Automatically constructs chunk with "none" state if not in map.
		switch (chunk.getState())
		{
		case ChunkState::None:
			chunk.setState(ChunkState::ToBuild);
			_toBuild.insert({ coord.x, coord.y });
			break;
		case ChunkState::Active:
			// No need to do anything, already active
			break;
		case ChunkState::Building:

			break;
		case ChunkState::Loading:

			break;
		case ChunkState::Saving:

			break;
		case ChunkState::Built:
			chunk.setState(ChunkState::Active);
			break;
		case ChunkState::Loaded:
			chunk.setState(ChunkState::Active);
			break;
		case ChunkState::Saved:
			chunk.setState(ChunkState::ToLoad);
			_toLoad.insert({ coord.x, coord.y });
			break;
		default:
			std::cout << "Chunk is in the middle of process " << (int)(chunk.getState()) << std::endl;
			break;
		}
	}
	// Then, scan for chunks to save
	for (auto& [coord, chunk] : _chunks)
	{
		if (chunk.getState() != ChunkState::Active)
		{
			continue;
		}
		float distance = std::hypotf(static_cast<float>((newPosition.x - coord.first)),
									static_cast<float>((newPosition.y - coord.second)));
		if (distance > _toSaveRadius)
		{
			_toSave.insert(coord);
			chunk.setState(ChunkState::ToSave);
		}
	}

	auto& debug = registry->ctx().get<util::DebugInfo>();
	std::string dataStr = std::to_string(_chunks.size());
	debug.addString("Chunks Active", dataStr);
}

bool drft::system::ChunkManager::build(sf::Vector2i chunkCoordinate, const float dt)
{
	auto& chunk = _chunks.at({ chunkCoordinate.x, chunkCoordinate.y });
	chunk.setState(ChunkState::Building);
	auto origin = spatial::toTileSpace(chunkCoordinate);
	util::buildMany("Tree", 400, { origin.x, origin.y, spatial::CHUNK_WIDTH, spatial::CHUNK_HEIGHT }, *registry);
	util::buildMany("NPC", 2, { origin.x, origin.y, spatial::CHUNK_WIDTH, spatial::CHUNK_HEIGHT }, *registry);
	chunk.setState(ChunkState::Built);
	return true;
}

bool drft::system::ChunkManager::load(sf::Vector2i chunkCoordinate, const float dt)
{
	auto& chunk = _chunks.at({ chunkCoordinate.x, chunkCoordinate.y });
	if (chunk.getState() == ChunkState::ToLoad)
	{
		chunk.setFuture(std::async(std::launch::async, &ChunkManager::asyncLoad, this, chunkCoordinate));
		chunk.setState(ChunkState::Loading);
	}

	auto status = chunk.getFuture().wait_for(WAIT_TIME);
	if (status != std::future_status::ready) return false;

	// 4. Get registry created from async load
	// Append to main registry

	chunk.setState(ChunkState::Loaded);
	return true;
}

bool drft::system::ChunkManager::save(sf::Vector2i chunkCoordinate, const float dt)
{
	auto& chunk = _chunks.at({ chunkCoordinate.x, chunkCoordinate.y });
	if (chunk.getState() == ChunkState::ToSave)
	{
		chunk.setFuture(std::async(std::launch::async, &ChunkManager::asyncSave, this, chunkCoordinate));
		chunk.setState(ChunkState::Saving);
	}

	auto status = chunk.getFuture().wait_for(WAIT_TIME);
	if (status != std::future_status::ready) return false;

	// 4. Get registry created from async load
	// Append to main registry

	chunk.setState(ChunkState::Saved);
	
	return true;
}

bool drft::system::ChunkManager::asyncBuild(sf::Vector2i chunkCoordinate)
{
	return true;
}

bool drft::system::ChunkManager::asyncLoad(sf::Vector2i chunkCoordinate)
{
	int count = 0;
	for (int i = 0; i < 10000000; ++i)
	{
		if (i > 5)
		{
			count += (i / 2);
		}
	}

	return true;
}

bool drft::system::ChunkManager::asyncSave(sf::Vector2i chunkCoordinate)
{
	int count = 0;
	for (int i = 0; i < 10000000; ++i)
	{
		if (i > 5)
		{
			count += (i / 2);
		}
	}

	return true;
}

void drft::system::ChunkManager::process(std::set<std::pair<int, int>>& chunkSet, bool (ChunkManager::*func)(sf::Vector2i, const float), const float dt)
{
	if (!chunkSet.empty())
	{
		std::vector<std::pair<int,int>> toDelete;
		for (auto coord : chunkSet)
		{
			if ((this->*func)({coord.first, coord.second}, dt))
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

void drft::system::VirtualChunk::setState(ChunkState state)
{
	this->state = state;
}

ChunkState drft::system::VirtualChunk::getState() const
{
	return state;
}

void drft::system::VirtualChunk::setFuture(std::shared_future<bool> future)
{
	this->future = future;
}

const std::shared_future<bool>& drft::system::VirtualChunk::getFuture() const
{
	return future;
}
