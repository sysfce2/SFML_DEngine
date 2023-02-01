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
		Active,
		Built,
		Building,
		Saved,
		Saving,
		Loaded,
		Loading
	};

	class ChunkManager : public System
	{
	public:
		ChunkManager(entt::registry& registry)
			: _grid(registry.ctx().get<spatial::WorldGrid&>()), System(registry)
		{
			phase = Phase::OnPostUpdate;
		}

		virtual void init() override;
		virtual void update(const float dt) override;

	private:
		std::vector<sf::Vector2i> getChunksInRadius(const sf::Vector2i centerPosition, const float radius);
		// Returns a vector of coordinates in the comp-vec that aren't in the in-vec.
		std::vector<sf::Vector2i> compareChunkList(const std::vector<sf::Vector2i>& inVec, const std::vector<sf::Vector2i>& compVec);

		void build(sf::Vector2i chunkCoordinate);
		void load(sf::Vector2i chunkCoordinate);
		void save(sf::Vector2i chunkCoordinate);

	private:
		std::reference_wrapper<spatial::WorldGrid> _grid;
		const float _activeChunkRadius = 2;
		const float _toSaveRadius = _activeChunkRadius + 1;
		std::map<std::pair<int, int>, ChunkState> _chunkStates;
		sf::Vector2i _currentPosition = { -1,-1 };
		std::queue<sf::Vector2i> _toBuild;
		std::queue<sf::Vector2i> _toLoad;
		std::queue<sf::Vector2i> _toSave;
	};
}



