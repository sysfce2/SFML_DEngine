#pragma once
#include "Actions/Action.h"


namespace component
{
	// Position in world space.
	struct Position
	{
		sf::Vector2f position;
		int depth = 0;
	};

	// Sprite and color.
	struct Render
	{
		unsigned int sprite = 0;
		sf::Color color = sf::Color::Magenta;
	};

	struct Camera
	{
		sf::FloatRect viewport;
		entt::entity target;
	};

	// Represents a player
	struct Player
	{};

	// Represents an AI
	struct AI
	{};

	// Desired input for the turn
	struct Input
	{
		std::unique_ptr<drft::action::Action> desiredAction;
	};

	struct OnScreen {};

} // namespace component