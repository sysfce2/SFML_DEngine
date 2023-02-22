#pragma once
#include "Components.h"

namespace component
{
	template<class Archive>
	void serialize(Archive& archive, Info& info)
	{
		archive(info.prototype, info.name, info.extendedName, info.description);
	}

	template<class Archive>
	void serialize(Archive& archive, Position& pos)
	{
		archive(pos.position.x, pos.position.y, pos.depth);
	}

	template<class Archive>
	void serialize(Archive& archive, Render& render)
	{
		archive(render.sprite, render.color.r, render.color.g, render.color.b);
	}

	template<class Archive>
	void serialize(Archive& archive, Actor& actor)
	{
		archive(actor.ap, actor.moveSpeed, actor.actSpeed);
	}

	template<class Archive>
	void serialize(Archive& archive, Health& health)
	{
		archive(health.max, health.current);
	}

	template<class Archive>
	void serialize(Archive& archive, AI& ai)
	{
		archive(ai.protocol);
	}
}



