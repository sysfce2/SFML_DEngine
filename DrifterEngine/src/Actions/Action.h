#pragma once

namespace drft::action
{
	struct Action
	{
		// Returns a fallback action if this action cannot be executed.
		virtual std::unique_ptr<Action> execute(entt::handle& entity) = 0;
		virtual float getCost() = 0;
		virtual std::unique_ptr<Action> clone() const = 0;
	};

	// Free functions
	
	// Attempt to perform an action, returning the cost of the action.
	// Will perform fall back action(s) if the given action cannot be performed.
	float attempt(entt::handle entity, std::unique_ptr<Action> action);
}



