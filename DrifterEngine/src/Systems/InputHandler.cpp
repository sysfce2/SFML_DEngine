#include "pch.h"
#include "InputHandler.h"
#include "Components/Components.h"

using namespace sf;


void drft::system::InputHandler::init()
{
	// TODO: move to file
	_keyBindings = {
		{ActionType::Wait,				Keyboard::Numpad5},
		{ActionType::MoveNorth,			Keyboard::Numpad8},
		{ActionType::MoveSouth,			Keyboard::Numpad2},
		{ActionType::MoveWest,			Keyboard::Numpad4},
		{ActionType::MoveEast,			Keyboard::Numpad6},
		{ActionType::MoveSouthEast,		Keyboard::Numpad3},
		{ActionType::MoveSouthWest,		Keyboard::Numpad1},
		{ActionType::MoveNorthEast,		Keyboard::Numpad9},
		{ActionType::MoveNorthWest,		Keyboard::Numpad7},
		{ActionType::PickUp,			Keyboard::G},
		{ActionType::Drop,				Keyboard::D},
		{ActionType::Talk,				Keyboard::T},
		{ActionType::Contextual,		Keyboard::Space},
	};
}

void drft::system::InputHandler::update(const float dt)
{
	auto view = registry->view<component::Player>();

	for (auto entity : view)
	{
		for (auto&& [thisAction, key] : _keyBindings)
		{
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
				if (_keyState[thisAction].timeHeld > _holdTime)
				{
					_keyState[thisAction].timeHeld = _holdTime;
				}
			}
			else
			{
				_keyState[thisAction].timeHeld = 0.0f;
				_keyState[thisAction].active = false;
			}

			if (_keyState[thisAction].active)
			{
				if (registry->any_of<component::Input>(entity)) break;
				registry->emplace<component::Input>(entity, thisAction);
			}
		}
	}
}
