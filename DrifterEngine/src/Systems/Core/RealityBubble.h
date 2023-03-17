#pragma once
#include "Systems/System.h"

namespace drft::system
{
	const int REALITY_RADIUS = 64; // in tiles

	class RealityBubble : public System
	{
	public:
		virtual void init() override;
		virtual void update(const float) override;

	private:
		void onActorAddOrUpdate(entt::registry& registry, entt::entity);
		void onActorRemove(entt::registry& registry, entt::entity);

		void onActiveAdd(entt::registry& registry, entt::entity);
		void onActiveRemove(entt::registry& registry, entt::entity);

	private:
		sf::Vector2i _cameraPosition;
		int _activeActors = 0;
	};
}



