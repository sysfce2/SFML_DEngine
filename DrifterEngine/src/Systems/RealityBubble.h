#pragma once
#include "System.h"

namespace drft::system
{
	const int REALITY_RADIUS = 2;

	class RealityBubble : public System
	{
	public:
		RealityBubble()
		{
			phase = Phase::OnPreUpdate;
		}
		virtual void init() override;
		virtual void update(const float) override;

	private:
		void deactivateEntities(std::vector<entt::entity>& entities);
		void activateEntities(std::vector<entt::entity>& entities);

	private:
		sf::Vector2i _cameraPosition;
	};
}



