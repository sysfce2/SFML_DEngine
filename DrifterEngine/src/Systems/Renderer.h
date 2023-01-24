#pragma once
#include "System.h"
#include "Utility/ResourceHolder.h"
#include "Utility/SpriteBatch.h"

namespace drft::system
{
    class Renderer : public System
    {
    public:
        Renderer(entt::registry& registry, ResourceHolder<sf::Texture, std::string>& textures) 
            : _textures(textures), System(registry)
        {
            phase = Phase::OnRender;
            _spriteBatch.setTexture(textures.get("Sprites"));
        }

        virtual void init() override;
        virtual void render(sf::RenderTarget& target) override;

    private:
        ResourceHolder<sf::Texture, std::string>& _textures;
        SpriteBatch _spriteBatch;
    };

} // namespace system


