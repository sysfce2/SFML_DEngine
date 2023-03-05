#include "pch.h"
#include "EntityHelpers.h"
#include "Components/Components.h"

const std::string drft::util::getEntityName(const entt::handle entity)
{
	auto info = entity.try_get<component::Info>();
	if (!info) return "UNNAMED";

	return info->name;
}
