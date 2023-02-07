#pragma once
#include "Action.h"

namespace drft::action
{
	struct Attack : public Action
	{
		Attack(std::vector<entt::entity> targets)
			: potentialTargets(targets) {}

		std::unique_ptr<Action> execute(entt::registry& registry, const entt::entity actor) override;
		std::unique_ptr<Action> clone() const override;

	private:
		std::vector<entt::entity> potentialTargets;;
	};
}



