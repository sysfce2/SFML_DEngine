#pragma once
#include "Components/Components.h"

namespace drft::system::views
{
	static auto Active = [](const entt::registry& registry) -> auto
	{
		return registry.view<component::Active>(entt::exclude<component::Prototype>);
	};
}