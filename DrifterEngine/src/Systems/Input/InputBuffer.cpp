#include "pch.h"
#include "InputBuffer.h"
#include "Actions/Action.h"

using namespace drft::system::input;

void InputBuffer::push(std::unique_ptr<action::Action> desiredAction)
{
	if (_buffer.size() >= MAX_SIZE)
	{
		this->pop();
	}
	_buffer.push(std::move(desiredAction));
}

std::unique_ptr<drft::action::Action> InputBuffer::pop()
{
	if (_buffer.empty()) return nullptr;
	auto result = std::move(_buffer.front());
	_buffer.pop();
	return std::move(result);
}
