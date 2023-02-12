#pragma once

#include <dependencies/common_includes.hpp>

#include <game/structures/structures.hpp>

namespace features::visuals::chams
{
	void callback( structures::player_t* player );

	void uncham(structures::player_t* player);
}