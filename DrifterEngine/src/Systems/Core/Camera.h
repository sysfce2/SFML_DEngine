#pragma once
#include "Systems/System.h"

namespace drft::system
{
	class Camera : public System
	{
	public:
		virtual void init() override;
		virtual void update(const float dt) override;

	private:
		const float _speed = 5.0f;
	};

}


