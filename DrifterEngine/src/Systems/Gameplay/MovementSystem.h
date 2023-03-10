#pragma	once
#include "Systems/System.h"

namespace drft::system
{
	class MovementSystem : public System
	{
	public:
		MovementSystem()
		{
			phase = Phase::OnUpdate;
		}
		void init() override;
		void update(const float dt) override;
	};
}