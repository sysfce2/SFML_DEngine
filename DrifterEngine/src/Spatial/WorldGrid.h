#pragma once
#include "WorldChunk.h"

namespace drft::spatial
{
	const int CHUNK_WIDTH = 64;
	const int CHUNK_HEIGHT = 64;
	const int TILE_WIDTH = 16;
	const int TILE_HEIGHT = 16;

	enum class Layer
	{
		Tile,
		Item,
		Prop,
		Actor,
		Effect,
		Camera,
		Total
	};


	// Spatial conversion functions //

	// Converts an int to a layer on the grid.
	inline Layer toLayer(const int depth)
	{
		assert(depth > 0);
		assert(depth < (int)Layer::Total);

		return (Layer)depth;
	}
	// Converts a position in world space into a position in tile space.
	inline sf::Vector2i toTileSpace(const sf::Vector2f worldPosition)
	{
		int x = std::floor(worldPosition.x / TILE_WIDTH);
		int y = std::floor(worldPosition.y / TILE_HEIGHT);

		return { x, y };
	}
	// Converts a chunk coordinate into a position in tile space.
	inline sf::Vector2i toTileSpace(const sf::Vector2i chunkCoordinate)
	{
		int x = chunkCoordinate.x * CHUNK_WIDTH;
		int y = chunkCoordinate.y * CHUNK_HEIGHT;

		return { x,y };
	}
	// Converts a position in tile space into a position in world space.
	inline sf::Vector2f toWorldSpace(const sf::Vector2i tilePosition)
	{
		float x = tilePosition.x * TILE_WIDTH;
		float y = tilePosition.y * TILE_HEIGHT;

		return { x, y };
	}
	// Converts a tile position into a chunk coordinate
	inline sf::Vector2i toChunkCoordinate(const sf::Vector2i tilePosition)
	{
		int xChunk = tilePosition.x / CHUNK_WIDTH;
		if (tilePosition.x < 0)
		{
			xChunk -= 1;
		}
		int yChunk = tilePosition.y / CHUNK_HEIGHT;
		if (tilePosition.y < 0)
		{
			yChunk -= 1;
		}
		return { xChunk, yChunk };
	}
	// Converts a world position into a chunk coordinate
	inline sf::Vector2i toChunkCoordinate(const sf::Vector2f worldPosition)
	{
		auto tilePosition = toTileSpace(worldPosition);
		int xChunk = tilePosition.x / CHUNK_WIDTH;
		if (tilePosition.x < 0)
		{
			xChunk -= 1;
		}
		int yChunk = tilePosition.y / CHUNK_HEIGHT;
		if (tilePosition.y < 0)
		{
			yChunk -= 1;
		}
		return { xChunk, yChunk };
	}
	// Converts a tile position into a chunk-relative position.
	// Upper-left corner of chunk is (0,0)
	inline sf::Vector2i toLocalChunkSpace(const sf::Vector2i tilePosition)
	{
		int xPos = tilePosition.x % CHUNK_WIDTH;
		if (tilePosition.x < 0)
		{
			xPos += CHUNK_WIDTH - 1;
		}

		int yPos = tilePosition.y % CHUNK_HEIGHT;
		if (tilePosition.y < 0)
		{
			yPos += CHUNK_HEIGHT - 1;
		}

		return { xPos, yPos };
	}

	// Class for single point of access to query/manipulate entity and chunk positions
	class WorldGrid
	{
	public:
		// Creates a world grid containing chunks with the given dimensions.
		WorldGrid() = default;
		WorldGrid(const WorldGrid&) = delete;
		WorldGrid& operator=(const WorldGrid&) = delete;
		WorldGrid(const WorldGrid&&) = delete;
		WorldGrid& operator=(const WorldGrid&&) = delete;

		// Entities //

		// Places entity in world tile position.
		void placeEntity(const entt::entity& entity, const sf::Vector2i tilePosition, Layer layer);

		// Removes entity from given world tile position and returns that entity.
		entt::entity removeEntity(const entt::entity& entity, const sf::Vector2i tilePosition, Layer layer);

		// Moves entity from one world tile position to another and returns if successful.
		bool moveEntity(const entt::entity, const sf::Vector2i fromTilePosition, const sf::Vector2i toTilePosition, Layer layer);

		// Returns all entities at the given world tile position.
		std::vector<entt::entity> entitiesAt(const sf::Vector2i tilePosition, const Layer layer);

		// Chunks //

		// Get the chunk coordinates that contain at least one entity.
		const std::vector<sf::Vector2i>& getActiveChunks();

		// Clears the chunk of all entities and removes the chunk.
		void removeChunk(const sf::Vector2i coordinate);

		// Returns all entities at the given chunk coordinate.
		std::vector<entt::entity> getAllEntities(const sf::Vector2i coordinate);

	private:
		std::map<std::pair<int, int>, std::unique_ptr<WorldChunk>> _chunks;
		std::vector<sf::Vector2i> _activeChunks;
	};

} // Namespace drft::spatial