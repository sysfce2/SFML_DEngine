#pragma once
#include "Systems/System.h"

namespace drft::spatial
{
	class WorldGrid;
}


namespace drft::system
{
	enum class ChunkState
	{
		None,
		Active,
		Built,
		ToBuild,
		Building,
		Saved,
		ToSave,
		Saving,
		Loaded,
		ToLoad,
		Loading
	};

	struct VirtualChunk
	{
		VirtualChunk(ChunkState state = ChunkState::None) : state(state) {}
		ChunkState state = ChunkState::None;
	};

	class ChunkManager : public System
	{
	public:
		ChunkManager()
		{
			phase = Phase::OnPostUpdate;
		}

		virtual void init() override;
		virtual void update(const float dt) override;

	private:
		void updateChunks(sf::Vector2i aroundNewPosition);


		void setState(sf::Vector2i coordinate, ChunkState state);

		bool build(sf::Vector2i chunkCoordinate);
		bool load(sf::Vector2i chunkCoordinate);
		bool save(sf::Vector2i chunkCoordinate);

		

		void process(std::set<std::pair<int, int>>& chunkSet, bool (ChunkManager::*func)(sf::Vector2i));

	private:
		std::map<std::pair<int, int>, VirtualChunk> _chunks;

		const int _activeChunkRadius = 2;
		const int _toSaveRadius = _activeChunkRadius + 1;
		sf::Vector2i _currentPosition = { 0, 0 };

		std::set<std::pair<int, int>> _toBuild;
		std::set<std::pair<int, int>> _toLoad;
		std::set<std::pair<int, int>> _toSave;
	};
}



