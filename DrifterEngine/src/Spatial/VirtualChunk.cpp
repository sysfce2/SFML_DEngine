#include "pch.h"
#include "VirtualChunk.h"
#include "Utility/CopyEntity.h"
#include "Utility/TestEntities.h"
#include "Conversions.h"
#include "WorldGrid.h"
#include "Snapshot/EnTTSnapshot.h"

using namespace drft::spatial;
using namespace std::chrono_literals;

static const auto WAIT_TIME = 3ms; // How long to wait for async operations
static const auto CHUNK_SAVE_PATH = ".\\data\\runtime\\chunks\\";

void drft::spatial::VirtualChunk::setState(ChunkState state)
{
	this->_state = state;
}

ChunkState VirtualChunk::getState() const
{
	return this->_state;
}

ioStatus drft::spatial::VirtualChunk::build(entt::registry& reg)
{
	if (getState() == ChunkState::ToBuild)
	{
		std::cout << "Building " << toString() << std::endl;
		setState(ChunkState::Building);
	}
	auto origin = toTileSpace(_coordinate);
	auto bounds = sf::Vector2i{ CHUNK_WIDTH, CHUNK_HEIGHT };
	util::buildMany("Tree", 100, { origin.x, origin.y, bounds.x, bounds.y }, reg);
	util::buildMany("NPC", 10, { origin.x, origin.y, bounds.x, bounds.y }, reg);
	
	setState(ChunkState::Built);

	return ioStatus::Done;
}

ioStatus drft::spatial::VirtualChunk::load(entt::registry& reg)
{
	if (getState() == ChunkState::ToLoad)
	{
		setFuture(std::async(std::launch::async, &VirtualChunk::loadChunkFromFile, this));
		std::cout << "Loading " << toString() << std::endl;
		setState(ChunkState::Loading);
	}

	auto status = getFuture().wait_for(WAIT_TIME);
	if (status != std::future_status::ready)
	{
		return ioStatus::Busy;
	}

	util::copyEntities(reg, _asyncRegistry);
	
	_asyncRegistry.clear();
	
	setState(ChunkState::Loaded);
	if (!_asyncRegistry.empty())
	{
		throw std::exception("Registry should be empty");
	}

	return ioStatus::Done;
}

ioStatus drft::spatial::VirtualChunk::save(entt::registry& reg)
{
	if (getState() == ChunkState::ToSave)
	{
		auto& grid = reg.ctx().get<spatial::WorldGrid&>();
		auto entities = grid.getAllEntities(this->_coordinate);
		//grid.removeChunk(_coordinate);
		if (entities.empty())
		{
			std::cout << "No need to save " << toString() << " chunk empty" << std::endl;
			setState(ChunkState::Saved);
			return ioStatus::Done;
		}
		
		util::copyEntities(entities, _asyncRegistry, reg);
		
		for (auto e : entities)
		{
			reg.destroy(e);
		}
		
		setFuture(std::async(std::launch::async, &VirtualChunk::saveChunkToFile, this));
		std::cout << "Saving " << toString() << std::endl;
		setState(ChunkState::Saving);
	}

	auto status = getFuture().wait_for(WAIT_TIME);
	if (status != std::future_status::ready)
	{
		return ioStatus::Busy;
	}
	
	_asyncRegistry.clear();
	
	if (!_asyncRegistry.empty())
	{
		throw std::exception("Registry should be empty");
	}

	setState(ChunkState::Saved);

	return ioStatus::Done;
}

void VirtualChunk::setFuture(std::shared_future<void> future)
{
	this->_future = future;
}

const std::shared_future<void>& VirtualChunk::getFuture() const
{
	return this->_future;
}

void drft::spatial::VirtualChunk::saveChunkToFile() const
{
	if (_asyncRegistry.empty())
	{
		throw std::exception("Registry should not be empty");
	}
	using namespace snapshot;
	std::string fullPath = CHUNK_SAVE_PATH + this->toString() + ".dat";
	std::ofstream ofs(fullPath, std::ios::binary | std::ofstream::trunc);
	cereal::BinaryOutputArchive output{ ofs };
	Snapshot::save(output, _asyncRegistry);
	
	std::cout << "Chunk " << toString() << " saved to file" << std::endl;
}

void drft::spatial::VirtualChunk::loadChunkFromFile()
{
	if (!_asyncRegistry.empty())
	{
		throw std::exception("Registry should be empty");
	}
	using namespace snapshot;
	std::string fullPath = CHUNK_SAVE_PATH + this->toString() + ".dat";
	std::ifstream ifs(fullPath, std::ios::binary);
	if (ifs.good())
	{
		cereal::BinaryInputArchive input{ ifs };

		SnapshotLoader::load(input, _asyncRegistry);

		std::cout << "Chunk " << toString() << " loaded from file" << std::endl;
	}
	else
	{
		std::cout << "File " << fullPath << " does not exist" << std::endl;
	}
	
}

std::string drft::spatial::VirtualChunk::toString() const
{
	return std::string(std::to_string(_coordinate.x) + "_" + std::to_string(_coordinate.y));
}
