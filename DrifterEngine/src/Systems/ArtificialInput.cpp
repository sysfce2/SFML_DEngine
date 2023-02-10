#include "pch.h"
#include "ArtificialInput.h"
#include "Components/Components.h"
#include "Actions/Move.h"

void drft::system::ArtificialInput::init()
{
	sf::Time time;
	srand(static_cast<unsigned int>(time.asMilliseconds()));
}

void drft::system::ArtificialInput::update(const float dt)
{
	auto view = registry->view<component::AI, component::MyTurn>(entt::exclude<component::Prototype>);

	for (auto ai : view)
	{
		int randx = (rand() % 3) - 1;
		int randy = (rand() % 3) - 1;
		registry->emplace<component::Input>(ai, std::make_unique<action::Move>(sf::Vector2i(randx, randy)));
	}
}
