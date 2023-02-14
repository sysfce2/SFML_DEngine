#pragma once
#include "State.h"

namespace drft
{
	class EntityFactory;

	namespace system
	{
		class SystemScheduler;
	}

	namespace system::input
	{
		class InputBuffer;
	}

	namespace spatial
	{
		class WorldGrid;
	}

	class GameState : public State
	{
	public:
		GameState(State::Context context);
		virtual void update(const float dt) override;
		virtual void render(sf::RenderTarget& target) override;
		virtual void endState() override;

	private:
		// Used to setup observers
		virtual void init() override;
		// Add all systems to the system queue
		void importSystems();

	private:
		bool _hasStarted = false;
		entt::registry _registry;
		std::unique_ptr<system::SystemScheduler> _systems;
		std::unique_ptr<spatial::WorldGrid> _world;
		std::unique_ptr<EntityFactory> _factory;
		std::unique_ptr<entt::dispatcher> _dispatcher;
	};

} // namespace drft


