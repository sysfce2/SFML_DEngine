#include "pch.h"
#include "GameState.h"

#include "Spatial/WorldGrid.h"
#include "Spatial/Conversions.h"

#include "Systems/SystemScheduler.h"
#include "Systems/Core/TileRenderer.h"
#include "Systems/Core/EntityRenderer.h"
#include "Systems/Core/RealityBubble.h"
#include "Systems/Core/PlayerInput.h"
#include "Systems/Core/Camera.h"
#include "Systems/Core/ChunkManager.h"
#include "Systems/Core/TurnManager.h"
#include "Systems/Core/WorldGridResolver.h"
#include "Systems/Gameplay/DamageSystem.h"
#include "Systems/Gameplay/DeathSystem.h"
#include "Systems/Gameplay/MovementSystem.h"
#include "Systems/Gameplay/LaunchAttackSystem.h"

#include "Components/Components.h"
#include "Components/Tags.h"
#include "Components/Meta.h"

#include "Utility/TestEntities.h"
#include "Factory/EntityFactory.h"



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
	std::cout << "Initializing GameState..." << std::endl;

	_systems = std::make_unique<system::SystemScheduler>(_registry);
	_world = std::make_unique<spatial::WorldGrid>();
	_factory = std::make_unique<EntityFactory>();
	_dispatcher = std::make_unique<entt::dispatcher>();

	_registry.ctx().emplace<spatial::WorldGrid&>(*_world);
	_registry.ctx().emplace<sf::Texture&>(getContext().textures.get("Sprites"));
	_registry.ctx().emplace<EntityFactory&>(*_factory);
	_registry.ctx().emplace<entt::dispatcher&>(*_dispatcher);
	
	importSystems();

	_factory->loadPrototypes("prototypes.json", _registry);
	_factory->loadPrototypes("player.json", _registry);

	// Add Player
	auto player = _factory->build("Player", _registry);
	player.emplace<component::Player>();
	sf::Vector2f startingPosition = spatial::toWorldSpace({ 32,32 });
	player.patch<component::Position>([&](auto& pos)
		{
			pos.position = startingPosition;
		});

	// ADD CAMERA ENTITY // 
	//
	auto camera = _registry.create();
	int viewportWidth = getContext().window.width;
	int viewportHeight = getContext().window.height;
	_registry.emplace<component::Camera>(camera, sf::FloatRect(0,0,viewportWidth, viewportHeight), player);
	_registry.emplace<component::Position>(camera, startingPosition, (int)spatial::Layer::Camera);
	//
	////////////////////////////////////////////////////////////////
	std::cout << "Starting Gamestate" << std::endl;
}

void drft::GameState::importSystems()
{
	std::cout << "Importing Systems..." << std::endl;

	using namespace system;

	// Import all systems into game state
	// Add an offset to adjust execution order of systems
	_systems->add(PlayerInput(),		Phase::OnProcessInput);
	_systems->add(TurnManager(),		Phase::OnPreUpdate);
	_systems->add(MovementSystem(),		Phase::OnUpdate);
	_systems->add(LaunchAttackSystem(), Phase::OnUpdate + 10);
	_systems->add(DamageSystem(),		Phase::OnUpdate + 10);
	_systems->add(DeathSystem(),		Phase::OnUpdate + 15);
	_systems->add(Camera(),				Phase::OnPostUpdate);
	_systems->add(ChunkManager(),		Phase::OnPostUpdate);
	_systems->add(RealityBubble(),		Phase::OnValidation);
	_systems->add(TileRenderer(),		Phase::OnRender);
	_systems->add(EntityRenderer(),		Phase::OnRender);
	_systems->add(WorldGridResolver(),	Phase::Reactive);

	_systems->initAll();
}
