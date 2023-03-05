#include "pch.h"
#include "Meta.h"
#include "Serialize.h"
#include "Snapshot/Reflection.h"
#include "../deps/Entt/core/hashed_string.hpp"

using namespace entt::literals;



void component::Meta::initialize()
{
	// Meta types initialized here. Use the current ones as examples to 'meta' new compenents
	
	snapshot::reflectComponent<Info, INFO_STRING>()
		.data<&Info::prototype>("prototype"_hs)
		.data<&Info::name>("name"_hs)
		.data<&Info::extendedName>("extendedName"_hs)
		.data<&Info::description>("description"_hs);

	snapshot::reflectComponent<Position, POSITION_STRING>()
		.data<&Position::position>("position"_hs)
		.data<&Position::depth>("depth"_hs);

	snapshot::reflectComponent<Render, RENDER_STRING>()
		.data<&Render::sprite>("sprite"_hs)
		.data<&Render::color>("color"_hs);

	snapshot::reflectComponent<Actor, ACTOR_STRING>()
		.data<&Actor::ap>("ap"_hs)
		.data<&Actor::moveSpeed>("moveSpeed"_hs)
		.data<&Actor::actSpeed>("actSpeed"_hs);

	snapshot::reflectComponent<Health, HEALTH_STRING>()
		.data<&Health::max>("max"_hs)
		.data<&Health::current>("current"_hs);

	snapshot::reflectComponent<Attacker, ATTACKER_STRING>()
		.data<&Attacker::baseDamage>("baseDamage"_hs);

	snapshot::reflectComponent<AI, AI_STRING>()
		.data<&AI::protocol>("protocol"_hs);
}
