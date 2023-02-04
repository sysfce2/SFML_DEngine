#pragma once
#include "System.h"

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
		Building,
		Saved,
		Saving,
		Loaded,
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
		void updateChunks(sf::Vector2i newPosition);
		std::vector<sf::Vector2i> getCoordinatesInRadius(const sf::Vector2i centerPosition, const float radius);

		void setState(sf::Vector2i coordinate, ChunkState state);

		void build(sf::Vector2i chunkCoordinate);
		void load(sf::Vector2i chunkCoordinate);
		void save(sf::Vector2i chunkCoordinate);

	private:
		bool _isFirstPass = true;
		std::map<std::pair<int, int>, VirtualChunk> _chunks;

		const float _activeChunkRadius = 2;
		const float _toSaveRadius = _activeChunkRadius + 1;
		sf::Vector2i _currentPosition = { 0, 0 };

		std::queue<sf::Vector2i> _toBuild;
		std::queue<sf::Vector2i> _toLoad;
		std::queue<sf::Vector2i> _toSave;
	};
}



