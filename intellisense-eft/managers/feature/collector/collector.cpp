#include "collector.hpp"

#include <managers/managers.hpp>
#include <game/structures/structures.hpp>

using namespace managers;

void collector::collect_players( )
{
	//if ( mgrs.feature_mgr.in_game )
	//{
	//	mgrs.feature_mgr.globals.set_players(
	//		mgrs.feature_mgr.globals.game_world->get_players( ) );

	//	utilities::io::log( "Collecting players\n" );
	//}
	//else
	//{
	//	utilities::io::log( "Collecting players but not in game\n" );
	//}
}

//void collector::collect_loot( )
//{
//	if ( mgrs.feature_mgr.in_game )
//	{
//		
//
//		utilities::io::log( "Collecting loot\n" );
//	}
//	else
//	{
//		utilities::io::log( "Collecting players but not in game\n" );
//	}
//}

void collector::collect_closest_player( )
{
	const auto camera = mgrs.feature_mgr.globals.get_camera();

	const auto lp = managers::mgrs.feature_mgr.globals.get_local_player();

	if ( mgrs.feature_mgr.in_game && lp && camera)
	{
		const auto lp_rotation = lp->get_rotation();

		const auto previous_closest = managers::mgrs.feature_mgr.globals.get_closest_player();

		const auto lock_target_toggle = managers::mgrs.cfg_mgr.get<bool>(features_t::silent_aim_lock_target);

		structures::player_t* closest_player = nullptr;

		float dist = std::numeric_limits< float >::max( );

		for ( const auto players = managers::mgrs.feature_mgr.globals.get_players( ); const auto player : players )
		{
			if ( lp == player )
				continue;

			const float fov = player->get_fov_to(structures::tarkov::bones::head, lp_rotation);

			if (lock_target_toggle && player == previous_closest && structures::unity::input_t::get_key(structures::unity::e_keycodes::mouse4)) {
				closest_player = player;
				break;
			}

			if ( fov < dist )
			{
				dist = fov;
				closest_player = player;
			}
		}

		managers::mgrs.feature_mgr.globals.set_closest_player( closest_player );
	}
}
