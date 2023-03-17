#include "pch.h"
#include "ChunkManager.h"
#include "Spatial/WorldGrid.h"
#include "Spatial/Conversions.h"
#include "Spatial/Helpers.h"
#include "Components/Components.h"
#include "Utility/DebugInfo.h"

using namespace drft::system;
static constexpr std::string_view CHUNK_SAVE_PATH = ".\\data\\runtime\\chunks\\";

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

	updateChunkStates(newPosition);

	process(_toBuild, BUILD);
	process(_toLoad, LOAD);
	process(_toSave, SAVE);

	cleanUpChunks(newPosition);
}

void drft::system::ChunkManager::updateChunkStates(sf::Vector2i newPosition)
{
	auto activeCoords = spatial::getIntPointsInRadius(newPosition, _activeChunkRadius);

	// Ensure active chunks are active or will be built
	for (auto coord : activeCoords)
	{
		auto keyablePair = std::make_pair(coord.x, coord.y);
		if (!_chunks.contains(keyablePair))
		{
			_chunks.emplace(std::make_pair(keyablePair, spatial::VirtualChunk{ coord }));
		}
		auto& chunk = _chunks.at(keyablePair);

		switch (chunk.getState())
		{
		case spatial::ChunkState::None:
			if (std::filesystem::exists(CHUNK_SAVE_PATH.data() + chunk.toString() + ".dat"))
			{
				chunk.setState(spatial::ChunkState::ToLoad);
				_toLoad.push(coord);
			}
			else
			{
				chunk.setState(spatial::ChunkState::ToBuild);
				_toBuild.push(coord);
			}
			break;
		case spatial::ChunkState::Built:
			chunk.setState(spatial::ChunkState::Active);
			break;
		case spatial::ChunkState::Loaded:
			chunk.setState(spatial::ChunkState::Active);
			break;
		case spatial::ChunkState::Saved:
			chunk.setState(spatial::ChunkState::ToLoad);
			_toLoad.push(coord);
			break;
		default:
			break;
		}
	}

	// Then, scan for chunks to save
	for (auto& [coord, chunk] : _chunks)
	{
		if (chunk.getState() != spatial::ChunkState::Active)
		{
			continue;
		}
		float distance = std::hypotf(static_cast<float>((newPosition.x - coord.first)),
									static_cast<float>((newPosition.y - coord.second)));
		if (distance > _toSaveRadius)
		{
			_toSave.push({ coord.first, coord.second });
			chunk.setState(spatial::ChunkState::ToSave);
		}
	}
}

void drft::system::ChunkManager::cleanUpChunks(sf::Vector2i newPosition)
{
	std::vector<std::pair<int, int>> toDelete;
	for (auto& [coord, chunk] : _chunks)
	{
		if (chunk.getState() != spatial::ChunkState::Saved)
		{
			continue;
		}
		float distance = std::hypotf(static_cast<float>((newPosition.x - coord.first)),
									static_cast<float>((newPosition.y - coord.second)));
		if (distance > _toSaveRadius)
		{
			toDelete.push_back(coord);
		}
	}
	auto& grid = registry->ctx().get<spatial::WorldGrid&>();
	for (auto pair : toDelete)
	{
		grid.removeChunk({ pair.first, pair.second });
		_chunks.erase(pair);
		std::cout << "Chunk " << pair.first << "_" << pair.second << " cleaned up." << std::endl;
	}
}

void drft::system::ChunkManager::process(std::queue<sf::Vector2i>& chunkQueue, ProcessType type)
{
	if (chunkQueue.empty()) return;
	
	std::vector<std::pair<int, int>> toDelete;

	auto coord = chunkQueue.front();
	auto keyablePair = std::make_pair(coord.x, coord.y);
	auto status = spatial::ioStatus::Busy;

	switch (type)
	{
	case BUILD:
		status = _chunks.at(keyablePair).build(*registry);
		break;
	case SAVE:
		status = _chunks.at(keyablePair).save(*registry, CHUNK_SAVE_PATH.data());
		break;
	case LOAD:
		status = _chunks.at(keyablePair).load(*registry, CHUNK_SAVE_PATH.data());
		break;
	}
	if (status == spatial::ioStatus::Busy)
	{
		auto temp = chunkQueue.front();
		chunkQueue.push(temp);
	}
	chunkQueue.pop();
}