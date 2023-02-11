#pragma once

namespace drft::spatial
{
	// Spatial conversion functions //

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
}

