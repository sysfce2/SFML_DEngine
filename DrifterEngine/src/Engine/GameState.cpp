#include "..\pch.h"
#include "GameState.h"
#include "../Systems/SystemQueue.h"
#include "../Systems/Renderer.h"


drft::GameState::GameState(State::Context context) : State(context)
{
	init();
}

void drft::GameState::update(const float dt)
{
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
	std::cout << "Initializing GameState" << std::endl;
	_systems = std::make_unique<system::SystemQueue>(_registry);

	importSystems();
}

void drft::GameState::importSystems()

{
	// Import all systems into game state in proper execution order
	_systems->add(std::make_unique<system::Renderer>(_registry, getContext().textures));
}
