#pragma once
#include "Systems/System.h"

namespace drft::system
{

	class WorldGridResolver : public System
	{
	public:
		virtual void init() override;

	private:
		void onPositionAdd(entt::registry& registry, entt::entity);
		void onPositionUpdate(entt::registry& registry, entt::entity);
		void onPositionRemove(entt::registry& registry, entt::entity);
	};

} // namespace dft::system


