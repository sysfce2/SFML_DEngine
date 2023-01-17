#pragma once

namespace drft::system
{
	enum class Phase
	{
		OnStart = -1,
		OnPreUpdate,
		OnUpdate,
		OnPostUpdate,
		OnValidation,
		AllUpdate,
		OnRender,
		AllPhases
	};


	class System
	{
	public:
		System(entt::registry& registry) : registry(registry) {};
		virtual ~System() = default;
		System(const System&) = default;
		System& operator=(const System&) = default;
		System(System&&) = default;
		System& operator=(System&&) = default;

		virtual void update(const float dt);
		virtual void render(sf::RenderTarget& target);

	protected:
		friend class SystemQueue; // SystemQueue needs phase
		entt::registry& registry;
		Phase phase = Phase::OnUpdate;
	};

} // namespace drft::system



