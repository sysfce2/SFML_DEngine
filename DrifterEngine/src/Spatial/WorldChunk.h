#pragma once
#include "Cell.h"
#include "Grid.h"


namespace drft::spatial
{

	class WorldChunk
	{
	public:
		WorldChunk(int width, int height);

		// Places entity in chunk tile position.
		void placeEntity(const entt::entity& entity, const sf::Vector2i chunkPosition, int layer);
		// Removes entity from given chunk tile position and returns that entity.
		entt::entity removeEntity(const entt::entity& entity, const sf::Vector2i chunkPosition, int layer);
		// Moves entity from one chunk tile position to another and returns if successful.
		bool moveEntity(const entt::entity, const sf::Vector2i fromChunkPosition, const sf::Vector2i toChunkPosition, int layer);
		std::vector<entt::entity> entitiesAt(sf::Vector2i chunkPosition, int layer) const;
		std::vector<entt::entity> getAllEntities();
		// Removes all entities reverting the WorldChunk to its default constructed state.
		void clear();
		// Checks if the chunk contains no entities
		bool empty() const;

	private:
		std::set<entt::entity> _entities;
		std::unique_ptr< Grid<Cell> > _grid = nullptr;
	};

} // namespace drft::spatial

