#include "pch.h"
#include "Input.h"
#include "Components/Components.h"
#include "Components/Tags.h"
#include "Actions/ActionBuffer.h"
#include "Actions/Wait.h"

std::unique_ptr<drft::action::Action> drft::system::input::getInput(entt::handle& entity)
{
	std::unique_ptr<action::Action> result;

	if (entity.all_of<component::Player>())
	{
		auto& player = entity.get<component::Player>();
		result = std::move(player.actionBuffer.pop());
	}
	else if (entity.all_of<component::AI>())
	{
		auto& ai = entity.get<component::AI>();
		//result = ArtificialInput::runProtocol(ai.protocol);
		//std::cout << "AI protocol: " << ai.protocol << std::endl;
		result = std::make_unique<action::Wait>();
	}

	return result;
}
