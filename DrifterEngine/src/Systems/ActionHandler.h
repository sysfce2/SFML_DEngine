#pragma once
#include "System.h"
#include "Actions/Action.h"

namespace drft::system
{
	struct EntityActionPair
	{
		entt::entity entity;
		std::unique_ptr<action::Action> action = nullptr;
	};

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
		std::queue<EntityActionPair> _actionQueue;
	};
}



