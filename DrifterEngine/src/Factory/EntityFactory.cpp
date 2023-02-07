#include "pch.h"
#include "EntityFactory.h"
#include "../deps/Cereal/external/rapidjson/istreamwrapper.h"
#include "../deps/Cereal/external/rapidjson/document.h"
#include "../deps/Cereal/external/rapidjson/stringbuffer.h"


drft::EntityFactory::EntityFactory(entt::registry& registry)
	: registry(registry)
{
}

bool drft::EntityFactory::loadPrototypes(std::string filename)
{
	using namespace rapidjson;

	std::string filepath = ".\\data\\static\\" + filename;

	std::ifstream ifs{ filepath };
	if (!ifs.is_open())
	{
		std::cerr << "Could not open file " << filepath << std::endl;
		return false;
	}
	IStreamWrapper isw{ ifs };
	Document doc{};
	doc.ParseStream(isw);

	assert(doc.IsObject());
	assert(doc.HasMember("Prototypes"));
	assert(doc["Prototypes"].IsArray());

	// Iterate each prototype
	for (auto& prototype : doc["Prototypes"].GetArray())
	{
		assert(prototype.HasMember("Name"));
		assert(prototype["Name"].IsString());
		auto entityName = prototype["Name"].GetString();

		entt::entity entity = registry.create();

		_prototypes[entt::hashed_string(entityName)] = entity;

		assert(prototype.HasMember("Components"));
		assert(prototype["Components"].IsArray());
		// Iterate each component
		for (auto& component : prototype["Components"].GetArray())
		{
			assert(component.HasMember("Name"));
			assert(component["Name"].IsString());

			auto componentName = component["Name"].GetString();

			auto meta = entt::resolve(entt::hashed_string(componentName));
			auto any = meta.construct(entt::forward_as_meta(registry), entity);

			assert(component.HasMember("Data"));
			assert(component["Data"].IsObject());

			// Iterate component data
			for (auto& data : component["Data"].GetObject())
			{
				if (data.value.IsArray())
				{
					
				}
				else if (data.value.IsString())
				{

				}
				else if (data.value.IsInt())
				{

				}
				else if (data.value.IsFloat())
				{

				}
			}
		}
	}

	return true;
}

entt::entity drft::EntityFactory::build(std::size_t id)
{
	auto newEntity = registry.create();

	copyEntity(_prototypes[id], newEntity);

	return entt::entity();
}

bool drft::EntityFactory::copyEntity(entt::entity from, entt::entity to)
{
	for (auto [id, storage] : registry.storage())
	{
		if (storage.contains(from))
		{
			storage.emplace(to, storage.get(from));
		}
	}

	return true;
}


