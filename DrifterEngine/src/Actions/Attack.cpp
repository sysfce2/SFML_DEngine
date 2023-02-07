#include "pch.h"
#include <cstring>
#include "Attack.h"
#include "Components/Components.h"

using namespace drft::action;

std::unique_ptr<Action> drft::action::Attack::execute(entt::registry& registry, const entt::entity actor)
{
	auto myInfo = registry.try_get<component::Info>(actor);

	if (!myInfo) return nullptr;

	if (!std::isupper(myInfo->name[0]))
	{
		std::cout << "The " + myInfo->name;
	}

	std::cout << myInfo->name << " attemts to attack ";

	for (auto target : potentialTargets)
	{
		auto theirInfo = registry.try_get<component::Info>(target);
		if (!theirInfo) continue;
		
		if (std::isupper(theirInfo->name[0]))
		{
			std::cout << theirInfo->name << std::endl;
		}
		else
		{
			std::cout << "\nthe " + theirInfo->name << std::endl;
		}
	}

	return nullptr;
}

std::unique_ptr<Action> drft::action::Attack::clone() const 
{
	return std::make_unique<Attack>(potentialTargets);
}