#include "pch.h"
#include "TestEntities.h"
#include "Components/Components.h"
#include "Spatial/WorldGrid.h"

void drft::util::buildTestEntities(entt::registry& registry, int numberOfEntities, sf::IntRect inArea)
{
	std::srand((unsigned int)&numberOfEntities);

	for (int i = 0; i < numberOfEntities; ++i)
	{
		int x = (std::rand() % inArea.width) + inArea.left;
		int y = (std::rand() % inArea.height) + inArea.top;
		int sprite = (std::rand() % 10) + 1;
		char r = std::rand() % 255;
		char g = std::rand() % 255;
		char b = std::rand() % 255;

		auto e = registry.create();

		auto tilePos = spatial::toWorldSpace({ x,y });

		registry.emplace<component::Position>(e, tilePos, (int)spatial::Layer::Actor);
		registry.emplace<component::Render>(e, (unsigned int)sprite, sf::Color(r, g, b, 255));
	}
}
