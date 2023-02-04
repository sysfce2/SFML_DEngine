#pragma once

namespace drft::action
{
	struct Action
	{
		// Returns an optional action if this action cannot be executed.
		virtual std::unique_ptr<Action> execute(entt::registry& registry, const entt::entity actor) = 0;
		virtual std::unique_ptr<Action> clone() const = 0;
	};
}



