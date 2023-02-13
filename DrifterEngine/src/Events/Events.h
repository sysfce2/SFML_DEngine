#pragma once

namespace drft::events
{
	struct ActionPerformed
	{
		int amount = 0;
		bool hasAnimation = false;
	};
}