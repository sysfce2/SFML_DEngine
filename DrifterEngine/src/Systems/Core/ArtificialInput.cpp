#include "pch.h"
#include "ArtificialInput.h"
#include "Components/Components.h"
#include "Components/Tags.h"

void drft::system::ArtificialInput::init()
{
}

void drft::system::ArtificialInput::update(const float dt)
{
	auto view = registry->view<const component::AI, component::tag::CurrentActor>();
	for (auto [entity, ai] : view.each())
	{

	}
}
