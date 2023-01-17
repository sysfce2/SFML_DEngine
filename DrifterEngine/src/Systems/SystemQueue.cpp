#include "../pch.h"
#include "SystemQueue.h"

void drft::system::SystemQueue::add(std::unique_ptr<System> sys)
{
	auto phase = sys->phase;
	_systems[phase].push_back(std::move(sys));
}

void drft::system::SystemQueue::update(const float dt)
{
	for (int i = 0; i < (int)Phase::AllUpdate; ++i)
	{
		for (auto& sys : _systems[(Phase)i])
		{
			sys->update(dt);
		}
	}
}

void drft::system::SystemQueue::render(sf::RenderTarget& target)
{
	for (auto& sys : _systems[Phase::OnRender])
	{
		sys->render(target);
	}
}
