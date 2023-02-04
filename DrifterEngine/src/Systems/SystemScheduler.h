#pragma once
#include "System.h"


// Container class that orders systems by their phase
namespace drft::system
{

	class SystemScheduler
	{
	public:
		SystemScheduler(entt::registry& registry) : _registry(registry) {}

		void initAll();
		template <typename T>
		void add(T&& system)
		{
			std::string typeName = typeid(T).name();
			static_assert(std::is_base_of<System, T>::value, "In SystemScheduler: type is not a system.");
			std::cout << "Adding " << typeName << "..." << std::endl;
			Phase phase = system.phase;
			_systems[phase].push_back(std::make_unique<T>());
			_systems[phase].back()->setRegistry(_registry);
		}
		void update(const float dt);
		void render(sf::RenderTarget& target);

	private:
		using SystemList = std::vector< std::unique_ptr< System > >;
		std::map<system::Phase, SystemList> _systems;
		entt::registry& _registry;
	};

} // namespace drft::system


