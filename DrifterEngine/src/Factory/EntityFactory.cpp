#include "pch.h"
#include "EntityFactory.h"
#include "Components/Meta.h"
#include "../deps/Cereal/external/rapidjson/istreamwrapper.h"
#include "../deps/Cereal/external/rapidjson/document.h"
#include "../deps/Cereal/external/rapidjson/stringbuffer.h"


drft::EntityFactory::EntityFactory()
{
	component::Meta::initialize();
}

bool drft::EntityFactory::loadPrototypes(std::string filename, entt::registry& registry)
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
		auto entityName = std::string(prototype["Name"].GetString());

		entt::entity entity = registry.create();

		_prototypes[entityName] = entity;

		assert(prototype.HasMember("Components"));
		assert(prototype["Components"].IsArray());

		// Iterate each component
		for (auto&& component : prototype["Components"].GetArray())
		{
			assert(component.HasMember("Name"));
			assert(component["Name"].IsString());
			auto componentName = component["Name"].GetString();

			auto meta = entt::resolve(entt::hashed_string(componentName));
			auto any = meta.construct(entt::forward_as_meta(registry), entity);

			assert(component.HasMember("Data"));
			assert(component["Data"].IsObject());

			// Iterate component data
			for (auto&& data : component["Data"].GetObject())
			{
				auto memberName = data.name.GetString();
				if (data.value.IsArray())
				{
					// HACKZZ: Assumes only Vector2f or Color data types represented by array
					auto arr = data.value.GetArray();
					int size = arr.Size();
					if (size == 2)
					{
						sf::Vector2f vec2 = { arr[0].GetFloat(), arr[1].GetFloat() };
						meta.data(entt::hashed_string(memberName)).set(any, vec2);
					}
					else if (size == 3)
					{
						
						sf::Color col = { 
							static_cast<sf::Uint8>(arr[0].GetInt()), 
							static_cast<sf::Uint8>(arr[1].GetInt()),
							static_cast<sf::Uint8>(arr[2].GetInt())
						};
						meta.data(entt::hashed_string(memberName)).set(any, col);
						
					}
				}
				else if (data.value.IsString())
				{
					std::string val(data.value.GetString());
					meta.data(entt::hashed_string(memberName)).set(any, val);
				}
				else if (data.value.IsInt())
				{
					meta.data(entt::hashed_string(memberName)).set(any, data.value.GetInt());
				}
				else if (data.value.IsFloat())
				{
					meta.data(entt::hashed_string(memberName)).set(any, data.value.GetFloat());
				}
			}
		}
	}

	return true;
}

entt::entity drft::EntityFactory::build(std::string name, entt::registry& registry)
{
	auto newEntity = copyEntity(_prototypes[name], registry);

	return newEntity;
}

entt::entity drft::EntityFactory::copyEntity(entt::entity from, entt::registry& registry)
{
	entt::entity newEntity = registry.create();
	for (auto [id, storage] : registry.storage())
	{
		if (storage.contains(from))
		{
			storage.emplace(newEntity, storage.get(from));
		}
	}

	return newEntity;
}


