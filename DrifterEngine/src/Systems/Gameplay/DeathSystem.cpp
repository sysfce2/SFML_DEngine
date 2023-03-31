#include "pch.h"
#include "DeathSystem.h"
#include "Factory/EntityFactory.h"
#include "Components/Components.h"
#include "Utility/EntityHelpers.h"

void drft::system::DeathSystem::init()
{
}

void drft::system::DeathSystem::update(const float dt)
{
	const auto& factory = registry->ctx().get<EntityFactory&>();
	auto view = registry->view<component::action::Die>();
	for (auto entity : view)
	{
		auto corpse = factory.build("Corpse", *registry);
		auto& pos = registry->get<component::Position>(entity);
		corpse.patch<component::Position>([&pos](auto& position)
			{
				position.position = pos.position;
			});
		registry->destroy(entity);
	}
}
