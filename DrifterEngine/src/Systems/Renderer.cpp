#include "../pch.h"
#include "Renderer.h"
#include "../Components/Components.h"
#include "../Utility/SpriteBatch.h"


void drft::system::Renderer::init()
{
}

void drft::system::Renderer::render(sf::RenderTarget& target)
{
	std::cout << "Rendering" << std::endl;
	auto view = registry.view< component::position, component::render >();

	for (auto [entity, pos, ren] : view.each())
	{
		_spriteBatch.addSprite(ren.sprite, ren.color, { pos.x, pos.y });
	}

	target.draw(_spriteBatch);
	_spriteBatch.clear();
}
