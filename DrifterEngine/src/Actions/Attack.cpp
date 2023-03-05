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
		int damage = calculateDamage(entity);

		for (auto target : potentialTargets)
		{
			// TODO: Check if not ally
			entity.registry()->emplace<component::Damage>(target, damage);
		}
	}

	return nullptr;
}

std::unique_ptr<Action> drft::action::Attack::clone() const 
{
	return std::make_unique<Attack>(potentialTargets);
}

float drft::action::Attack::calculateDamage(entt::handle& entity) const
{
	auto& attacker = entity.get<component::Attacker>();

	return attacker.baseDamage;
}
