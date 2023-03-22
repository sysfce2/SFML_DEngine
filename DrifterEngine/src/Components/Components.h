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

	struct Position
	{
		sf::Vector2f position = { 0,0 };
		int depth = { 0 };
	};

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

	struct Player
	{};

	struct AI
	{
		std::vector<std::string> goals;
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

	struct Die
	{};
}