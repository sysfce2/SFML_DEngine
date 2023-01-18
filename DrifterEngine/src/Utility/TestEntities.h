#pragma once

namespace drft::util
{
	// Create a number of entities in the given registry with a random position and a random render component
	void buildTestEntities(entt::registry& registry, int numberOfEntities, sf::IntRect inArea);

}