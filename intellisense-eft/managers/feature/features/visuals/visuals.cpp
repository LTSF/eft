#include "visuals.hpp"

#include <managers/managers.hpp>
#include <game/structures/structures.hpp>

#include <string>

using namespace managers;

void features::visuals::draw_box( structures::player_t* player )
{
	const auto root_pos = player->get_bone_transform( structures::tarkov::bones::root )
		->get_position( );

	const auto head_transform = player->get_bone_transform(structures::tarkov::bones::head);
		
	const auto head_pos = head_transform->get_position( );

	const auto root_sp = mgrs.feature_mgr.globals.get_camera()->world_to_screen( root_pos );
	const auto head_sp = mgrs.feature_mgr.globals.get_camera()->world_to_screen( head_pos );

	if ( root_sp && head_sp ) /* Temporary math */
	{
		const float width = std::fabs( head_sp->y - root_sp->y ) / 4.0f;
		const float height = std::fabs( head_sp->y - root_sp->y ) * 1.2f;

		if (isnan(width) || isnan(height))
			return;

		const auto line_len_width = width / 2.f;

		switch (mgrs.cfg_mgr.get<int>(features_t::esp_type))
		{
			case esp_type_t::bounding:
			{
				mgrs.interface_mgr.m_draw.rect(ImVec2{ root_sp->x - width - 1, root_sp->y + 1 }, ImVec2{ root_sp->x + width + 1, root_sp->y - height - 1 }, ImColor{ 0, 0, 0, 192 });
				mgrs.interface_mgr.m_draw.rect(ImVec2{ root_sp->x - width + 1, root_sp->y - 1 }, ImVec2{ root_sp->x + width - 1, root_sp->y - height + 1 }, ImColor{ 0, 0, 0, 192 });
				mgrs.interface_mgr.m_draw.rect(ImVec2{ root_sp->x - width, root_sp->y }, ImVec2{ root_sp->x + width, root_sp->y - height }, ImColor{ 255, 255, 255, 255 });
				break;
			}
			case esp_type_t::corner:
			{
				mgrs.interface_mgr.m_draw.line(ImVec2{ root_sp->x - width, root_sp->y - height }, ImVec2{ root_sp->x - width + line_len_width, root_sp->y - height }, ImColor{ 255, 255, 255 });
				mgrs.interface_mgr.m_draw.line(ImVec2{ root_sp->x - width, root_sp->y - height }, ImVec2{ root_sp->x - width, (root_sp->y - height) + line_len_width }, ImColor{ 255, 255, 255 });

				mgrs.interface_mgr.m_draw.line(ImVec2{ root_sp->x + width, root_sp->y - height }, ImVec2{ root_sp->x + width - line_len_width, root_sp->y - height }, ImColor{ 255, 255, 255 });
				mgrs.interface_mgr.m_draw.line(ImVec2{ root_sp->x + width, root_sp->y - height }, ImVec2{ root_sp->x + width, (root_sp->y - height) + line_len_width }, ImColor{ 255, 255, 255 });

				mgrs.interface_mgr.m_draw.line(ImVec2{ root_sp->x - width, root_sp->y }, ImVec2{ root_sp->x - width + line_len_width, root_sp->y }, ImColor{ 255, 255, 255 });
				mgrs.interface_mgr.m_draw.line(ImVec2{ root_sp->x - width, root_sp->y }, ImVec2{ root_sp->x - width, root_sp->y - line_len_width }, ImColor{ 255, 255, 255 });

				mgrs.interface_mgr.m_draw.line(ImVec2{ root_sp->x + width, root_sp->y }, ImVec2{ root_sp->x + width - line_len_width, root_sp->y }, ImColor{ 255, 255, 255 });
				mgrs.interface_mgr.m_draw.line(ImVec2{ root_sp->x + width, root_sp->y }, ImVec2{ root_sp->x + width, root_sp->y - line_len_width }, ImColor{ 255, 255, 255 });
				break;
			}
		}

		if (mgrs.cfg_mgr.get<bool>(features_t::health_bar_esp)) {
			structures::unity::vector2_t health = player->get_health();
			if (health.empty())
				health = { 1.f, 1.f };

			float percentage = health.x / health.y;

			mgrs.interface_mgr.m_draw.rect_filled(ImVec2{ root_sp->x - width - 6, root_sp->y+2 }, ImVec2{ root_sp->x - width - 2, root_sp->y - height-2 }, ImColor{ 0, 0, 0, 192 });

			ImVec2 health_top = ImVec2{ root_sp->x - width - 3, root_sp->y - ((height+1)*percentage) };
			mgrs.interface_mgr.m_draw.rect_filled(ImVec2{ root_sp->x - width - 5, root_sp->y+1 }, health_top, ImColor{ 0, 255, 0, 255 });
		
			if (percentage < 0.95f && percentage > 0.05f) 
				mgrs.interface_mgr.m_draw.text(health_top, ImColor{ 255, 255, 255, 192 }, std::to_string((int)(percentage * 100)), true, true);
		}

		const auto profile = player->get_profile();
		const auto info = profile->get_profile_info();
		const auto profile_settings = info->get_settings();

		if (mgrs.cfg_mgr.get<bool>(features_t::name_esp)) {

			structures::unity::str_t* nickname = info->get_nickname();
			structures::e_player_side side = info->get_side();

			std::string name = "player";
			bool is_scav = info->is_scav();
			if (is_scav) {
				const auto reg_date = info->get_registration_date();
				if (reg_date > 0)
					name += " scav";
				else {
					name = profile_settings->get_scav_name();
				}
			} else if (nickname)
				name = nickname->convert();
			mgrs.interface_mgr.m_draw.text({ root_sp->x, root_sp->y - height - 15 }, ImColor(255, 255, 255, 255), name, true, true);
		}

		const auto procedural_weapon_animation = player->get_procedural_weapon_animation();

		if (mgrs.cfg_mgr.get<bool>(features_t::weapon_esp)) {
			auto firearm_controller = procedural_weapon_animation->get_firearm_controller();
			if (firearm_controller) {
				auto item = firearm_controller->get_item();
				if (item) {
					auto localized_name = item->localized_short_name();
					std::string name = "no item";
					if (localized_name)
						name = localized_name->convert();
					mgrs.interface_mgr.m_draw.text({ root_sp->x, root_sp->y + 1 }, ImColor(255, 255, 255, 255), name, true, true);
				}
			}
		}

		if (mgrs.cfg_mgr.get<bool>(features_t::distance_esp)) {
			float distance = mgrs.feature_mgr.globals.get_camera_pos().distance(root_pos);

			mgrs.interface_mgr.m_draw.text({ root_sp->x, root_sp->y + 12 }, ImColor(255, 255, 255, 255), std::to_string((int)distance)+"m", true, true);
		}
	}
}

void features::visuals::draw_snapline( )
{
	if (!structures::unity::input_t::get_key(structures::unity::e_keycodes::mouse4))
		return;

	const auto camera = mgrs.feature_mgr.globals.get_camera();

	const auto lp = mgrs.feature_mgr.globals.get_local_player();

	const auto lp_rotation = lp->get_rotation();

	if (const auto closest_player = managers::mgrs.feature_mgr.globals.get_closest_player( ) )
	{

		const auto head_transform = closest_player
			->get_bone_transform(structures::tarkov::bones::head);
			
		const auto head_pos = head_transform->get_position( );

		const auto head_sp = camera->world_to_screen( head_pos );

		if ( head_sp )
		{
			const auto fov = closest_player->get_fov_to(structures::tarkov::bones::head, lp_rotation);

			if (fov < mgrs.cfg_mgr.get<float>(features_t::silent_aim_fov))
			{
				/* Todo, using gunlength and direction calculate the point of the barrel(looks much nicer than fireport cuz fireport doesnt constantly udpated)*/
				const auto dimensions = structures::unity::screen_t::get_dimensions();

				const auto fireport = lp->get_fireport();

				if (fireport)
				{
					if (const auto fireport_sp = camera->world_to_screen(fireport->get_position()))
					{
						const auto cam_transform = camera->get_transform();

						const auto cam_pos = cam_transform->get_position();

						if (cam_transform)
						{
							structures::unity::raycast_hit_t hit;

							if (mgrs.cfg_mgr.get<bool>(features_t::silent_aim_linecast) && structures::unity::physics_t::linecast(cam_pos, head_pos, &hit, structures::tarkov::layers::HighPoly | structures::tarkov::layers::HitCollider | structures::tarkov::layers::LowPoly | structures::tarkov::layers::Interactive))
								//if (hit.get_transform() != head_transform)
									return;

							mgrs.interface_mgr.m_draw.line(ImVec2{ fireport_sp->x, fireport_sp->y }, ImVec2{ head_sp->x, head_sp->y }, ImColor{ 255, 255, 255 });
						}
					}
				}
			}
		}
	}
}

void features::visuals::draw_grenade( structures::throwable_t* grenade )
{
	if ( !grenade )
		return;

	if ( const auto rigid_body = grenade->get_rigid_body( ) )
	{
		const auto position = rigid_body->get_position( );

		if ( const auto sp = managers::mgrs.feature_mgr.globals.get_camera()->world_to_screen(position))
			managers::mgrs.interface_mgr.m_draw.text(ImVec2{ sp->x, sp->y }, ImColor { 255, 80, 80 }, "Grenade", true, true );
	}
}

void features::visuals::draw_loot(structures::loot_entry_t loot) {
	if (!mgrs.cfg_mgr.get<bool>(features_t::loot_esp))
		return;

	if (!loot.item || !loot.loot_item)
		return;

	if (mgrs.cfg_mgr.get<int>(features_t::loot_esp_min_value) > loot.price)
		return;

	if (const auto transform = loot.loot_item->get_transform())
	{
		const auto position = transform->get_position();

		if (const auto sp = managers::mgrs.feature_mgr.globals.get_camera()->world_to_screen(position)) {
			const auto localized_name = loot.item->localized_short_name();

			std::string text = localized_name->convert() + " [" + std::to_string(loot.price / 1000) + "k]";

			managers::mgrs.interface_mgr.m_draw.text(ImVec2{ sp->x, sp->y }, ImColor{ 80, 80, 255 }, text, true, true);
		}
	}
}

void features::visuals::draw_exfil( structures::exfiltration_point_t* exfil )
{
	if ( !exfil )
		return;

	if ( const auto transform = exfil->get_transform( ) )
	{
		const auto position = transform->get_position( );

		ImColor col = ImColor{ 0, 145, 0 };

		switch (exfil->get_status()) {
		case structures::e_exfiltration_status::pending:
		case structures::e_exfiltration_status::not_present:
			return;
		case structures::e_exfiltration_status::uncompleted_requirements:
		case structures::e_exfiltration_status::countdown:
			col = ImColor{ 184, 120, 51 };
			break;
		}

		const auto name = exfil->get_settings( )->get_name( )->convert( );

		if ( const auto sp = managers::mgrs.feature_mgr.globals.get_camera()->world_to_screen(position))
			managers::mgrs.interface_mgr.m_draw.text(ImVec2{ sp->x, sp->y }, col, name, true, true );
	}
}

void features::visuals::draw_fov()
{
	const auto dimensions = structures::unity::screen_t::get_dimensions();

	const auto camera = mgrs.feature_mgr.globals.get_camera();

	if (!camera)
		return;

	float camera_fov = camera->get_fov();
	float aimbot_fov = mgrs.cfg_mgr.get<float>(features_t::silent_aim_fov);

	const auto aspect_ratio = camera->get_aspect_ratio();

	const auto fov_h_rad = 2 * atan(tan(DEG2RAD(camera_fov / 2.f)) * aspect_ratio);
	const auto radius = tan(DEG2RAD(aimbot_fov)) / tan(fov_h_rad / 2.f) * (dimensions.x/2);
	//float radius = (tan(DEG2RAD(aimbot_fov) / 2)) / (tan(DEG2RAD(camera_fov) / 2)) * (dimensions.x/2);

	mgrs.interface_mgr.m_draw.circle(
		ImVec2{ dimensions.x / 2.f, dimensions.y / 2.f },
		radius,
		ImColor{ 255, 255, 255 }
	);
}

void features::visuals::callback( )
{
	const auto lp = managers::mgrs.feature_mgr.globals.get_local_player();

	if ( managers::mgrs.feature_mgr.in_game && lp )
	{
		managers::mgrs.interface_mgr.m_draw.begin();

		draw_snapline();

		for (const auto grenades = managers::mgrs.feature_mgr.globals.get_grenades(); const auto grenade : grenades)
			draw_grenade(grenade);

		for (const auto exfils = managers::mgrs.feature_mgr.globals.get_exfils(); const auto exfil : exfils)
			draw_exfil(exfil);

		for (const auto items = managers::mgrs.feature_mgr.globals.get_loot(); const auto entry : items)
			draw_loot(entry);

		for ( const auto players = managers::mgrs.feature_mgr.globals.get_players( ); const auto player : players )
		{
			if ( !player || lp == player )
				continue;

			draw_box( player );
		}

		draw_fov();

		managers::mgrs.interface_mgr.m_draw.end();
	}
}