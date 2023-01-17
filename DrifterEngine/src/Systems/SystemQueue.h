#pragma once
#include "System.h"


// Container class that orders systems by their phase
namespace drft::system
{

	class SystemQueue
	{
	public:
		SystemQueue(entt::registry& registry) : _registry(registry) {}
		void add(std::unique_ptr<System> sys);
		void update(const float dt);
		void render(sf::RenderTarget& target);

	private:
		using SystemList = std::vector< std::unique_ptr< System > >;
		std::map<system::Phase, SystemList> _systems;
		entt::registry& _registry;
	};

} // namespace drft::system


