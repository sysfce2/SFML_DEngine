#pragma once

namespace drft::service
{
	class DebugInfo
	{
	public:
		static DebugInfo& instance()
		{
			static DebugInfo inst;
			return inst;
		}

		DebugInfo(const DebugInfo&) = delete;
		void operator=(const DebugInfo&) = delete;

		void putInfo(std::string name, const std::string& data);
		
		void render(sf::RenderTarget& target);
		
		void setPosition(sf::Vector2f position);
		void setFont(const sf::Font& font);
	private:
		DebugInfo() {}

	private:
		std::unordered_map<std::string, std::string> _debugStrings;
		std::vector<std::string> _stringOrder;
		sf::Text _text;
	};
}


