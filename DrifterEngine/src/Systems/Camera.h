#pragma once
#include "System.h"

namespace drft::system
{
	class Camera : public System
	{
	public:
		Camera()
		{
			phase = Phase::OnPostUpdate;
		}

		virtual void init() override;
		virtual void update(const float dt) override;

	private:
		const float _speed = 5.0f;
	};

}


