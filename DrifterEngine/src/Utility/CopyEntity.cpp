#include "pch.h"
#include "CopyEntity.h"
#include "Components/Components.h"
#include "Components/Meta.h"

using namespace entt::literals;

void drft::util::copyEntity(entt::entity to, entt::entity from, entt::registry& registry)
{
	auto& prototypeStorage = registry.view<component::Prototype>().storage();

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

void drft::util::copyEntity(entt::entity to, entt::entity from, entt::registry& toRegistry, entt::registry& fromRegistry)
{
	auto& prototypeStorage = fromRegistry.view<component::Prototype>().storage();

	for (auto [id, storage] : fromRegistry.storage())
	{
		if (storage.contains(from) && storage.type() != prototypeStorage.type())
		{
			auto toStorage = toRegistry.storage(id);
			if (!toStorage)
			{
				// Some jank to add this type to the registry if it didn't exist
				auto meta = entt::resolve(storage.type());
				meta.func("emplace"_hs).invoke({}, entt::forward_as_meta(toRegistry), to);
				toStorage = toRegistry.storage(id);
				toStorage->remove(to);	
			}
			assert(toStorage != nullptr);
			toStorage->emplace(to, storage.get(from));
		}
	}
}

