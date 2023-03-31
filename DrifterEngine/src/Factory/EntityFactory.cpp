#include "pch.h"
#include "EntityFactory.h"
#include "Components/Meta.h"
#include "Utility/CopyEntity.h"
#include <EnTT/meta/container.hpp>
#include "../deps/Cereal/external/rapidjson/istreamwrapper.h"
#include "../deps/Cereal/external/rapidjson/document.h"
#include "../deps/Cereal/external/rapidjson/stringbuffer.h"

using namespace entt::literals;

drft::EntityFactory::EntityFactory()
{
	component::Meta::initialize();
}

bool drft::EntityFactory::loadPrototypes(std::string filename, entt::registry& registry)
{
	using namespace rapidjson;

	std::string filepath = ".\\data\\static\\" + filename;

	std::ifstream ifs{ filepath };

	std::cout << "Loading " << filename << "..." << std::endl;

	if (!ifs.is_open())
	{
		std::cerr << "Could not open file " << filepath << std::endl;
		return false;
	}
	IStreamWrapper isw{ ifs };
	Document doc{};
	doc.ParseStream<kParseCommentsFlag>(isw);

	if (!doc.HasMember("Prototypes") || doc.HasParseError())
	{
		std::cout << filename << " could not be parsed." << std::endl;
		return false;
	}

	std::cout << "Parsing " << filename << "..." << std::endl;

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
				util::copyEntity(entity, _prototypes[baseName], registry);
			}
			
		}
		if (prototype.HasMember("Components"))
		{
			// Iterate each component
			for (auto&& component : prototype["Components"].GetArray())
			{
				auto componentName = component["Name"].GetString();

				auto meta = entt::resolve(entt::hashed_string(componentName));
				auto any = meta.func("emplace"_hs).invoke(meta, entt::forward_as_meta(registry), entity);
				
				if (!component.HasMember("Data")) continue;
					
				// Iterate component data
				for (auto&& data : component["Data"].GetObject())
				{
					auto memberName = data.name.GetString();
					if (data.value.IsArray())
					{
						// HACKZZ: Inflexible - assumes certain types in arrays
						auto arr = data.value.GetArray();
						int size = arr.Size();
						if (size == 2 && arr[0].IsFloat())
						{
							sf::Vector2f vec2 = { arr[0].GetFloat(), arr[1].GetFloat() };
							meta.data(entt::hashed_string(memberName)).set(any, vec2);
						}
						else if (size == 3 && arr[0].IsInt())
						{
							sf::Color col = {
								static_cast<sf::Uint8>(arr[0].GetInt()),
								static_cast<sf::Uint8>(arr[1].GetInt()),
								static_cast<sf::Uint8>(arr[2].GetInt())
							};
							meta.data(entt::hashed_string(memberName)).set(any, col);
						}
						else if (arr[0].IsString())
						{
							std::unordered_set<std::string> strings;
							for (int i = 0; i < size; ++i)
							{
								strings.insert(arr[i].GetString());
							}
							if (meta.data(entt::hashed_string(memberName)).type().is_associative_container())
							{
								meta.data(entt::hashed_string(memberName)).set(any, strings);
							}
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

	return true;
}

entt::handle drft::EntityFactory::build(const std::string& name, entt::registry& registry) const
{
	if (!_prototypes.contains(name))
	{
		std::string message = "Entity " + name + " does not exist";
		throw std::invalid_argument(message);
	}
	entt::entity newEntity = registry.create();
	auto info = registry.try_get<component::Info>(newEntity);
	if (info)
	{
		info->prototype = name;
	}
	util::copyEntity(newEntity, _prototypes.at(name), registry);

	return entt::handle(registry, newEntity);
}

bool drft::EntityFactory::has(const std::string& name) const
{
	return _prototypes.contains(name);
}


