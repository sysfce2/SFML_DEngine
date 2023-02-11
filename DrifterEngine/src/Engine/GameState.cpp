#include "pch.h"
#include "GameState.h"
#include "Spatial/WorldGrid.h"
#include "Spatial/Conversions.h"
#include "Systems/SystemScheduler.h"
#include "Systems/TileRenderer.h"
#include "Systems/EntityRenderer.h"
#include "Systems/WorldGridResolver.h"
#include "Systems/RealityBubble.h"
#include "Systems/PlayerInput.h"
#include "Systems/ArtificialInput.h"
#include "Systems/ActionHandler.h"
#include "Systems/Camera.h"
#include "Systems/ChunkManager.h"
#include "Systems/TurnManager.h"
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
	_factory = std::make_unique<EntityFactory>();
	_dispatcher = std::make_unique<entt::dispatcher>();

	_registry.ctx().emplace<spatial::WorldGrid&>(*_world);
	_registry.ctx().emplace<util::DebugInfo&>(getContext().debugInfo);
	_registry.ctx().emplace<sf::Texture&>(getContext().textures.get("Sprites"));
	_registry.ctx().emplace<EntityFactory&>(*_factory);
	_registry.ctx().emplace<entt::dispatcher&>(*_dispatcher);
	
	importSystems();

	_factory->loadPrototypes("prototypes.json", _registry);
	_factory->loadPrototypes("player.json", _registry);

	sf::Vector2f startingPosition = spatial::toWorldSpace({ 32,32 });

	auto player = _factory->build("Player", _registry);
	player.patch<component::Position>([&](auto& pos)
		{
			pos.position = startingPosition;
		});

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
	_systems->add(system::RealityBubble());
	_systems->add(system::TurnManager());
	_systems->add(system::PlayerInput());
	_systems->add(system::ArtificialInput());
	_systems->add(system::ActionHandler());
	_systems->add(system::ChunkManager());

	_systems->initAll();
}
