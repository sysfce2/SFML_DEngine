#pragma once
#include "Action.h"

namespace drft::action
{
	struct Move : public Action
	{
		Move(sf::Vector2i direction)
			: direction(direction) {}

		std::unique_ptr<Action> execute(entt::registry& registry, const entt::entity actor) override;
		virtual float getCost() override
		{
			return 100.0f;
		}
		virtual Type getType() override
		{
			return Type::MOVE;
		}
		std::unique_ptr<Action> clone() const override;

	private:
		sf::Vector2i direction;
	};

}

