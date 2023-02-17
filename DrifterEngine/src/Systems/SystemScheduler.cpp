#include "pch.h"
#include "SystemScheduler.h"

void drft::system::SystemScheduler::initAll()
{
	std::cout << "Initializing Systems..." << std::endl;
	for (auto &phase : _systems)
	{
		for (auto &sys : phase.second)
		{
			sys->init();
		}
	}
}

void drft::system::SystemScheduler::update(const float dt)
{
	for (unsigned int i = (int)Phase::OnProcessInput; i < (int)Phase::AllUpdate; ++i)
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
