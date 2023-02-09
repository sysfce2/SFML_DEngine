#include "pch.h"
#include "TestEntities.h"
#include "Components/Components.h"
#include "Spatial/WorldGrid.h"
#include "Factory/EntityFactory.h"

void drft::util::buildTestTrees(entt::registry& registry, int numberOfTrees, sf::IntRect inArea)
{
	std::srand((unsigned int)&numberOfTrees);

	auto& factory = registry.ctx().get<EntityFactory&>();

	for (int i = 0; i < numberOfTrees; ++i)
	{
		int x = (std::rand() % inArea.width) + inArea.left;
		int y = (std::rand() % inArea.height) + inArea.top;
		int sprite = 5;
		char r = (std::rand() % 55) + 20;
		char g = (std::rand() % 25) + 100;
		char b = (std::rand() % 55) + 20;

		auto e = registry.create();

		auto position = spatial::toWorldSpace({ x,y });

		auto tree = factory.build("Tree", registry);
		tree.patch<component::Position>([&](auto& pos)
			{
				pos.position = position;
			});

	}
}
