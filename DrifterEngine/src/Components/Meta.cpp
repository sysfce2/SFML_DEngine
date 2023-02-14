#include "pch.h"
#include "Meta.h"
#include "../deps/Entt/core/hashed_string.hpp"

using namespace entt::literals;

void component::Meta::initialize()
{
	// Meta types initialized here. Use the current ones as examples to 'meta' new compenents

	entt::meta<Info>().type("Info"_hs)
		.ctor<&entt::registry::emplace_or_replace<Info>, entt::as_ref_t>()
		.data<&Info::name>("name"_hs)
		.data<&Info::extendedName>("extendedName"_hs)
		.data<&Info::description>("description"_hs);

	entt::meta<Position>().type("Position"_hs)
		.ctor<&entt::registry::emplace_or_replace<Position>, entt::as_ref_t>()
		.data<&Position::position>("position"_hs)
		.data<&Position::depth>("depth"_hs);

	entt::meta<Render>().type("Render"_hs)
		.ctor<&entt::registry::emplace_or_replace<Render>, entt::as_ref_t>()
		.data<&Render::sprite>("sprite"_hs)
		.data<&Render::color>("color"_hs);

	entt::meta<Actor>().type("Actor"_hs)
		.ctor<&entt::registry::emplace_or_replace<Actor>, entt::as_ref_t>()
		.data<&Actor::ap>("ap"_hs)
		.data<&Actor::moveSpeed>("moveSpeed"_hs)
		.data<&Actor::actSpeed>("actSpeed"_hs);

	entt::meta<Player>().type("Player"_hs)
		.func<&entt::registry::emplace_or_replace<Player>>("tag"_hs);

	entt::meta<AI>().type("AI"_hs)
		.ctor<&entt::registry::emplace_or_replace<AI>, entt::as_ref_t>()
		.data<&AI::protocol>("protocol"_hs);
}
