#include "pch.h"
#include "DebugInfo.h"

using namespace drft::util;

void DebugInfo::addString(std::string name, std::string data)
{
	if (!_debugStrings.contains(name))
	{
		_dataOrder.push_back(name);
	}
	_debugStrings[name] = data;
}

void DebugInfo::setStrings(sf::Text& text)
{
	std::stringstream ss;
	for (auto& name : _dataOrder)
	{
		ss << (name + " = " + _debugStrings[name]) << "\n";
	}
	text.setString(ss.str());
}