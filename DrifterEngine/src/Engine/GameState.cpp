#include "pch.h"
#include "GameState.h"
#include "Spatial/WorldGrid.h"
#include "Systems/SystemScheduler.h"
#include "Systems/TileRenderer.h"
#include "Systems/EntityRenderer.h"
#include "Systems/WorldGridResolver.h"
#include "Systems/InputHandler.h"
#include "Systems/ActionHandler.h"
#include "Systems/Camera.h"
#include "Systems/ChunkManager.h"
#include "Components/Components.h"
#include "Components/Meta.h"
#include "Utility/TestEntities.h"
#include "Factory/EntityFactory.h"


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

	_registry.ctx().emplace<spatial::WorldGrid&>(*_world);
	_registry.ctx().emplace<util::DebugInfo&>(getContext().debugInfo);
	_registry.ctx().emplace<sf::Texture&>(getContext().textures.get("Sprites"));
	
	importSystems();
	_systems->initAll();

	// ADD PLAYER ENTITY // ** temporary just for testing **
	//
	auto player = _registry.create();
	auto startingPosition = spatial::toWorldSpace({ 10, 10 });
	_registry.emplace<component::Info>(player, "Player", "", "");
	_registry.emplace<component::Position>(player, startingPosition, (int)spatial::Layer::Actor);
	_registry.emplace<component::Render>(player, 1u, sf::Color::White);
	_registry.emplace<component::Player>(player);
	//
	////////////////////////////////////////////////////////////////

	// ADD CAMERA ENTITY // ** temporary just for testing **
	//
	auto camera = _registry.create();
	int viewportWidth = getContext().window.width;
	int viewportHeight = getContext().window.height;
	_registry.emplace<component::Camera>(camera, sf::FloatRect(0,0,viewportWidth, viewportHeight), player);
	_registry.emplace<component::Position>(camera, startingPosition, (int)spatial::Layer::Camera);
	//
	////////////////////////////////////////////////////////////////
}

void drft::GameState::importSystems()
{
	std::cout << "Importing Systems..." << std::endl;

	// Import all systems into game state
	_systems->add(system::TileRenderer());
	_systems->add(system::EntityRenderer());
	_systems->add(system::Camera());
	_systems->add(system::WorldGridResolver());
	_systems->add(system::InputHandler());
	_systems->add(system::ActionHandler());
	_systems->add(system::ChunkManager());
}
