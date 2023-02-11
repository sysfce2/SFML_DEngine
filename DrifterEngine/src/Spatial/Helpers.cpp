#include "pch.h"
#include "Helpers.h"


std::vector<sf::Vector2i> drft::spatial::getIntPointsInRadius(const sf::Vector2i centerPosition, const int radius)
{
	std::vector<sf::Vector2i> result;

	for (int y = centerPosition.y - radius; y <= centerPosition.y + radius; ++y)
	{
		for (int x = centerPosition.x - radius; x <= centerPosition.x + radius; ++x)
		{
			float distance = std::hypotf((centerPosition.x - x), (centerPosition.y - y));

			if (distance < radius)
			{
				result.push_back({ x,y });
			}
		}
	}

	return result;
}

float drft::spatial::distance(sf::Vector2i pt1, sf::Vector2i pt2)
{
	auto delta = pt1 - pt2;
	return std::hypotf(delta.x, delta.y);
}

float drft::spatial::distance(sf::Vector2f pt1, sf::Vector2f pt2)
{
	auto delta = pt1 - pt2;
	return std::hypotf(delta.x, delta.y);
}
