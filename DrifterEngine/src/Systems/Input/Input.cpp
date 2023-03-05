#include "pch.h"
#include "Input.h"
#include "Components/Components.h"
#include "Components/Tags.h"
#include "Actions/ActionBuffer.h"
#include "Actions/Wait.h"
#include "Actions/Move.h"

std::unique_ptr<drft::action::Action> drft::system::input::getInput(entt::handle& entity)
{
	std::unique_ptr<action::Action> result = nullptr;

	if (entity.all_of<component::Player>())
	{
		auto& player = entity.get<component::Player>();
		result = std::move(player.actionBuffer.pop());
	}
	else if (entity.all_of<component::AI>())
	{
		auto& ai = entity.get<component::AI>();
		
		int randx = (std::rand() % 3) - 1;
		int randy = (std::rand() % 3) - 1;

		result = std::make_unique<action::Move>(sf::Vector2i(randx, randy));
	}

	return result;
}
