#pragma once
#include "System.h"
#include "Components/Components.h"

namespace drft::events
{
	struct SpendActionPoints;
}

namespace drft::system
{
	const int AP_PER_TICK = 100;

	class TurnManager : public System
	{
	public:
		TurnManager()
		{
			phase = Phase::OnUpdate;
		}

		virtual void init() override;
		virtual void update(const float dt) override;

	private:
		void onActorRemove(entt::registry& registry, entt::entity entity);
		void onSpendActionPoints(events::SpendActionPoints& ev);
		void updateActorQueue();
		void popFrontPushBack();
		void sortQueue();
		void printQueue();
		void tick();

	private:
		std::vector<entt::entity> _queue;
		std::set<entt::entity> _managedEntities;
		entt::entity _timeKeeper;
		entt::entity _currentActor;
		entt::entity _player;
	};
}


