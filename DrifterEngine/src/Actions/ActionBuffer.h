#pragma once


namespace drft::action
{
	struct Action;

	class ActionBuffer
	{
	public:
		ActionBuffer() = default;
		ActionBuffer(const ActionBuffer& other);
		ActionBuffer& operator=(const ActionBuffer& other);
		ActionBuffer(const ActionBuffer&& other) = delete;
		ActionBuffer&& operator=(const ActionBuffer&& other) = delete;

		void push(std::unique_ptr<action::Action> desiredAction);
		std::unique_ptr<action::Action> pop();

	private:
		const int MAX_SIZE = 1;
		std::list< std::unique_ptr<action::Action> > _buffer;
	};
}



