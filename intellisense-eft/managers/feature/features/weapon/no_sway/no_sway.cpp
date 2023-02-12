#include "no_sway.hpp"

#include <managers/managers.hpp>
#include <game/structures/structures.hpp>

using namespace managers;

void features::weapon::no_sway::initiate( )
{
	const auto sway_factor_update = mono::structures::image_t::get_image_by_name( "Assembly-CSharp" )
		->get_class( "EFT.Animations", "ProceduralWeaponAnimation" )->get_method_from_name( "UpdateSwayFactors", -1 )
			->get_function_pointer< std::uint8_t* >( );

	mgrs.hook_mgr.hooks.on_sway_factor_update.hook(
		{ reinterpret_cast< std::uintptr_t >(sway_factor_update),
			[ ] ( structures::procedural_weapon_animation_t* procedural_weapon_animation )
			{
				structures::breath_effector_t* breath_effector = procedural_weapon_animation->get_breath_effector();
				if (breath_effector)
					breath_effector->set_intensity(mgrs.cfg_mgr.get<bool>(features_t::no_sway) ? 0.0f : 1.0f );

				mgrs.hook_mgr.hooks.on_sway_factor_update.call_original(procedural_weapon_animation );
			}
		}
	);
}

