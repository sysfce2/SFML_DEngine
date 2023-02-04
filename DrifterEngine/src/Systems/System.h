#pragma once

namespace drft::system
{
	enum class Phase
	{
		OnStartUp = -1,
		OnProcessInput,
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
		System() {};
		virtual ~System() = default;
		System(const System&) = default;
		System& operator=(const System&) = default;
		System(System&&) = default;
		System& operator=(System&&) = default;

		void setRegistry(entt::registry& registry);
		virtual void init() = 0;
		virtual void update(const float dt);
		virtual void render(sf::RenderTarget& target);

	protected:
		friend class SystemScheduler; // SystemScheduler needs phase
		entt::registry* registry = nullptr;
		Phase phase = Phase::OnUpdate;
	};

} // namespace drft::system



