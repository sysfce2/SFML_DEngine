#pragma once
#include "Utility/ResourceHolder.h"
#include "State.h"

namespace drft
{
	

	class Engine
	{
	public:
		Engine();
		void run();

	private:
		void initialize();
		void loadResources();
		void processEvents();
		void update(const float dt);
		void render(const float dt);
		void shutDown();

	private:
		sf::RenderWindow _window;
		ResourceHolder<sf::Texture, std::string> _textures;
		ResourceHolder<sf::Font, std::string> _fonts;
		std::stack<std::unique_ptr<State> > _states;

		bool _showDebug;
	};

} // namespace drft






