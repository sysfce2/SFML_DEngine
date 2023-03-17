#include <pch.h>
#include "DamageSystem.h"
#include "Components/Components.h"
#include "Utility/EntityHelpers.h"

void drft::system::DamageSystem::init()
{
}

void drft::system::DamageSystem::update(const float dt)
{
	auto damageView = registry->view<component::action::TakeDamage>();

	for (auto [entity, damage] : damageView.each())
	{
		entt::handle handle = { *registry, entity };
		auto health = handle.try_get<component::Health>();
		if (!health) continue;
		
		health->current -= damage.amount;
		std::cout << "The " << util::getEntityName(handle) << " takes " << damage.amount << " damage!" << std::endl;
		handle.remove<component::action::TakeDamage>();

		if (health->current <= 0)
		{
			// TODO: add component instead of calling a function - should be handled in a death system 
			kill(handle);
		}
	}
}

void drft::system::DamageSystem::kill(entt::handle entity) const
{
	std::cout << "The " << util::getEntityName(entity) << " is destroyed!" << std::endl;
	entity.destroy();
}
