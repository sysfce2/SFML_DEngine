#include "pch.h"
#include "State.h"

using namespace drft;

State::State(Context context) : _context(context)
{
	_quit = false;
}

State::~State()
{
}

const bool& State::getQuit() const
{
	return _quit;
}

bool drft::State::processEvent(const sf::Event& ev)
{
	if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
	{
		_quit = true;
		return true;
	}

	return false;
}

drft::State::Context &const drft::State::getContext()
{
	return this->_context;
}
