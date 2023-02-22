#pragma once

namespace drft::util
{
	void serialize(entt::registry& registry, cereal::BinaryOutputArchive& output);
	void deserialize(entt::registry& registry, cereal::BinaryInputArchive& input);
	void transfer(entt::registry& from, entt::registry& to);
}



