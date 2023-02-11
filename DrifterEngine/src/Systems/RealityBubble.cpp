#include "pch.h"
#include "RealityBubble.h"
#include "Components/Components.h"
#include "Spatial/Conversions.h"
#include "Spatial/WorldGrid.h"
#include "Spatial/Helpers.h"

void drft::system::RealityBubble::init()
{

}

void drft::system::RealityBubble::update(const float)
{
	auto cameraView = registry->view<component::Camera, component::Position>(entt::exclude<component::Prototype>);
	sf::Vector2i newCamPosition;
	for (auto [entity, camera, position] : cameraView.each())
	{
		newCamPosition = spatial::toChunkCoordinate(position.position);
	}

	if (newCamPosition != _cameraPosition)
	{

	}
	
}