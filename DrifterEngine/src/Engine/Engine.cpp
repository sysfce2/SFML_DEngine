#include "pch.h"
#include "Engine.h"
#include "GameState.h"
#include "Services/DebugInfo.h"

using namespace drft;
constexpr float TARGET_FPS = 60.0f;

drft::Engine::Engine()
	: _window(sf::VideoMode(960, 540), "Drifter Engine")
	, _showDebug(false)
{
	initialize();
}

void drft::Engine::run()
{
	sf::Clock clock;
	while (_window.isOpen())
	{
		sf::Time deltaTime = clock.restart();
		processEvents();
		update(deltaTime.asSeconds());
		render(deltaTime.asSeconds());
	}
}

void drft::Engine::initialize()
{
	loadResources();
	service::DebugInfo::instance().setFont(_fonts.get("Terminus"));
	service::DebugInfo::instance().setPosition({ 16,8 });
	_states.push(std::make_unique<GameState>(State::Context{ _textures, _fonts, {960, 540}}));
}

void drft::Engine::loadResources()
{
	std::string resourcePath("./resources/");
	_textures.load("Sprites", resourcePath + "Textures/simpleTileset.png");
	_fonts.load("Terminus", resourcePath + "Fonts/terminus.ttf");
}

void drft::Engine::processEvents()
{
	sf::Event event;
	while (_window.pollEvent(event))
	{

		if (!_states.empty() && _states.top()->processEvent(event)) continue;

		switch (event.type)
		{
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::F5)
			{
				_showDebug = !_showDebug;
			}
			break;
		case sf::Event::KeyReleased:

			break;

		case sf::Event::Closed:
			this->shutDown();
			break;
		}
	}
}

void drft::Engine::update(const float dt)
{
	if (!_states.empty())
	{
		float fps = 1.0f / dt;

		service::DebugInfo::instance().putInfo("FPS", std::to_string(fps));
		service::DebugInfo::instance().putInfo("dt", std::to_string(dt));

		_states.top()->update(dt);
		if (_states.top()->getQuit())
		{
			_states.top()->endState();
			_states.pop();
		}
	}
	else
	{
		this->shutDown();
	}
	
	
}

void drft::Engine::render(const float dt)
{
	static float dtSinceRender = 0.0f;

	dtSinceRender += dt;
	if (dtSinceRender >= (1.0f / TARGET_FPS))
	{
		_window.clear();
		if (!_states.empty())
		{
			_states.top()->render(_window);
		}
		if (_showDebug)
		{
			service::DebugInfo::instance().render(_window);
		}
			
		_window.display();
		dtSinceRender = 0.0f;
	}
	
}

void drft::Engine::shutDown()
{
	std::cout << "Closing Engine" << std::endl;
	_window.close();
}




