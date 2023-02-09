#pragma once

namespace drft::events
{
	struct UpdatePosition
	{
		entt::entity entity;
		sf::Vector2f prevPosition;
		sf::Vector2f newPosition;
		int depth;
	};
}