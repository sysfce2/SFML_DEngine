#pragma once
#include "System.h"
#include "Actions/Action.h"


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
		std::unordered_map<action::Type, sf::Keyboard::Key> _keyBindings;
		std::unordered_map<action::Type, KeyState> _keyState;
		float _refractoryPeriod = 0.15f; // sec
		float _holdTime = 0.75f; // sec
	};

}


