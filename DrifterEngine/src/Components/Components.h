#pragma once


namespace component
{
	// Position in world space.
	struct Position
	{
		float x = 0;
		float y = 0;
		int depth = 0;
	};

	// Sprite and color.
	struct Render
	{
		unsigned int sprite = 0;
		sf::Color color = sf::Color::Magenta;
	};

	// Change of position in world space.
	struct Motion
	{
		float dx = 0;
		float dy = 0;
	};

	// 
	struct PlayerInput
	{
		int desiredAction = 0;
	};

	struct OnScreen {};

} // namespace component