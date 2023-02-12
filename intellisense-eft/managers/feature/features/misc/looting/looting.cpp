#include "looting.hpp"

#include <managers/managers.hpp>

using namespace managers;

void features::misc::looting::callback( )
{
	static const auto hard_c = mono::structures::image_t::get_image_by_name( "Assembly-CSharp" )
		->get_class( "", "EFTHardSettings" );

	static const auto get_instance = hard_c->get_method_from_name( "get_Instance", -1 )
			->get_function_pointer< std::uint8_t*( __fastcall* )( ) >( );

	const auto instance = get_instance();

	if (!instance)
		return;

	//far loot
	static const auto dist = hard_c->get_field( "LOOT_RAYCAST_DISTANCE" )->get_offset( );
	const auto old_dist = *reinterpret_cast< float* >( instance + dist );
	*reinterpret_cast<float*>(instance + dist) = mgrs.cfg_mgr.get<float>(features_t::loot_distance);

	//loot through walls
	static const auto gw_sf = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		 ->get_class("EFT", "GameWorld")->get_vtable()->get_static_field_data();
	
	if (!gw_sf)
		return;

	static const int32_t original_mask = *reinterpret_cast<int32_t*>(gw_sf + 0xC); //FindInteractable : if (!Physics.Linecast(ray.origin, hit.point, GameWorld.\uE00D))
	*reinterpret_cast<int32_t*>(gw_sf + 0xC) = mgrs.cfg_mgr.get<bool>(features_t::loot_through_walls) ? 1 : original_mask;

	////legacy loot through walls
	//static const auto behind = c->get_field( "BEHIND_CAST" )->get_offset( );
	//const auto old_behind = *reinterpret_cast< float* >( instance + behind );
	//*reinterpret_cast< float* >( instance + behind ) = -1.f;


}
