#include "feature.hpp"

// --FEATURES--
// -EVENT-
#include "features/event/event.hpp"

// -VISUALS-
#include "features/visuals/chams/chams.hpp"
#include "features/visuals/visuals.hpp"
#include "features/visuals/no_visor/no_visor.hpp"

// -WEAPON-
#include "features/weapon/no_recoil/no_recoil.hpp"
#include "features/weapon/no_sway/no_sway.hpp"
#include "features/weapon/silent/silent_aim/silent_aim.hpp"

// -MISC-
#include "features/misc/looting/looting.hpp"
#include "features/misc/stamina/stamina.hpp"
#include "features/misc/speed/speed.hpp"
// --FEATURES--

#include "collector/collector.hpp"

#include <managers/managers.hpp>

void managers::feature_manager_t::setup_callback( )
{
	const auto players = globals.get_players();

	globals.set_local_player(!players.empty() ? players.at(0) : nullptr);

	globals.set_camera(structures::unity::camera_t::get());

	utilities::io::log( "New game world : 0x%p\n", globals.get_game_world() );
	utilities::io::log( "New local player : 0x%p\n", globals.get_local_player() );
	utilities::io::log( "Cam : 0x%p\n", globals.get_camera() );

	features::weapon::no_recoil::callback( );
	features::misc::looting::callback();
}

void managers::feature_manager_t::free_callback( )
{
	globals.clear_exfils();

	globals.set_game_world(nullptr);

	globals.clear_loot();
	globals.clear_players();
	globals.set_local_player(nullptr);
	globals.set_camera(nullptr);
	globals.set_closest_player(nullptr);
	globals.clear_grenades();

	globals.clear_shaders();
	globals.clear_update_queue();

	utilities::io::log( "Left game, freeing globals\n" );
}

void managers::feature_manager_t::initiate( )
{
	features::events::initiate( );

	features::weapon::no_recoil::initiate( );
	features::weapon::no_sway::initiate( );
	features::weapon::silent::silent_aim::initiate( );

	features::misc::stamina::initiate( );
	//features::misc::speed::initiate( );
}

void managers::feature_manager_t::visual_callback( )
{
	if ( in_game && globals.get_camera() && globals.get_local_player() ) {
		features::visuals::callback();
		features::visuals::no_visor::callback();
	}
}