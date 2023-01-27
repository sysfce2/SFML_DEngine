#include "pch.h"
#include "GameState.h"
#include "Systems/SystemScheduler.h"
#include "Systems/Renderer.h"
#include "Systems/WorldGridResolver.h"
#include "Systems/InputHandler.h"
#include "Components/Components.h"
#include "Utility/TestEntities.h"
#include "Spatial/WorldGrid.h"


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
	_world = std::make_unique<spatial::WorldGrid>();

	std::cout << "Importing Systems..." << std::endl;
	importSystems();
	std::cout << "Initializing Systems..." << std::endl;
	_systems->initAll();


	// ADD PLAYER ENTITY // ** temporary just for testing **
	//
	auto player = _registry.create();
	auto startingPosition = spatial::toWorldSpace({ 10, 10 });
	_registry.emplace<component::Position>(player, startingPosition.x, startingPosition.y, (int)spatial::Layer::Actor);
	_registry.emplace<component::Render>(player, 1u, sf::Color::White);
	_registry.emplace<component::PlayerInput>(player);
	//
	////////////////////////////////////////////////////////////////
}

void drft::GameState::importSystems()

{
	// Import all systems into game state in proper execution order
	_systems->add(std::make_unique<system::Renderer>(_registry, getContext().textures));
	_systems->add(std::make_unique<system::WorldGridResolver>(_registry, *_world));
	_systems->add(std::make_unique<system::InputHandler>(_registry));
}
