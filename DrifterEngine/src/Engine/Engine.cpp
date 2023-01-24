#include "pch.h"
#include "Engine.h"
#include "GameState.h"

using namespace drft;

drft::Engine::Engine() : _window(sf::VideoMode(960, 540), "Drifter Engine"), _showDebug(false)
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
	_states.push(std::make_unique<GameState>(State::Context{_textures, _fonts}));
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
		if (_states.top()->processEvent(event)) continue;

		switch (event.type)
		{
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::D)
			{
				_showDebug = !_showDebug;
			}
			break;
		case sf::Event::KeyReleased:

			break;
		case sf::Event::Closed:
			_window.close();
			break;
		}
	}
}

void drft::Engine::update(const float dt)
{
	if (!_states.empty())
	{
		float fps = 1.0f / dt;

		_states.top()->update(dt);
		if (_states.top()->getQuit())
		{
			_states.top()->endState();
			_states.pop();
		}
		if (_showDebug)
		{
			debugText.setFont(_fonts.get("Terminus"));
			debugText.setFillColor(sf::Color::Yellow);
			debugText.setPosition({ 16,16 });
			debugText.setCharacterSize(16);
			debugText.setString("fps:    \t" + std::to_string(fps) + "\n" +
								"dt:     \t" + std::to_string(dt) + " s");
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
	if (dtSinceRender >= (1.0f / _renderRate))
	{
		_window.clear();
		if (!_states.empty())
			_states.top()->render(_window);
		if (_showDebug)
			_window.draw(debugText);
		_window.display();
		dtSinceRender = 0.0f;
	}
	
}

void drft::Engine::shutDown()
{
	std::cout << "Closing Engine" << std::endl;
	_window.close();
}




