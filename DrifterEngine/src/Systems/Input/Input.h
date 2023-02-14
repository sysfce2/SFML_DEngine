#pragma once

namespace drft::action
{
	struct Action;
}

namespace drft::system::input
{
	std::unique_ptr<action::Action> getInput(entt::handle& entity);
}