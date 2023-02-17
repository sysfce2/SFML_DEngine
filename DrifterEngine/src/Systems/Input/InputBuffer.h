#pragma once

namespace drft::action
{
	struct Action;
}

namespace drft::system::input
{
	class InputBuffer
	{
	public:
		static InputBuffer& getInstance()
		{
			static InputBuffer instance;
			return instance;
		}

	private:
		InputBuffer() {}

		InputBuffer(InputBuffer const&);
		void operator=(InputBuffer const&);
	public:


		void push(std::unique_ptr<action::Action> desiredAction);
		std::unique_ptr<action::Action> pop();

	private:
		const int MAX_SIZE = 1;
		std::queue< std::unique_ptr<action::Action> > _buffer;
	};
}



