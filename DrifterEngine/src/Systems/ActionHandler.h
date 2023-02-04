#pragma once
#include "System.h"
#include "Actions/Actions.h"

namespace drft::system
{
	class ActionHandler : public System
	{
	public:
		ActionHandler()
		{
			phase = Phase::OnUpdate;
		}

		virtual void init() override;
		virtual void update(const float dt) override;

	private:
		// Performs the action, returning the fallback action in case of failure.
		drft::action::ActionResult performAction(entt::entity, action::ActionResult action);
	};
}



