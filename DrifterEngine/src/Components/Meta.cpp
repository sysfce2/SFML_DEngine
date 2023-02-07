#include "pch.h"
#include "Meta.h"
#include "../deps/Entt/core/hashed_string.hpp"

using namespace entt::literals;

void component::Meta::initialize()
{
	// Meta types initialized here. Use the current ones as examples to 'meta' new compenents

	entt::meta<component::Info>().type("Info"_hs)
		.ctor<&entt::registry::emplace_or_replace<Info>, entt::as_ref_t>()
		.data<&Info::name>("name"_hs)
		.data<&Info::extendedName>("extendedName"_hs)
		.data<&Info::description>("description"_hs);

	entt::meta<component::Position>().type("Position"_hs)
		.ctor<&entt::registry::emplace_or_replace<Position>, entt::as_ref_t>()
		.data<&Position::position>("position"_hs)
		.data<&Position::depth>("depth"_hs);

	entt::meta<component::Render>().type("Render"_hs)
		.ctor<&entt::registry::emplace_or_replace<Render>, entt::as_ref_t>()
		.data<&Render::sprite>("sprite"_hs)
		.data<&Render::color>("color"_hs);

	entt::meta<component::Player>().type("Player"_hs)
		.func<&entt::registry::emplace_or_replace<Player>>("tag"_hs);

	entt::meta<component::AI>().type("AI"_hs)
		.func<&entt::registry::emplace_or_replace<AI>>("tag"_hs);
}
