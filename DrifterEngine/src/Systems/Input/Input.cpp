#include "pch.h"
#include "Input.h"
#include "Components/Components.h"
#include "InputBuffer.h"
#include "Actions/Action.h"
#include "Actions/Wait.h"

std::unique_ptr<drft::action::Action> drft::system::input::getInput(entt::handle& entity)
{
	std::unique_ptr<action::Action> result;

	if (entity.all_of<component::Player>())
	{
		result = std::move(InputBuffer::getInstance().pop());
	}
	else if (entity.all_of<component::AI>())
	{
		auto& ai = entity.get<component::AI>();
		//result = ArtificialInput::runProtocol(ai.protocol);
		std::cout << "AI protocol: " << ai.protocol << std::endl;
		result = std::make_unique<action::Wait>();
	}

	return result;
}
