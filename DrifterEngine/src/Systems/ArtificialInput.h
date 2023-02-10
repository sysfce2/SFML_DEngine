#pragma once
#include "System.h"

namespace drft::system
{
	class ArtificialInput : public System
	{
	public:
		ArtificialInput()
		{
			phase = Phase::OnProcessInput;
		}

		virtual void init() override;
		virtual void update(const float dt) override;

	private:

	};

}


