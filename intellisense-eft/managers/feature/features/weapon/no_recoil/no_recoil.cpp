#include "no_recoil.hpp"

#include <managers/managers.hpp>
#include <game/structures/structures.hpp>

using namespace managers;

void features::weapon::no_recoil::initiate( )
{
	const auto weapon_params_changed = mono::structures::image_t::get_image_by_name( "Assembly-CSharp" )
		->get_class( "", "ShotEffector" )->get_method_from_name( "OnWeaponParametersChanged", -1 )
			->get_function_pointer< std::uint8_t* >( );

	mgrs.hook_mgr.hooks.on_weapon_parameters_changed.hook(
		{ reinterpret_cast< std::uintptr_t >( weapon_params_changed ),
			[ ] ( structures::shot_effector_t* shot_effector )
			{
				mgrs.hook_mgr.hooks.on_weapon_parameters_changed.call_original( shot_effector );

				shot_effector->set_recoil_strength( mgrs.cfg_mgr.get<bool>(features_t::no_recoil) ? 0.0f : 1.0f);

				utilities::io::log( "on_weapon_parameters_changed ShotEffector : %p\n", shot_effector );
			}
		}
	);
}

void features::weapon::no_recoil::callback( )
{
	const auto lp = mgrs.feature_mgr.globals.get_local_player();

	if(lp)
		lp->get_procedural_weapon_animation( )
			->get_shot_effector( )
				->set_recoil_strength(mgrs.cfg_mgr.get<bool>(features_t::no_recoil) ? 0.0f : 1.0f);
}
