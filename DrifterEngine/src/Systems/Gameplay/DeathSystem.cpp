#include "pch.h"
#include "DeathSystem.h"
#include "Components/Components.h"
#include "Utility/EntityHelpers.h"

void drft::system::DeathSystem::init()
{
}

void drft::system::DeathSystem::update(const float dt)
{
	auto view = registry->view<component::action::Die>();
	for (auto entity : view)
	{
		std::cout << "The " << util::getEntityName({ *registry, entity }) << " is destroyed!" << std::endl;
		registry->destroy(entity);
	}
}
