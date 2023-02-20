#include "pch.h"
#include "ActionBuffer.h"
#include "Actions/Action.h"

using namespace drft::action;

ActionBuffer::ActionBuffer(const ActionBuffer& other)
{
	for (auto& action : other._buffer)
	{
		this->push(action->clone());
	}
}

ActionBuffer& ActionBuffer::operator=(const ActionBuffer& other)
{
	for (auto& action : other._buffer)
	{
		this->push(action->clone());
	}

	return *this;
}

void ActionBuffer::push(std::unique_ptr<Action> desiredAction)
{
	if (_buffer.size() >= MAX_SIZE)
	{
		this->pop();
	}
	_buffer.push_back(std::move(desiredAction));
}

std::unique_ptr<Action> ActionBuffer::pop()
{
	if (_buffer.empty()) return nullptr;
	auto result = std::move(_buffer.front());
	_buffer.pop_front();
	return std::move(result);
}
