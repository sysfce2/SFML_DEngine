#include "../pch.h"
#include "GameState.h"
#include "../Systems/SystemScheduler.h"
#include "../Systems/Renderer.h"
#include "../Components/Components.h"
#include "../Utility/TestEntities.h"


drft::GameState::GameState(State::Context context) : State(context)
{
	init();
}

void drft::GameState::update(const float dt)
{
	if (!_hasStarted)
	{
		std::cout << "Starting Gamestate" << std::endl;
		_hasStarted = true;
	}
	_systems->update(dt);
}

void drft::GameState::render(sf::RenderTarget& target)
{	
	_systems->render(target);
}

void drft::GameState::endState()
{
	std::cout << "Leaving GameState" << std::endl;
}

void drft::GameState::init()
{
	std::cout << "Initializing GameState..." << std::endl;
	_systems = std::make_unique<system::SystemScheduler>(_registry);

	std::cout << "Importing Systems..." << std::endl;
	importSystems();
	std::cout << "Initializing Systems..." << std::endl;
	_systems->initAll();

	util::buildTestEntities(_registry, 10000, { 0,0,1600,1600 });
}

void drft::GameState::importSystems()

{
	// Import all systems into game state in proper execution order
	_systems->add(std::make_unique<system::Renderer>(_registry, getContext().textures));
	
}
