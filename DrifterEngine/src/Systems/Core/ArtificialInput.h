#pragma once
#include "Systems/System.h"

namespace drft::system
{
	class ArtificialInput : public System
	{
	public:
		void init() override;
		void update(const float dt) override;

	private:
		entt::entity findTarget(entt::handle entity) const;
		void randomMove(entt::handle entity) const;
	};
}

