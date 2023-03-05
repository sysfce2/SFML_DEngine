#pragma once
#include "Systems/System.h"


namespace component
{
	struct Actor;
}

namespace drft::system
{
	const int AP_PER_TICK = 100;

	class ActorQueue
	{
	public:
		ActorQueue(entt::registry& registry);
		void refresh(std::set<entt::entity>& currentEntities);
		void rotate();
		void tick();

		void setSentinel(entt::entity sentinel);
		component::Actor& getCurrentActor() const;
		void remove(entt::entity);
	private:
		entt::registry& registry;
		entt::entity _sentinel;
		std::vector<entt::entity> _queue;
	};

	class TurnManager : public System
	{
	public:
		TurnManager()
			: _timeKeeper(entt::null)
		{
			phase = Phase::OnUpdate;
		}

		virtual void init() override;
		virtual void update(const float dt) override;

	private:
		void onActorRemove(entt::registry& registry, entt::entity entity);
		float act(component::Actor& actor);

	private:
		std::unique_ptr<ActorQueue> _actorQueue;
		std::set<entt::entity> _managedEntities;
		entt::entity _timeKeeper;
	};

	
}


