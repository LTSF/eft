#include "event.hpp"

#include <managers/managers.hpp>
#include <game/structures/structures.hpp>

#include <cwctype>

#include <managers/feature/collector/collector.hpp>

#include <managers/feature/features/visuals/chams/chams.hpp>

#include "../../../../game/item_list/items.hpp"

using namespace managers;

void features::events::initiate()
{
	const auto game_world = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT", "GameWorld");

	const auto game_started = game_world->get_method_from_name("OnGameStarted", -1)
		->get_function_pointer< std::uint8_t* >();

	mgrs.hook_mgr.hooks.on_game_started.hook(
		{ reinterpret_cast<std::uintptr_t>(game_started),
			[](structures::game_world_t* game_world)
			{
				mgrs.hook_mgr.hooks.on_game_started.call_original(game_world);

				mgrs.feature_mgr.in_game = true;

				mgrs.feature_mgr.globals.set_game_world(game_world);

				mgrs.feature_mgr.setup_callback();
			}
		}
	);

	const auto game_world_dispose = game_world->get_method_from_name("Dispose", 0)
		->get_function_pointer< std::uint8_t* >();

	mgrs.hook_mgr.hooks.on_game_world_dispose.hook(
		{ reinterpret_cast<std::uintptr_t>(game_world_dispose),
			[](void* game_world)
			{
				mgrs.hook_mgr.hooks.on_game_world_dispose.call_original(game_world);

				mgrs.feature_mgr.in_game = false;

				mgrs.feature_mgr.free_callback();
			}
		}
	);

	const auto register_player = game_world->get_method_from_name("RegisterPlayer", -1)
		->get_function_pointer< std::uint8_t* >();

	mgrs.hook_mgr.hooks.on_register_player.hook(
		{ reinterpret_cast<std::uintptr_t>(register_player),
			[](void* game_world, structures::player_t* player)
			{
				mgrs.hook_mgr.hooks.on_register_player.call_original(game_world, player);

				mgrs.feature_mgr.globals.add_player( player );

				features::visuals::chams::callback(player);
			}
		}
	);

	const auto unregister_player = game_world->get_method_from_name("UnregisterPlayer", -1)
		->get_function_pointer< std::uint8_t* >();

	mgrs.hook_mgr.hooks.on_unregister_player.hook(
		{ reinterpret_cast<std::uintptr_t>(unregister_player),
			[](void* game_world, structures::player_t* player)
			{
				mgrs.hook_mgr.hooks.on_unregister_player.call_original(game_world, player);

				mgrs.feature_mgr.globals.remove_player(player);
			}
		}
	);

	const auto register_grenade = game_world->get_method_from_name("RegisterGrenade", -1)
		->get_function_pointer< std::uint8_t* >();

	mgrs.hook_mgr.hooks.on_register_grenade.hook(
		{ reinterpret_cast<std::uintptr_t>(register_grenade),
			[](void* game_world,  structures::throwable_t* grenade)
			{
				mgrs.hook_mgr.hooks.on_register_grenade.call_original(game_world, grenade);

				mgrs.feature_mgr.globals.push_grenade(grenade);
			}
		}
	);

	const auto unregister_grenade = game_world->get_method_from_name("\uE000", -1)
		->get_function_pointer< std::uint8_t* >();

	mgrs.hook_mgr.hooks.on_unregister_grenade.hook(
		{ reinterpret_cast<std::uintptr_t>(unregister_grenade),
			[](void* game_world, structures::throwable_t* grenade)
			{
				mgrs.feature_mgr.globals.destroy_grenade(grenade);

				mgrs.hook_mgr.hooks.on_unregister_grenade.call_original(game_world, grenade);
			}
		}
	);

	const auto loot_item = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.Interactive", "LootItem");

	const auto loot_item_init = loot_item->get_method_from_name("Init", -1)
		->get_function_pointer< std::uint8_t* >();

	mgrs.hook_mgr.hooks.on_loot_item_init.hook(
		{ reinterpret_cast<std::uintptr_t>(loot_item_init),
			[](structures::loot_item_t* loot_item, structures::item_t* item, void* itemName, void* gameWorld, bool randomRotation, void* validProfiles, void* staticId, bool performPickUpValidation)
			{
				const auto res = mgrs.hook_mgr.hooks.on_loot_item_init.call_original(loot_item, item, itemName, gameWorld, randomRotation, validProfiles, staticId, performPickUpValidation);


				const auto item_template = item->get_item_template();
				if (!item_template)
					return res;

				const auto template_id = item_template->get_template_id();
				if (template_id) {
					if (game::loot_data::data.contains(template_id->get())) {
						auto val = game::loot_data::data.at(template_id->get());

						auto entry = structures::loot_entry_t{ loot_item, item, val };
						mgrs.feature_mgr.globals.push_loot(entry);
					}
				}

				return res;
			}
		}
	);

	const auto remove_loot_item = loot_item->get_method_from_name("RemoveLootItem", -1)
		->get_function_pointer< std::uint8_t* >();

	mgrs.hook_mgr.hooks.on_remove_loot_item.hook(
		{ reinterpret_cast<std::uintptr_t>(remove_loot_item),
			[](structures::loot_item_t* loot_item, structures::loot_command_t* args)
			{
				mgrs.feature_mgr.globals.destroy_loot(loot_item);

				mgrs.hook_mgr.hooks.on_remove_loot_item.call_original(loot_item, args);
			}
		}
	);

	const auto on_exfil_awake = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.Interactive", "ExfiltrationPoint")->get_method_from_name("Awake", -1)
		->get_function_pointer< std::uint8_t* >();

	mgrs.hook_mgr.hooks.on_exfil_awake.hook(
		{ reinterpret_cast<std::uintptr_t>(on_exfil_awake),
			[](structures::exfiltration_point_t* exfil)
			{
				mgrs.hook_mgr.hooks.on_exfil_awake.call_original(exfil);

				mgrs.feature_mgr.globals.push_exfil(exfil);
			}
		}
	);

	const auto on_exfil_destroy = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("EFT.Interactive", "ExfiltrationPoint")->get_method_from_name("OnDestroy", -1)
		->get_function_pointer< std::uint8_t* >();

	mgrs.hook_mgr.hooks.on_exfil_destroy.hook(
		{ reinterpret_cast<std::uintptr_t>(on_exfil_destroy),
			[](structures::exfiltration_point_t* exfil)
			{
				mgrs.feature_mgr.globals.destroy_exfil(exfil);

				mgrs.hook_mgr.hooks.on_exfil_destroy.call_original(exfil);
			}
		}
	);

	const auto update = mono::structures::image_t::get_image_by_name( "Assembly-CSharp" )
		->get_class( "EFT", "GameWorld" )->get_method_from_name( "Update", -1 )
			->get_function_pointer< std::uint8_t* >( );

	mgrs.hook_mgr.hooks.on_game_world_update.hook(
		{ reinterpret_cast< std::uintptr_t >( update ),
			[ ] ( void* game_world )
			{
				mgrs.hook_mgr.hooks.on_game_world_update.call_original( game_world );

				//need to figure out a good hook for camera it becomes valid so much later
				if (mgrs.feature_mgr.in_game && !mgrs.feature_mgr.globals.get_camera())
					mgrs.feature_mgr.globals.set_camera(structures::unity::camera_t::get());

				collector::collect_closest_player( );

				if (mgrs.feature_mgr.in_game)
				{
					const auto callbacks = mgrs.feature_mgr.globals.get_update_queue();

					for (const auto callback : callbacks)
						callback();

					mgrs.feature_mgr.globals.clear_update_queue();
				}
			}
		}
	);

	const auto camera = mono::structures::image_t::get_image_by_name("UnityEngine.CoreModule")
		->get_class("UnityEngine", "Camera");

	const auto fire_on_post_render = camera->get_method_from_name("FireOnPostRender", -1)
		->get_function_pointer< std::uint8_t* >();

	mgrs.hook_mgr.hooks.on_post_render.hook(
		{ reinterpret_cast<std::uintptr_t>(fire_on_post_render),
			[](void* camera)
			{
				if (!camera || !mgrs.feature_mgr.in_game || !mgrs.feature_mgr.globals.get_camera())
				{
					return mgrs.hook_mgr.hooks.on_post_render.call_original(camera);
				}

				mgrs.feature_mgr.globals.set_camera_pos(
					mgrs.feature_mgr.globals.get_camera()->get_transform()->get_position()
				);
				managers::mgrs.feature_mgr.visual_callback();

				return mgrs.hook_mgr.hooks.on_post_render.call_original(camera);
			}
		}
	);
}