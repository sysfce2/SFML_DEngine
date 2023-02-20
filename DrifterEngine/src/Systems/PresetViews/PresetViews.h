#pragma once
#include "Components/Components.h"
#include "Components/Tags.h"

namespace drft::system::views
{
	static auto Active = [](const entt::registry& registry) -> auto
	{
		return registry.view<component::tag::Active>(entt::exclude<component::Prototype>);
	};
}