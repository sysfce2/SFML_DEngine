#include "pch.h"
#include "SerializeEntity.h"
#include "Components/Components.h"
#include "Components/Serialize.h"

using namespace component;

void drft::util::serialize(entt::registry& registry, cereal::BinaryOutputArchive& output)
{
	entt::snapshot{ registry }
		.entities(output)
		.component<Position, Info, Render, Actor, Health, AI>(output);
}

void drft::util::deserialize(entt::registry& registry, cereal::BinaryInputArchive& input)
{
	entt::snapshot_loader{ registry }
		.entities(input)
		.component<Position, Info, Render, Actor, Health, AI>(input);
}

void drft::util::transfer(entt::registry& from, entt::registry& to)
{
	std::stringstream ss;
	{
		cereal::BinaryOutputArchive outArchive{ ss };
		serialize(from, outArchive);
	}
	{
		cereal::BinaryInputArchive inArchive{ ss };
		deserialize(to, inArchive);
	}
}
