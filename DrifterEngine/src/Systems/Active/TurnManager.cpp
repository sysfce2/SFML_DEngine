#include "pch.h"
#include "TurnManager.h"
#include "Components/Components.h"
#include "Components/Tags.h"
#include "Systems/Input/Input.h"
#include "Actions/Action.h"

void drft::system::TurnManager::init()
{
	registry->on_destroy<component::Actor>().connect<&TurnManager::onActorRemove>(this);
	registry->on_destroy<component::tag::Active>().connect<&TurnManager::onActorRemove>(this);

	_actorQueue = std::make_unique<ActorQueue>(*registry);

	_timeKeeper = registry->create();
	registry->emplace<component::Actor>(_timeKeeper, 100, 1.0f, 1.0f);
	registry->emplace<component::tag::Active>(_timeKeeper);

	_actorQueue->setSentinel(_timeKeeper);
}

void drft::system::TurnManager::update(const float)
{
	_actorQueue->refresh(_managedEntities);
	
	auto& currentActor = _actorQueue->getCurrentActor();
	auto currentEntity = entt::to_entity(*registry, currentActor);

	while (currentActor.ap < 0)
	{
		_actorQueue->rotate();
		currentActor = _actorQueue->getCurrentActor();
	}

	if (currentEntity == _timeKeeper)
	{
		_actorQueue->tick();
		return;
	}
	
	auto pointsSpent = act(currentActor);
	currentActor.ap -= pointsSpent;
	if (pointsSpent > 0.0f)
	{
		_actorQueue->rotate();
	}
}

void drft::system::TurnManager::onActorRemove(entt::registry& registry, entt::entity entity)
{
	if (registry.any_of<component::Prototype>(entity)) return;
	if (_managedEntities.contains(entity))
	{
		_managedEntities.erase(entity);
	}
	_actorQueue->remove(entity);
}

float drft::system::TurnManager::act(component::Actor& actor)
{
	auto currentEntity = entt::to_entity(*registry, actor);
	entt::handle handle = entt::handle(*registry, currentEntity);
	auto desiredAction = input::getInput(handle);
	float pointsSpent = action::attempt(handle, std::move(desiredAction));
	
	return pointsSpent;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Actor Queue
///////////////////////////////////////////////////////////////////////////////////////////////////

drft::system::ActorQueue::ActorQueue(entt::registry& registry)
	: registry(registry)
	, _sentinel(entt::null)
{}

void drft::system::ActorQueue::refresh(std::set<entt::entity>& currentEntities)
{
	auto actorView = registry.view<component::Actor, component::tag::Active>(entt::exclude<component::Prototype>);
	for (auto entity : actorView)
	{
		if (currentEntities.contains(entity)) continue;

		// TODO: find out way to insert above timeKeeper (cache position?)
		auto& actor = actorView.get<component::Actor>(entity);
		actor.ap = 0;

		_queue.push_back(entity);
		currentEntities.insert(entity);
	}
}

void drft::system::ActorQueue::rotate()
{
	auto front = _queue.front();
	_queue.erase(_queue.begin());
	_queue.push_back(front);
}

void drft::system::ActorQueue::tick()
{
	std::cout << "Tick!" << std::endl;
	for (auto& e : _queue)
	{
		if (e == _sentinel) continue;
		auto& actor = registry.get<component::Actor>(e);
		actor.ap += AP_PER_TICK;
	}
	rotate();
}

void drft::system::ActorQueue::setSentinel(entt::entity sentinel)
{
	_sentinel = sentinel;
}

component::Actor& drft::system::ActorQueue::getCurrentActor() const
{
	auto& actor = registry.get<component::Actor>(_queue.front());
	return actor;
}

void drft::system::ActorQueue::remove(entt::entity entity)
{
	auto toRemove = std::find(_queue.begin(), _queue.end(), entity);
	if (toRemove != _queue.end())
	{
		_queue.erase(toRemove);
	}
}
