#pragma once

namespace drft::action
{
	enum class Type
	{
		MOVE,
		ACT
	};

	struct Action
	{
		// Returns a fallback action if this action cannot be executed.
		virtual std::unique_ptr<Action> execute(entt::registry& registry, const entt::entity actor) = 0;
		virtual float getCost() = 0;
		virtual Type getType() = 0;
		virtual std::unique_ptr<Action> clone() const = 0;
	};
}



