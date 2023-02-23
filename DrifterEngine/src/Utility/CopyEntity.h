#pragma once

namespace drft::util
{
	void copyEntity(entt::entity to, entt::entity from, entt::registry& registry);
	void copyEntity(entt::entity to, entt::entity from, entt::registry& toRegistry, entt::registry& fromRegistry);
}