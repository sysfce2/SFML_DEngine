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
		Item = 0,
		Prop,
		Actor,
		Effect,
		Total
	};

	// Converts a position in world space into a position in tile space.
	inline sf::Vector2f toTileSpace(sf::Vector2f worldPosition)
	{
		float x = std::floor((worldPosition.x / TILE_WIDTH) * TILE_WIDTH);
		float y = std::floor((worldPosition.y / TILE_HEIGHT) * TILE_HEIGHT);

		return { x, y };
	}

	// Converts a position in tile space into a position in world space.
	inline sf::Vector2f toWorldSpace(sf::Vector2f tilePosition)
	{
		float x = tilePosition.x * TILE_WIDTH;
		float y = tilePosition.y * TILE_HEIGHT;

		return { x, y };
	}


	// Class for single point of access to query/manipulate entity and chunk positions
	class WorldGrid
	{
	public:
		// Creates a world grid containing chunks with the given dimensions.
		WorldGrid(sf::Vector2i chunkDimensions = {64, 64}, sf::Vector2i tileDimensions = {16, 16});
		WorldGrid(const WorldGrid&) = delete;
		WorldGrid& operator=(const WorldGrid&) = delete;
		WorldGrid(const WorldGrid&&) = delete;
		WorldGrid& operator=(const WorldGrid&&) = delete;

		// Entities //
		
		// Places entity in world tile position.
		void placeEntity(const entt::entity& entity, const sf::Vector2i worldPosition, Layer layer);
		// Removes entity from given world tile position and returns that entity.
		entt::entity removeEntity(const entt::entity& entity, const sf::Vector2i worldPosition, Layer layer);
		// Moves entity from one world tile position to another and returns if successful.
		bool moveEntity(const entt::entity, const sf::Vector2i fromWorldPosition, const sf::Vector2i toWorldPosition, Layer layer);
		// Returns all entities at the given world tile position.
		std::vector<entt::entity> entitiesAt(sf::Vector2i worldPosition, Layer layer);

		// Chunks //

	private:
		std::map<std::pair<int, int>, std::unique_ptr<WorldChunk>> _chunks;
		int _chunkWidth;
		int _chunkHeight;
		int _tileWidth;
		int _tileHeight;
	};

} // Namespace drft::spatial