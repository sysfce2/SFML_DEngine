#pragma once
#include "System.h"
#include "Spatial/WorldGrid.h"

namespace drft::system
{

	class WorldGridResolver : public System
	{
	public:
		WorldGridResolver(entt::registry& registry, spatial::WorldGrid& world)
			: world(world), System(registry)
		{
			phase = Phase::OnValidation;
		}

		virtual void init() override;

	private:
		void OnPositionAdd(entt::registry& registry, entt::entity);
		void OnPositionRemove(entt::registry& registry, entt::entity);

	private:
		spatial::WorldGrid& world;
	};

} // namespace dft::system


