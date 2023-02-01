#include "pch.h"
#include "Camera.h"
#include "Components/Components.h"
#include "Spatial/WorldGrid.h"
#include "Engine/State.h"

void drft::system::Camera::init()
{
}

void drft::system::Camera::update(const float dt)
{
	auto&& cameraView = registry.view<component::Camera, component::Position>();

	for (auto [entity, camera, pos] : cameraView.each())
	{
		auto const & target = registry.get<const component::Position>(camera.target);
		if (std::abs(pos.position.x - target.position.x) < 0.25f)
		{
			pos.position.x = target.position.x;
		}
		else
		{
			pos.position.x = std::lerp(pos.position.x, target.position.x, _speed * dt);
		}
		if (std::abs(pos.position.y - target.position.y) < 0.25f)
		{
			pos.position.y = target.position.y;
		}
		else
		{
			pos.position.y = std::lerp(pos.position.y, target.position.y, _speed * dt);
		}

		camera.viewport.left = pos.position.x - (camera.viewport.width / 2);
		camera.viewport.top = pos.position.y - (camera.viewport.height / 2);

		auto& debug = registry.ctx().get<util::DebugInfo>();
		auto tilePos = spatial::toTileSpace(target.position);
		std::string dataStr = std::to_string(tilePos.x) + ", " + std::to_string(tilePos.y);
		debug.addString("Position", dataStr);
		auto chunkPos = spatial::toChunkCoordinate(target.position);
		dataStr = std::to_string(chunkPos.x) + ", " + std::to_string(chunkPos.y);
		debug.addString("Coords", dataStr);
	}
}
