#include "pch.h"
#include "TurnManager.h"
#include "Components/Components.h"
#include "Components/Tags.h"

void drft::system::TurnManager::init()
{
	registry->on_destroy<component::Actor>().connect<&TurnManager::onActorRemove>(this);
	registry->on_destroy<component::tag::Active>().connect<&TurnManager::onActorRemove>(this);

	_actorQueue = std::make_unique<ActorQueue>(*registry);

	_timeKeeper = registry->create();
	registry->emplace<component::Actor>(_timeKeeper, 100, 1.0f, 1.0f);
	registry->emplace<component::tag::Active>(_timeKeeper);
	_actorQueue->setSentinel(_timeKeeper);
	_managedEntities.insert(_timeKeeper);
}

void drft::system::TurnManager::update(const float)
{
	_actorQueue->refresh(_managedEntities);
	auto spendPointsView = registry->view<component::action::SpendPoints, component::Actor>();
	for (auto [entity, points, actor] : spendPointsView.each())
	{
		actor.ap -= points.amount;
		registry->remove<component::action::SpendPoints>(entity);
	}

	auto currentActor = registry->get<component::Actor>(_actorQueue->front());
	while (currentActor.ap <= 0)
	{
		registry->remove<component::tag::CurrentActor>(_actorQueue->front());
		currentActor = _actorQueue->next();
	}

	if (_actorQueue->front() == _timeKeeper)
	{
		_actorQueue->tick();
		return;
	}

	registry->emplace_or_replace<component::tag::CurrentActor>(_actorQueue->front());
}

void drft::system::TurnManager::onActorRemove(entt::registry& registry, entt::entity entity)
{
	if (registry.any_of<component::Prototype>(entity)) return;
	if (_managedEntities.contains(entity))
	{
		_managedEntities.erase(entity);
	}
	registry.remove<component::tag::CurrentActor>(entity);
	_actorQueue->remove(entity);
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

component::Actor& drft::system::ActorQueue::next()
{
	auto front = _queue.front();
	_queue.erase(_queue.begin());
	_queue.push_back(front);
	return registry.get<component::Actor>(_queue.front());
}

entt::entity drft::system::ActorQueue::front() const
{
	return _queue.front();
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
	next();
}

void drft::system::ActorQueue::printQueue() const
{
	int count = 1;
	for (auto entity : _queue)
	{
		std::cout << count << ". Entity: " << (uint32_t)entity << std::endl;
		++count;
	}
}

void drft::system::ActorQueue::setSentinel(entt::entity sentinel)
{
	_sentinel = sentinel;
	_queue.push_back(_sentinel);

}

void drft::system::ActorQueue::remove(entt::entity entity)
{
	auto toRemove = std::find(_queue.begin(), _queue.end(), entity);
	if (toRemove != _queue.end())
	{
		_queue.erase(toRemove);
	}
}
