#pragma once
#include "Actions/Action.h"


namespace component
{
	struct Prototype {};

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

	struct Actor
	{
		int ap = 0;
		float moveSpeed = 1.0f;
		float actSpeed = 1.0f;
	};

	struct MyTurn
	{};

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