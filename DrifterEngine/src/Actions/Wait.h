#pragma once
#include "Action.h"

namespace drft::action
{
	class Wait : public Action
	{
	public:
		virtual std::unique_ptr<Action> execute(entt::handle& entity) override;
		virtual float getCost() override;
		virtual std::unique_ptr<Action> clone() const override;
	};

}

