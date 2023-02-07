#pragma once
#include "../deps/Cereal/external/rapidjson/rapidjson.h"

namespace drft
{
	class EntityFactory
	{
	public:
		EntityFactory();

		// Load entity prototypes from the given file into the given registry.
		bool loadPrototypes(std::string filename, entt::registry& registry);

		// Build a new entity and place into the given registry.
		entt::entity build(std::string name, entt::registry& registry);

	private:
		entt::entity copyEntity(entt::entity from, entt::registry& registry);

	private:
		std::unordered_map<std::string, entt::entity> _prototypes;
	};
}



