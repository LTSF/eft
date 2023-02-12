#include "stamina.hpp"

#include <managers/managers.hpp>

using namespace managers;

void features::misc::stamina::initiate( )
{
	//this.Stamina.UpdateStamina((float)(value.StaminaExhausted ? 0 : 100));
	const auto c = mono::structures::image_t::get_image_by_name( "Assembly-CSharp" )
		->get_class( "", "\uE302" );

	const auto process_stamina = c->get_method_from_name( "Process", -1 )
			->get_function_pointer< std::uint8_t* >( );

	static const auto current_offs = c->get_field( "Current" )->get_offset( );

	mgrs.hook_mgr.hooks.on_process_stamina.hook(
		{ reinterpret_cast< std::uintptr_t >( process_stamina ),
			[ ] ( std::uint8_t* ecx, float dt )
			{
				if (mgrs.cfg_mgr.get<bool>(features_t::infinite_stamina))
					*reinterpret_cast< float* >( ecx + current_offs ) = 100.f;

				mgrs.hook_mgr.hooks.on_process_stamina.call_original( ecx, dt );
			}
		}
	);
}
