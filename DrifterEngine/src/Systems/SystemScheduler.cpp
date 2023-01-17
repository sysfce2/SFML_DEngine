#include "../pch.h"
#include "SystemScheduler.h"

void drft::system::SystemScheduler::initAll()
{
	for (auto &phase : _systems)
	{
		for (auto &sys : phase.second)
		{
			sys->init();
		}
	}
}

void drft::system::SystemScheduler::add(std::unique_ptr<System> sys)
{
	auto phase = sys->phase;
	_systems[phase].push_back(std::move(sys));
}

void drft::system::SystemScheduler::update(const float dt)
{
	for (int i = 0; i < (int)Phase::AllUpdate; ++i)
	{
		for (auto& sys : _systems[(Phase)i])
		{
			sys->update(dt);
		}
	}
}

void drft::system::SystemScheduler::render(sf::RenderTarget& target)
{
	for (auto& sys : _systems[Phase::OnRender])
	{
		sys->render(target);
	}
}
