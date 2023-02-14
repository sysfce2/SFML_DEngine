#pragma once
#include "Action.h"

namespace drft::action
{
	struct Move : public Action
	{
		Move(sf::Vector2i direction)
			: direction(direction) {}

		std::unique_ptr<Action> execute(entt::handle& entity) override;
		virtual float getCost() override
		{
			return 100.0f;
		}
		std::unique_ptr<Action> clone() const override;

	private:
		sf::Vector2i direction;
	};

}

