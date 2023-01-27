#include "pch.h"
#include "Renderer.h"
#include "Components/Components.h"
#include "Utility/SpriteBatch.h"


void drft::system::Renderer::init()
{
}

void drft::system::Renderer::render(sf::RenderTarget& target)
{
	auto view = registry.view< const component::Position, const component::Render >();

	for (auto const & [entity, pos, ren] : view.each())
	{
		_spriteBatch.addSprite(ren.sprite, ren.color, { pos.x, pos.y });
	}

	target.draw(_spriteBatch);
	_spriteBatch.clear();
}
