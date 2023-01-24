#include "WorldChunk.h"
/*
namespace drft::spatial
{
	class WorldChunk;
}
*/

namespace drft::spatial
{
	enum class Layer
	{
		Item = 0,
		Prop,
		Actor,
		Effect,
		Total
	};


	// Class for single point of access to query/manipulate entity and chunk positions
	class WorldGrid
	{
	public:
		// Creates a world grid containing chunks with the given dimensions.
		WorldGrid(int chunkWidth = 64, int chunkHeight = 64);
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
	};

} // Namespace drft::spatial