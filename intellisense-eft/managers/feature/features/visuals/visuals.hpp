#pragma once

#include <dependencies/common_includes.hpp>

#include <game/structures/structures.hpp>

namespace features::visuals
{
	void draw_box( structures::player_t* player );

	void draw_snapline( );

	void draw_grenade( structures::throwable_t* grenade );
	void draw_loot(structures::loot_entry_t exfil);
	void draw_exfil( structures::exfiltration_point_t* exfil );

	void draw_fov();

	void callback( );
}