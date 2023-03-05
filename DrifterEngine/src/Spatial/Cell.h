#pragma once

namespace drft::spatial
{
	class Cell
	{
	public:
		void placeEntity(entt::entity entity, int layer);
		entt::entity removeEntity(entt::entity entity, int layer);
		std::vector<entt::entity> at(int layer);
		bool empty();

	private:
		std::unordered_map<int, std::vector<entt::entity> > _entities;
	};

} // namespace drft::spatial


