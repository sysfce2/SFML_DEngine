#pragma once

namespace drft
{
	enum ActionType
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
		Swap,
		Attack,
		PickUp,
		Drop,
		Talk,
		Contextual,
		Total
	};
} // namespace drft::action



