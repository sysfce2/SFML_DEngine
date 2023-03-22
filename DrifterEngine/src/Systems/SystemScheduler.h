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
		OnRender = 10000
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

			if (priority >= static_cast<int>(Phase::OnRender))
			{
				_systems[Phase::OnRender].push_back({ std::make_unique<T>(), priority });
				_systems[Phase::OnRender].back().system->setRegistry(_registry);
				std::stable_sort(_systems[Phase::OnRender].begin(), _systems[Phase::OnRender].end(),
					[](const SystemPriorityPair& lhs, const SystemPriorityPair& rhs)
					{
						return lhs.priority < rhs.priority;
					});
			}
			else if (priority < static_cast<int>(Phase::OnProcessInput))
			{
				_systems[Phase::Reactive].push_back({ std::make_unique<T>(), priority });
				_systems[Phase::Reactive].back().system->setRegistry(_registry);
			}
			else
			{
				_systems[Phase::OnUpdate].push_back({ std::make_unique<T>(), priority });
				_systems[Phase::OnUpdate].back().system->setRegistry(_registry);
				std::stable_sort(_systems[Phase::OnUpdate].begin(), _systems[Phase::OnUpdate].end(),
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

		std::unordered_map<Phase, SystemList> _systems;
		entt::registry& _registry;
	};

} // namespace drft::system


