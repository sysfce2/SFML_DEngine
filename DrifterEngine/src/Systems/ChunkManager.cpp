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

	if (!_toSave.empty())
	{
		auto& coord = _toSave.front();
		save(coord);
		_toSave.pop();
	}
	if (!_toBuild.empty())
	{
		auto& coord = _toBuild.front();
		build(coord);
		_toBuild.pop();
	}
	if (!_toLoad.empty())
	{
		auto& coord = _toLoad.front();
		load(coord);
		_toLoad.pop();
	}
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
			setState(coord, ChunkState::Building);
			_toBuild.push(coord);
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
			setState(coord, ChunkState::Loading);
			_toLoad.push(coord);
			break;
		default:
			std::cout << "No way to handle chunk state " << (int)state << std::endl;
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
		float distance = std::hypotf((newPosition.x - coord.first), (newPosition.y - coord.second));
		if (distance > _toSaveRadius)
		{
			_toSave.push({ coord.first, coord.second });
			_chunks.at(coord).state = ChunkState::Saving;
		}
	}

	auto& debug = registry->ctx().get<util::DebugInfo>();
	std::string dataStr = std::to_string(_chunks.size());
	debug.addString("Chunks Active", dataStr);

	_currentPosition = newPosition;
}

void drft::system::ChunkManager::setState(sf::Vector2i coordinate, ChunkState state)
{
	_chunks.at({ coordinate.x, coordinate.y }).state = state;
}

void drft::system::ChunkManager::build(sf::Vector2i chunkCoordinate)
{
	//std::cout << "Building chunk (" << chunkCoordinate.x << ", " << chunkCoordinate.y << ") ... \n" << std::endl;
	auto origin = spatial::toTileSpace(chunkCoordinate);
	util::buildMany("Tree", 100, { origin.x, origin.y, spatial::CHUNK_WIDTH, spatial::CHUNK_HEIGHT }, *registry);
	util::buildMany("NPC", 1, { origin.x, origin.y, spatial::CHUNK_WIDTH, spatial::CHUNK_HEIGHT }, *registry);
	_chunks.at({ chunkCoordinate.x, chunkCoordinate.y }).state = ChunkState::Built;
}

void drft::system::ChunkManager::load(sf::Vector2i chunkCoordinate)
{
	//std::cout << "Loading chunk (" << chunkCoordinate.x << ", " << chunkCoordinate.y << ") ... \n" << std::endl;
	_chunks.at({ chunkCoordinate.x, chunkCoordinate.y }).state = ChunkState::Loaded;
}

void drft::system::ChunkManager::save(sf::Vector2i chunkCoordinate)
{
	//std::cout << "Saving chunk (" << chunkCoordinate.x << ", " << chunkCoordinate.y << ") ... \n" << std::endl;
	_chunks.at({ chunkCoordinate.x, chunkCoordinate.y }).state = ChunkState::Saved;
}
