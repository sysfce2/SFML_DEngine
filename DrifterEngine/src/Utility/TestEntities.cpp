#include "pch.h"
#include "TestEntities.h"
#include "Components/Components.h"
#include "Spatial/Conversions.h"
#include "Factory/EntityFactory.h"

void drft::util::buildMany(std::string entity, int number, sf::IntRect inArea, entt::registry& registry)
{
	std::srand((unsigned int)&number);

	auto& factory = registry.ctx().get<EntityFactory&>();

	for (int i = 0; i < number; ++i)
	{
		int x = (std::rand() % inArea.width) + inArea.left;
		int y = (std::rand() % inArea.height) + inArea.top;
		auto position = spatial::toWorldSpace({ x,y });

		auto tree = factory.build(entity, registry);
		tree.patch<component::Position>(
			[&](auto& pos)
			{
				pos.position = position;
			}
		);
	}
}
