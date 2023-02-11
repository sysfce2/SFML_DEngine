#include "pch.h"
#include "TurnManager.h"
#include "Events/Events.h"

void drft::system::TurnManager::init()
{
	registry->on_destroy<component::Actor>().connect<&TurnManager::onActorRemove>(this);
	registry->on_destroy<component::Active>().connect<&TurnManager::onActorRemove>(this);

	registry->ctx().get<entt::dispatcher&>().sink<events::SpendActionPoints>().connect<&TurnManager::onSpendActionPoints>(this);

	_timeKeeper = registry->create();
	registry->emplace<component::Actor>(_timeKeeper, 100, 1.0f, 1.0f);

}

void drft::system::TurnManager::update(const float)
{
	updateActorQueue();

	auto playerView = registry->view<component::Player, component::Actor>();
	for (auto e : playerView)
	{
		_player = e;
	}

	_currentActor = _queue.front();
	if (_currentActor == _timeKeeper)
	{
		tick();
	}
	registry->emplace_or_replace<component::MyTurn>(_currentActor);

}

void drft::system::TurnManager::onActorRemove(entt::registry& registry, entt::entity entity)
{
	if (registry.any_of<component::Prototype>(entity)) return;
	if (!_managedEntities.contains(entity)) return;

	_managedEntities.erase(entity);
	auto toRemove = std::find(_queue.begin(), _queue.end(), entity);
	if (toRemove != _queue.end())
	{
		_queue.erase(toRemove);
		sortQueue();
	}
}

void drft::system::TurnManager::onSpendActionPoints(events::SpendActionPoints& ev)
{
	auto& actor = registry->get<component::Actor>(_currentActor);
	actor.ap -= ev.amount;
	popFrontPushBack();
	sortQueue();
	registry->remove<component::MyTurn>(_currentActor);
}

void drft::system::TurnManager::updateActorQueue()
{
	auto actorView = registry->view<component::Actor, component::Active>(entt::exclude<component::Prototype>);
	for (auto entity : actorView)
	{
		if (_managedEntities.contains(entity)) continue;

		_queue.push_back(entity);
		_managedEntities.insert(entity);
	}

	sortQueue();
}

void drft::system::TurnManager::popFrontPushBack()
{
	auto front = _queue.front();
	_queue.erase(_queue.begin());
	_queue.push_back(front);
}

void drft::system::TurnManager::sortQueue()
{
	const auto& tmp = registry;
	std::stable_sort(_queue.begin(), _queue.end(),
		[tmp](const entt::entity& l, const entt::entity& r) -> bool
		{
			auto& lActor = tmp->get<component::Actor>(l);
			auto& rActor = tmp->get<component::Actor>(r);

			return lActor.ap > rActor.ap;
		}
	);
}

void drft::system::TurnManager::printQueue()
{
	std::cout << "Turn Queue:" << std::endl;
	int count = 1;
	for (auto e : _queue)
	{
		if (e == _timeKeeper)
		{
			auto& tk = registry->get<component::Actor>(_timeKeeper);
			std::cout << count++ << ". " << "Time Keeper" << ": " << tk.ap << " pts" << std::endl;
			continue;
		}
		auto& info = registry->get<component::Info>(e);
		auto& actor = registry->get<component::Actor>(e);
		std::cout << count << ". " << info.name << ": " << actor.ap << " pts" << std::endl;
		++count;
	}
	std::cout << std::endl;
}

void drft::system::TurnManager::tick()
{
	std::cout << "Tick!" << std::endl;
	for (auto e : _queue)
	{
		if (e == _timeKeeper) continue;
		auto& actor = registry->get<component::Actor>(e);
		actor.ap += AP_PER_TICK;
	}
	popFrontPushBack();
	sortQueue();
	_currentActor = _queue.front();
}

