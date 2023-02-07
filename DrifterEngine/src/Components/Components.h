#pragma once
#include "Actions/Action.h"


namespace component
{
	struct Info
	{
		std::string name = "";
		std::string extendedName = "";
		std::string description = "";
	};

	// Position in world space.
	struct Position
	{
		sf::Vector2f position = { 0,0 };
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
		sf::FloatRect viewport = { 0,0,0,0 };
		entt::entity target = entt::null;
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

} // namespace component