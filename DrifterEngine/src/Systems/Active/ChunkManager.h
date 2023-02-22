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


	namespace
	{
		struct VirtualChunk
		{
			VirtualChunk(ChunkState state = ChunkState::None)
				: state(state) {}

			void setState(ChunkState state);
			ChunkState getState() const;
			void setFuture(std::shared_future<bool> future);
			const std::shared_future<bool>& getFuture() const;

		private:
			ChunkState state = ChunkState::None;
			std::shared_future<bool> future;
		};
	}
	

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

		bool build(sf::Vector2i chunkCoordinate, const float dt);
		bool load(sf::Vector2i chunkCoordinate, const float dt);
		bool save(sf::Vector2i chunkCoordinate, const float dt);

		bool asyncBuild(sf::Vector2i chunkCoordinate);
		bool asyncLoad(sf::Vector2i chunkCoordinate);
		bool asyncSave(sf::Vector2i chunkCoordinate);

		void process(std::set<std::pair<int, int>>& chunkSet,
			bool(ChunkManager::*func)(sf::Vector2i, const float), const float dt);

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



