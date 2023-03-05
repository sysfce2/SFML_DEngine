#pragma once
#include "Action.h"

namespace drft::action
{
	struct Attack : public Action
	{
		Attack(std::vector<entt::entity> targets)
			: potentialTargets(targets) {}

		std::unique_ptr<Action> execute(entt::handle& entity) override;
		virtual float getCost() override
		{
			return 100.0f;
		}
		std::unique_ptr<Action> clone() const override;

	private:
		float calculateDamage(entt::handle& entity) const;

	private:
		std::vector<entt::entity> potentialTargets;
	};
}



