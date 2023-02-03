#pragma once

namespace drft::serialization
{

	void serializeEntity(entt::registry& registry, entt::entity entity);
	entt::entity deserializeEntity(entt::registry& registry);

}
