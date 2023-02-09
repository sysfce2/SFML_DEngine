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

	if (!doc.HasMember("Prototypes") || doc.HasParseError())
	{
		std::cout << filename << " could not be parsed." << std::endl;
		return false;
	}

	// Iterate each prototype
	for (auto& prototype : doc["Prototypes"].GetArray())
	{
		auto entityName = std::string(prototype["Name"].GetString());

		entt::entity entity = registry.create();
		registry.emplace_or_replace<component::Prototype>(entity);
		_prototypes[entityName] = entity;

		if (prototype.HasMember("Inherits"))
		{
			// Copy all inherited values into entity
			for (auto&& base : prototype["Inherits"].GetArray())
			{
				auto baseName = std::string(base.GetString());
				if (!_prototypes.contains(baseName)) continue;
				copyEntity(entity, _prototypes[baseName], registry);
			}
			
		}
		if (prototype.HasMember("Components"))
		{
			// Iterate each component
			for (auto&& component : prototype["Components"].GetArray())
			{
				auto componentName = component["Name"].GetString();

				auto meta = entt::resolve(entt::hashed_string(componentName));

				if (!component.HasMember("Data"))
				{
					// No data, must be a 'tag' component
					meta.func(entt::hashed_string("tag")).invoke(meta, entt::forward_as_meta(registry), entity);
				}
				else
				{
					auto any = meta.construct(entt::forward_as_meta(registry), entity);
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
		}
	}

	return true;
}

entt::handle drft::EntityFactory::build(const std::string& name, entt::registry& registry)
{
	if (!_prototypes.contains(name))
	{
		std::cout << "Prototypes does not contain an entity named " << name << std::endl;
		return entt::handle();
	}
	entt::entity newEntity = registry.create();

	copyEntity(newEntity, _prototypes[name], registry);

	return entt::handle(registry, newEntity);
}

bool drft::EntityFactory::has(const std::string& name) const
{
	return _prototypes.contains(name);
}

// Copies the components from 'from' into 'to'.
void drft::EntityFactory::copyEntity(entt::entity to, entt::entity from, entt::registry& registry)
{
	auto &prototypeStorage = registry.view<component::Prototype>().storage();

	for (auto [id, storage] : registry.storage())
	{
		if (storage.contains(from) && !(storage.type() == prototypeStorage.type()))
		{
			if (storage.contains(to))
			{
				storage.erase(to);
			}
			storage.emplace(to, storage.get(from));
		}
	}
}


