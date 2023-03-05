#include "pch.h"
#include <cstring>
#include "Attack.h"
#include "Components/Components.h"
#include "Utility/EntityHelpers.h"

using namespace drft::action;

std::unique_ptr<Action> drft::action::Attack::execute(entt::handle& entity)
{
	auto attacker = entity.try_get<component::Attacker>();
	
	if (attacker)
	{
		std::cout << "The " << util::getEntityName(entity) << " launches an attack." << std::endl;
		for (auto target : potentialTargets)
		{
			
			entity.registry()->emplace<component::Damage>(target, attacker->baseDamage);
		}
	}

	return nullptr;
}

std::unique_ptr<Action> drft::action::Attack::clone() const 
{
	return std::make_unique<Attack>(potentialTargets);
}