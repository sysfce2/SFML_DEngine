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
	for (auto [id, storage] : fromRegistry.storage())
	{
		if (storage.contains(from))
		{
			auto meta = entt::resolve(storage.type());
			if (!meta) continue;
			meta.func("emplace"_hs).invoke(meta, entt::forward_as_meta(toRegistry), to);
			auto toStorage = toRegistry.storage(id);
			toStorage->remove(to);
			toStorage->emplace(to, storage.get(from));
		}
	}
}

