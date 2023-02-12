#include "silent_aim.hpp"

#include <managers/managers.hpp>

using namespace managers;

void features::weapon::silent::silent_aim::initiate( )
{
	const auto create = mono::structures::image_t::get_image_by_name( "Assembly-CSharp" )
		->get_class( "", "\uEADB" )->get_method_from_name( "\uE004", -1 )
			->get_function_pointer< std::uint8_t* >( );

	mgrs.hook_mgr.hooks.on_create_shot.hook(
		{ reinterpret_cast< std::uintptr_t >( create ),
			[ ] ( void* rcx, void* ammo, std::int32_t fragment_index, std::int32_t seed, structures::unity::vector3_t origin, structures::unity::vector3_t direction, float initial_speed, float speed, float bullet_mass_kg, float bullet_diameter, float damage, float pen_power, float pen_chance, float ricochet_chance, float fragment_chance, float deviation_chance, std::int32_t min_frag_count, std::int32_t max_frag_count, void* default_ballistic_collider, void* randoms, float ballistic_coefficient, void* player, void* weapon, std::int32_t fire_index, void* parent )
			{
				if (mgrs.cfg_mgr.get<bool>(features_t::silent_aim) && structures::unity::input_t::get_key(structures::unity::e_keycodes::mouse4))
				{
					const auto lp = mgrs.feature_mgr.globals.get_local_player();

					const auto lp_rotation = lp->get_rotation();

					if (const auto closest_player = mgrs.feature_mgr.globals.get_closest_player())
					{
						const auto head_transform = closest_player
							->get_bone_transform(structures::tarkov::bones::head);

						const auto head_pos = head_transform->get_position();
						
						const auto fov = closest_player->get_fov_to(structures::tarkov::bones::head, lp_rotation);

						if (fov < mgrs.cfg_mgr.get<float>(features_t::silent_aim_fov))
						{
							const auto cam_transform = mgrs.feature_mgr.globals.get_camera()->get_transform();

							if (cam_transform)
							{
								structures::unity::raycast_hit_t hit;

								const auto cam_pos = cam_transform->get_position();

								if (mgrs.cfg_mgr.get<bool>(features_t::silent_aim_linecast) && structures::unity::physics_t::linecast(cam_pos, head_pos, &hit, structures::tarkov::layers::HighPoly | structures::tarkov::layers::HitCollider | structures::tarkov::layers::LowPoly | structures::tarkov::layers::Interactive))
									//if (hit.get_transform() != head_transform)
										return;

								direction = structures::unity::vector3_t{ head_pos - origin }.normalized();
							}
						}
					}
				}

				mgrs.hook_mgr.hooks.on_create_shot.call_original( rcx, ammo, fragment_index, seed,origin, direction, initial_speed, speed, bullet_mass_kg, bullet_diameter, damage, pen_power, pen_chance, ricochet_chance, fragment_chance, deviation_chance, min_frag_count, max_frag_count, default_ballistic_collider, randoms, ballistic_coefficient, player, weapon, fire_index, parent );
			}
		}
	);
}
