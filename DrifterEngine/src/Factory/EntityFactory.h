#pragma once
#include "../deps/Cereal/external/rapidjson/rapidjson.h"

namespace drft
{
	class EntityFactory
	{
	public:
		EntityFactory(entt::registry& registry);

		// Load entity prototypes from the given file.
		bool loadPrototypes(std::string filename);

		// Use EnTT hashed string to index desired prototype.
		entt::entity build(std::size_t);
		bool copyEntity(entt::entity from, entt::entity to);

	private:
		std::unordered_map<std::size_t, entt::entity> _prototypes;
		entt::registry& registry;
	};
}



