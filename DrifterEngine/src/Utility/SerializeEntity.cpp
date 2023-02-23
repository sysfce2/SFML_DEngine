#include "pch.h"
#include "SerializeEntity.h"
#include "CopyEntity.h"
#include "Components/Components.h"
#include "Components/Serialize.h"

using namespace component;

void drft::util::serialize(entt::registry& registry, cereal::BinaryOutputArchive& output)
{
	
}

void drft::util::deserialize(entt::registry& registry, cereal::BinaryInputArchive& input)
{
	entt::snapshot_loader{ registry }
		.entities(input)
		.component<Position, Info, Render, Actor, Health, AI>(input);
}

void drft::util::transfer(entt::registry& from, entt::registry& to)
{
	from.each([&](const entt::entity fromEntity)
		{
			auto toEntity = to.create();
			copyEntity(toEntity, fromEntity, to, from);
		}
	);
}
