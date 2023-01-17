#pragma once


namespace component
{
	struct position
	{
		float x = 0;
		float y = 0;
		int depth = 0;
	};

	struct render
	{
		unsigned int sprite = 0;
		sf::Color color = sf::Color::Magenta;
	};

	struct velocity
	{
		float dx = 0;
		float dy = 0;
	};

	struct onScreen {};

} // namespace component