#pragma once
#include "System.h"
#include "Actions/ActionTypes.h"


namespace drft::system
{
	struct KeyState
	{
		float timeHeld = 0;
		bool active = false;
	};

	class InputHandler : public System
	{
	public:
		InputHandler(entt::registry& registry)
			: System(registry)
		{
			phase = Phase::OnProcessInput;
		}

		virtual void init() override;
		virtual void update(const float dt) override;

	private:
		std::unordered_map<ActionType, sf::Keyboard::Key> _keyBindings;
		std::unordered_map<ActionType, KeyState> _keyState;
		float _refractoryPeriod = 0.05f; // sec
		float _holdTime = 0.4f; // sec
	};

}


