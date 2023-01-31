#pragma once

namespace drft::util
{
	class DebugInfo
	{
	public:
		void addString(std::string name, std::string data);
		void setStrings(sf::Text& text);

	private:
		std::unordered_map<std::string, std::string> _debugStrings;
		std::vector<std::string> _dataOrder;
	};
}


