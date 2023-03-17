#pragma once

namespace component
{
	struct Prototype 
	{};

	struct Info
	{
		std::string prototype = { "" };
		std::string name = { "" };
		std::string extendedName = { "" };
		std::string description = { "" };
	};

	// Position in world space.
	struct Position
	{
		sf::Vector2f position = { 0,0 };
		int depth = { 0 };
	};

	// Sprite and color.
	struct Render
	{
		unsigned int sprite = { 0 };
		sf::Color color = { sf::Color::Magenta };
	};

	struct Camera
	{
		sf::FloatRect viewport = { 0,0,0,0 };
		entt::entity target = { entt::null };
	};

	struct Actor
	{
		int ap = { 0 };
		float moveSpeed = { 1.0f };
		float actSpeed = { 1.0f };
	};

	struct Health
	{
		int max = 10;
		int current = 10;
	};

	struct Attacker
	{
		int baseDamage = 1;
	};

	// Represents a player
	struct Player
	{};

	// Represents an AI
	struct AI
	{
		std::string protocol = { "random" };
	};

}

namespace component::action
{
	struct Move
	{
		sf::Vector2i direction = { 0,0 };
	};

	struct LaunchAttack
	{
		sf::Vector2i direction = { 0,0 };
		int damage = 0;
	};

	struct Wait
	{};

	struct TakeDamage
	{
		int amount = 0;
	};

	struct SpendPoints
	{
		int amount = 0;
	};
}