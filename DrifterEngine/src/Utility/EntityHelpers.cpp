#include "pch.h"
#include "EntityHelpers.h"
#include "Components/Components.h"

const std::string drft::util::getEntityName(const entt::entity entity, const entt::registry& registry)
{
	auto info = registry.try_get<component::Info>(entity);
	if (!info) return "UNNAMED";

	return info->name;
}
