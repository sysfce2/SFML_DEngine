#pragma once
#include "System.h"

namespace drft::action
{
	struct Action;
}

namespace drft::system
{
	struct KeyState
	{
		float timeHeld = 0;
		bool active = false;
	};

	class ActionMap
	{
	public:
		// Adss an action to the map. new in a ptr, don't be scared.
		void addAction(sf::Keyboard::Key key, action::Action* action);
		std::unique_ptr<action::Action> operator[](sf::Keyboard::Key key);
		std::unordered_map<sf::Keyboard::Key, std::unique_ptr<action::Action>>& iterate();
	private:
		std::unordered_map<sf::Keyboard::Key, std::unique_ptr<action::Action>> _map;
	};

	class InputHandler : public System
	{
	public:
		InputHandler()
		{
			phase = Phase::OnProcessInput;
		}

		virtual void init() override;
		virtual void update(const float dt) override;

	private:
		std::unordered_map<sf::Keyboard::Key, KeyState> _keyState;
		ActionMap _actionMap;
		float _refractoryPeriod = 0.05f; // sec
		float _holdTime = 0.4f; // sec
	};

}


