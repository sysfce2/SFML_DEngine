#include "pch.h"
#include "TestEntities.h"
#include "Components/Components.h"
#include "Random/RandomNumberGenerator.h"
#include "Spatial/Conversions.h"
#include "Factory/EntityFactory.h"

void drft::util::buildMany(std::string entity, int number, sf::IntRect inArea, entt::registry& registry)
{
	auto& factory = registry.ctx().get<EntityFactory&>();

	for (int i = 0; i < number; ++i)
	{
		int x = rng::RandomNumberGenerator::intInRange(inArea.left, inArea.left + inArea.width);
		int y = rng::RandomNumberGenerator::intInRange(inArea.top, inArea.top + inArea.height);
		auto position = spatial::toWorldSpace({ x,y });

		auto tree = factory.build(entity, registry);
		tree.patch<component::Position>(
			[&position](auto& pos)
			{
				pos.position = position;
			}
		);
	}
}
