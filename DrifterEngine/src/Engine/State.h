#pragma once
#include "Utility/ResourceHolder.h"

namespace drft
{
	class State
	{
	public:

		struct Window
		{
			int width = 0;
			int height = 0;
		};

		struct Context
		{
			using TextureHolder = ResourceHolder<sf::Texture, std::string>;
			using FontHolder = ResourceHolder<sf::Font, std::string>;

			Context(TextureHolder& textures, FontHolder& fonts, Window window)
				: textures(textures), fonts(fonts), window(window) {}
			Context(const Context& c) = default;
			Context& operator=(const Context&) = default;
			Context(Context&&) = default;
			Context& operator=(Context&&) = default;

			TextureHolder& textures;
			FontHolder& fonts;
			Window window;
		};

	public:
		State(Context context);
		virtual ~State();

		const bool& getQuit() const;
		virtual bool processEvent(const sf::Event& ev);

		virtual void update(const float dt) = 0;
		virtual void render(sf::RenderTarget& target) = 0;
		virtual void endState() = 0;

	protected:
		Context &const getContext();

	private:
		virtual void init() = 0;

	private:
		bool _quit;
		Context _context;
	};
}



