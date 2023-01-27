#pragma once

namespace drft::action
{
	enum Type
	{
		None,
		Wait,
		MoveNorth,
		MoveSouth,
		MoveWest,
		MoveEast,
		MoveSouthEast,
		MoveSouthWest,
		MoveNorthEast,
		MoveNorthWest,
		Attack,
		PickUp,
		Drop,
		Talk,
		Contextual,
		Total
	};

	class IAction
	{
	public:
		virtual bool execute()
	};

} // namespace drft::action



