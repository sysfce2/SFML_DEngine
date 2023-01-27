#include "pch.h"
#include "InputHandler.h"
#include "Components/Components.h"

using namespace sf;


void drft::system::InputHandler::init()
{
	// maybe read input mapping from file??
	_keyBindings = {
		{action::Type::Wait,				Keyboard::Numpad5},
		{action::Type::MoveNorth,			Keyboard::Numpad8},
		{action::Type::MoveSouth,			Keyboard::Numpad2},
		{action::Type::MoveWest,			Keyboard::Numpad4},
		{action::Type::MoveEast,			Keyboard::Numpad6},
		{action::Type::MoveSouthEast,		Keyboard::Numpad3},
		{action::Type::MoveSouthWest,		Keyboard::Numpad1},
		{action::Type::MoveNorthEast,		Keyboard::Numpad9},
		{action::Type::MoveNorthWest,		Keyboard::Numpad7},
		{action::Type::PickUp,				Keyboard::G},
		{action::Type::Drop,				Keyboard::D},
		{action::Type::Talk,				Keyboard::T},
		{action::Type::Contextual,			Keyboard::Space},
	};
}

void drft::system::InputHandler::update(const float dt)
{
	auto view = registry.view<component::PlayerInput>();

	for (auto [entity, input] : view.each())
	{
		input.desiredAction = action::Type::None;
		for (int k = 1; k < action::Type::Total; ++k)
		{
			action::Type thisAction = (action::Type)k;

			if (Keyboard::isKeyPressed(_keyBindings[thisAction]))
			{
				_keyState[thisAction].active = false;
				if (_keyState[thisAction].timeHeld <= std::numeric_limits<float>::epsilon())
				{
					// Just pressed
					_keyState[thisAction].active = true;
					_keyState[thisAction].timeHeld = 0.0f;
				}
				else if (_keyState[thisAction].timeHeld >= _holdTime)
				{
					// Held key long enough
					_keyState[thisAction].active = true;
					_keyState[thisAction].timeHeld -= _refractoryPeriod;
				}
				
				_keyState[thisAction].timeHeld += dt;
			}
			else
			{
				_keyState[thisAction].timeHeld = 0.0f;
				_keyState[thisAction].active = false;
			}

			if (_keyState[thisAction].active)
			{
				if (input.desiredAction == action::Type::None)
				{
					input.desiredAction = thisAction;
				}
			}
		}
	}
}
