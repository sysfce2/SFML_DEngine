#include "pch.h"
#include "DebugInfo.h"

using namespace drft::service;

void drft::service::DebugInfo::putInfo(std::string name, const std::string& data)
{
	if (!_debugStrings.contains(name))
	{
		_stringOrder.push_back(name);
	}
	_debugStrings[name] = data;
}

void drft::service::DebugInfo::render(sf::RenderTarget& target)
{
	std::stringstream ss;
	for (auto& name : _stringOrder)
	{
		ss << name << ": " << _debugStrings[name] << std::endl;
	}
	_text.setString(ss.str());

	target.draw(_text);
}

void drft::service::DebugInfo::setPosition(sf::Vector2f position)
{
	_text.setPosition(position);
}

void drft::service::DebugInfo::setFont(const sf::Font& font)
{
	_text.setFont(font);
	_text.setFillColor(sf::Color::Yellow);
	_text.setCharacterSize(16);
	_text.setPosition({ 8,8 });
}
