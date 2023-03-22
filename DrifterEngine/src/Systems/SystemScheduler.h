#pragma once
#include "System.h"


// Container class that orders systems by their phase
namespace drft::system
{
	enum Phase : int
	{
		Reactive = -100,
		OnProcessInput = 0,
		OnPreUpdate = 100,
		OnUpdate = 200,
		OnPostUpdate = 300,
		OnValidation = 400,
		OnRender = 500
	};

	namespace
	{
		struct SystemPriorityPair
		{
			std::unique_ptr<System> system = nullptr;
			int priority = 0;
		};
	}

	class SystemScheduler
	{
	public:
		SystemScheduler(entt::registry& registry) : _registry(registry) {}

		void initAll() const;

		template <typename T>
		void add(T&& system, int priority)
		{
			static_assert(std::is_base_of<System, T>::value, "In SystemScheduler: type is not a system.");

			if (priority >= OnRender)
			{
				_systems[OnRender].push_back({ std::make_unique<T>(), priority });
				_systems[OnRender].back().system->setRegistry(_registry);
				std::stable_sort(_systems[OnRender].begin(), _systems[OnRender].end(),
					[](const SystemPriorityPair& lhs, const SystemPriorityPair& rhs)
					{
						return lhs.priority < rhs.priority;
					});
			}
			else if (priority < OnProcessInput)
			{
				_systems[Reactive].push_back({ std::make_unique<T>(), priority });
				_systems[Reactive].back().system->setRegistry(_registry);
			}
			else
			{
				_systems[OnUpdate].push_back({ std::make_unique<T>(), priority });
				_systems[OnUpdate].back().system->setRegistry(_registry);
				std::stable_sort(_systems[OnUpdate].begin(), _systems[OnUpdate].end(),
					[](const SystemPriorityPair& lhs, const SystemPriorityPair& rhs)
					{
						return lhs.priority < rhs.priority;
					});
			}
		}

		void update(const float dt) const;
		void render(sf::RenderTarget& target) const;

	private:
		using SystemList = std::vector< SystemPriorityPair >;

		std::unordered_map<int, SystemList> _systems;
		entt::registry& _registry;
	};

} // namespace drft::system


