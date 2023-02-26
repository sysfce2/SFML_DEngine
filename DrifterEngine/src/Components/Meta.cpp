#include "pch.h"
#include "Meta.h"
#include "Serialize.h"
#include "Snapshot/Reflection.h"
#include "../deps/Entt/core/hashed_string.hpp"

using namespace entt::literals;

void component::Meta::initialize()
{
	// Meta types initialized here. Use the current ones as examples to 'meta' new compenents

	
	entt::meta<Info>().type("Info"_hs)
		.func<&entt::registry::emplace_or_replace<Info>, entt::as_ref_t>("emplace"_hs)
		.data<&Info::prototype>("prototype"_hs)
		.data<&Info::name>("name"_hs)
		.data<&Info::extendedName>("extendedName"_hs)
		.data<&Info::description>("description"_hs)
		.prop("serialize"_hs);

	entt::meta<Position>().type("Position"_hs)
		.func<&entt::registry::emplace_or_replace<Position>, entt::as_ref_t>("emplace"_hs)
		.data<&Position::position>("position"_hs)
		.data<&Position::depth>("depth"_hs)
		.prop("serialize"_hs);

	entt::meta<Render>().type("Render"_hs)
		.func<&entt::registry::emplace_or_replace<Render>, entt::as_ref_t>("emplace"_hs)
		.data<&Render::sprite>("sprite"_hs)
		.data<&Render::color>("color"_hs)
		.prop("serialize"_hs);

	entt::meta<Actor>().type("Actor"_hs)
		.func<&entt::registry::emplace_or_replace<Actor>, entt::as_ref_t>("emplace"_hs)
		.data<&Actor::ap>("ap"_hs)
		.data<&Actor::moveSpeed>("moveSpeed"_hs)
		.data<&Actor::actSpeed>("actSpeed"_hs)
		.prop("serialize"_hs);

	entt::meta<Health>().type("Health"_hs)
		.func<&entt::registry::emplace_or_replace<Health>, entt::as_ref_t>("emplace"_hs)
		.data<&Health::max>("max"_hs)
		.data<&Health::current>("current"_hs)
		.prop("serialize"_hs);

	entt::meta<AI>().type("AI"_hs)
		.func<&entt::registry::emplace_or_replace<AI>, entt::as_ref_t>("emplace"_hs)
		.data<&AI::protocol>("protocol"_hs)
		.prop("serialize"_hs);
}
