#include "pch.h"
#include "Input.h"
#include "Components/Components.h"
#include "InputBuffer.h"
#include "Actions/Action.h"

std::unique_ptr<drft::action::Action> drft::system::input::getInput(entt::handle& entity)
{
	std::unique_ptr<action::Action> result = nullptr;

	if (entity.all_of<component::Player>())
	{
		result = std::move(InputBuffer::getInstance().pop());
	}
	else if (entity.all_of<component::AI>())
	{
		auto& ai = entity.get<component::AI>();
		//result = ArtificialInput::runProtocol(ai.protocol);
	}

	return result;
}
