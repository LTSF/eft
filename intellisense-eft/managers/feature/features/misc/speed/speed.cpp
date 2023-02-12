#include "speed.hpp"

#include <managers/managers.hpp>

using namespace managers;

void features::misc::speed::initiate( )
{
	const auto set_time_scale = mono::structures::image_t::get_image_by_name( "UnityEngine.CoreModule" )
		->get_class( "UnityEngine", "Time" )->get_method_from_name( "set_timeScale", -1 )
			->get_function_pointer< void( __fastcall* )( float scale ) >( );

	set_time_scale( 2.0f );
}
