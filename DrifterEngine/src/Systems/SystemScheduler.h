#pragma once
#include "System.h"


// Container class that orders systems by their phase
namespace drft::system
{
	enum class Phase
	{
		OnStartUp = -2,
		Reactive = -1,
		OnProcessInput,
		OnPreUpdate,
		OnUpdate,
		OnPostUpdate,
		OnValidation,
		AllUpdate,
		OnRender,
		AllPhases,
		None
	};
	class SystemScheduler
	{
	public:
		SystemScheduler(entt::registry& registry) : _registry(registry) {}

		void initAll() const;

		template <typename T>
		void add(T&& system, Phase phase)
		{
			std::string typeName = typeid(T).name();
			static_assert(std::is_base_of<System, T>::value, "In SystemScheduler: type is not a system.");
			std::cout << "Adding " << typeName << "..." << std::endl;
			_systems[phase].push_back(std::make_unique<T>());
			_systems[phase].back()->setRegistry(_registry);
		}

		void update(const float dt) const;
		void render(sf::RenderTarget& target) const;

	private:
		using SystemList = std::vector< std::unique_ptr< System > >;
		std::map<system::Phase, SystemList> _systems;
		entt::registry& _registry;
	};

} // namespace drft::system


