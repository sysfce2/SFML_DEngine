#pragma once

namespace drft::util
{
	// Create a number of entities in the given registry with a random position and a random render component
	void buildTestTrees(entt::registry& registry, int numberOfTrees, sf::IntRect inArea);

}