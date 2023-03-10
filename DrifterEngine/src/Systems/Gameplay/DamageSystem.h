#include "Systems/System.h"

namespace drft::system
{
	class DamageSystem : public System
	{
	public:
		DamageSystem()
		{
			phase = Phase::OnPostUpdate;
		}

		void init() override;
		void update(const float dt) override;

	private:
		void kill(entt::handle entity) const;
	};
}