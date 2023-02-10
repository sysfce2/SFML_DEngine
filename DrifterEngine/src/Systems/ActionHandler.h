#pragma once
#include "System.h"
#include "Actions/Action.h"

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
	};
}



