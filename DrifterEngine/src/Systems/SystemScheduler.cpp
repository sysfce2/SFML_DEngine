#include "pch.h"
#include "SystemScheduler.h"

void drft::system::SystemScheduler::initAll() const
{
	std::cout << "Initializing Systems..." << std::endl;
	for (auto& [phase, systems] : _systems)
	{
		for (auto &sys : systems)
		{
			sys->init();
		}
	}
}

void drft::system::SystemScheduler::update(const float dt) const
{
	for (unsigned int i = (int)Phase::OnProcessInput; i < (int)Phase::AllUpdate; ++i)
	{
		for (auto& sys : _systems.at(static_cast<Phase>(i)))
		{
			sys->update(dt);
		}
	}
}

void drft::system::SystemScheduler::render(sf::RenderTarget& target) const
{
	for (auto& sys : _systems.at(Phase::OnRender))
	{
		sys->render(target);
	}
}
