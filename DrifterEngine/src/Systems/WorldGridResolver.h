#pragma once
#include "System.h"
#include "Spatial/WorldGrid.h"

namespace drft::system
{

	class WorldGridResolver : public System
	{
	public:
		WorldGridResolver()
		{
			phase = Phase::OnValidation;
		}

		virtual void init() override;

	private:
		void OnPositionAdd(entt::registry& registry, entt::entity);
		void OnPositionRemove(entt::registry& registry, entt::entity);
	};

} // namespace dft::system


