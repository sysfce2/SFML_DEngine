#pragma once
#include "System.h"
#include "Utility/SpriteBatch.h"

namespace drft::system
{
	class TileRenderer : public System
	{
    public:
        TileRenderer(entt::registry& registry)
            : System(registry)
        {
            phase = Phase::OnRender;
            _spriteBatch.setTexture(registry.ctx().get<sf::Texture&>());
        }

        virtual void init() override;
        virtual void render(sf::RenderTarget& target) override;

    private:
        SpriteBatch _spriteBatch;
        sf::Vector2f _prevPosition = { 0,0 };
	};
}


