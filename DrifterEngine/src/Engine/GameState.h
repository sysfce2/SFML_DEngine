#pragma once
#include "State.h"

namespace drft
{
	namespace system
	{
		class SystemScheduler;
	}

	class GameState : public State
	{
	public:
		GameState(State::Context context);
		virtual void update(const float dt) override;
		virtual void render(sf::RenderTarget& target) override;
		virtual void endState() override;

	private:
		virtual void init() override;
		// Add all systems to the system queue
		void importSystems();

	private:
		bool _hasStarted = false;
		entt::registry _registry;
		std::unique_ptr<system::SystemScheduler> _systems;
	};

} // namespace drft


