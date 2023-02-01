#include "pch.h"
#include "TestEntities.h"
#include "Components/Components.h"
#include "Spatial/WorldGrid.h"

void drft::util::buildTestTrees(entt::registry& registry, int numberOfTrees, sf::IntRect inArea)
{
	std::srand((unsigned int)&numberOfTrees);

	for (int i = 0; i < numberOfTrees; ++i)
	{
		int x = (std::rand() % inArea.width) + inArea.left;
		int y = (std::rand() % inArea.height) + inArea.top;
		int sprite = 5;
		char r = (std::rand() % 55) + 20;
		char g = (std::rand() % 25) + 100;
		char b = (std::rand() % 55) + 20;

		auto e = registry.create();

		auto tilePos = spatial::toWorldSpace({ x,y });

		registry.emplace<component::Position>(e, tilePos, (int)spatial::Layer::Actor);
		registry.emplace<component::Render>(e, (unsigned int)sprite, sf::Color(r, g, b, 255));
	}
}
