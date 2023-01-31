#include "pch.h"
#include "Renderer.h"
#include "Components/Components.h"
#include "Utility/SpriteBatch.h"


void drft::system::Renderer::init()
{
}

void drft::system::Renderer::render(sf::RenderTarget& target)
{
	sf::Vector2f cameraOrigin = { 0.0f, 0.0f };
	auto camera = registry.view<const component::Camera, const component::Position>();

	// assumes only one camera, otherwise will just use the last one's position
	for (auto const& [entity, cam, pos] : camera.each())
	{
		cameraOrigin = { cam.viewport.left, cam.viewport.top };
	}

	auto view = registry.view< const component::Position, const component::Render >();

	for (auto const & [entity, pos, ren] : view.each())
	{
		_spriteBatch.addSprite(ren.sprite, ren.color, pos.position - cameraOrigin);
	}

	target.draw(_spriteBatch);
	_spriteBatch.clear();
}
