#include "menu.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_internal.h>

#include <managers/managers.hpp>

#include <imgui/imgui_memory_editor.h>

#include "../managers/feature/features/visuals/chams/chams.hpp"
#include <dependencies/font/hashes.h>
#include <dependencies/font/poppins.hpp>
#include <dependencies/font/faprolight.hpp>
#include <dependencies/font/drukwidebold.hpp>
#include <managers/menu/image/image.hpp>
#include <dependencies/imgui/textures/player.hpp>
#include <dependencies/imgui/textures/icons/intellisense.hpp>
#include <managers/menu/elements/elements.hpp>

void managers::menu_manager_t::set_style( )
{
	ImGui::GetStyle().WindowPadding = { 0u,0u };
	ImGui::GetStyle().WindowBorderSize = 1u;
	ImGui::GetStyle().PopupBorderSize = 0u;
	ImGui::GetStyle().PopupRounding = 5;
	ImGui::GetStyle().WindowRounding = 8;
	ImGui::GetStyle().ScrollbarSize = 4;

	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImColor{ 18, 25, 35 };
	ImGui::GetStyle().Colors[ImGuiCol_Border] = ImColor{ 28, 37, 54 };

	ImGui::GetStyle().Colors[ImGuiCol_PopupBg] = ImColor{ 33, 33, 33 };
	ImGui::GetStyle().Colors[ImGuiCol_Text] = ImColor{ 119, 119, 119 };
	ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg] = ImColor{ 119, 119, 119, 0u };

	ImGui::GetStyle().Colors[ImGuiCol_Header] = ImColor{ 35, 42, 51 };
	ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] = ImColor{ 35, 42, 51 };
	ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] = ImColor{ 35, 42, 51 };

	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0u };

	static const ImWchar ranges[] = {
	0x0020, 0x00FF,0x2000, 0x206F,0x3000, 0x30FF,0x31F0, 0x31FF, 0xFF00,
	0xFFEF,0x4e00, 0x9FAF,0x0400, 0x052F,0x2DE0, 0x2DFF,0xA640, 0xA69F, 0
	};

	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;

	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(poppins_compressed_data, poppins_compressed_size, 14, NULL, ranges);
	ImGui::GetIO().Fonts->AddFontFromMemoryTTF(faprolight, sizeof faprolight, 22, &icons_config, icon_ranges);

	ImGui::GetIO().IniFilename = NULL;

	managers::mgrs.interface_mgr.fonts.pro_font_windows = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(visitor_font, sizeof visitor_font, 18, NULL, ranges);
	managers::mgrs.interface_mgr.fonts.druk_wide_bold = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(druk_wide_bold_compressed_data, druk_wide_bold_compressed_size, 30u, NULL, ranges);

	managers::mgrs.interface_mgr.fonts.px18 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(poppins_compressed_data, poppins_compressed_size, 18, NULL, ranges);
	managers::mgrs.interface_mgr.fonts.px24 = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(poppins_compressed_data, poppins_compressed_size, 24, NULL, ranges);

	image::d3dx::LoadTextureFromMemory(managers::mgrs.interface_mgr.global_device, player_texture, sizeof player_texture, &image::d3dx::player_image, nullptr, nullptr);
	image::d3dx::LoadTextureFromMemory(managers::mgrs.interface_mgr.global_device, intellisense_icon, sizeof intellisense_icon, &image::d3dx::intellisense_icon, nullptr, nullptr);
}

static auto current_tab = 0;

void Draw_Main()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });

	auto DrawList = ImGui::GetWindowDrawList();
	auto Position = ImGui::GetWindowPos();

	ImGui::SetCursorPos({ 20, 10 });

	ImGui::BeginChild(("Main"), { 760, 500 }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	{
		switch (current_tab)
		{
		case 0:
			ImGui::SetCursorPos({ 0, 25 });

			if (menu::elements::functions::begin_child(("GENERAL"), { 205, 430 }))
			{
				menu::elements::functions::checkbox("Silent Aim", "Silent Aim", managers::mgrs.cfg_mgr.get<bool>(features_t::silent_aim));
				menu::elements::functions::checkbox("Silent Aim LOS Check", "Silent Aim LOS Check", managers::mgrs.cfg_mgr.get<bool>(features_t::silent_aim_linecast));
				menu::elements::functions::slider_float("Silent Aim FOV", &managers::mgrs.cfg_mgr.get<float>(features_t::silent_aim_fov), 0, 100, "%f", 1.f);
			} menu::elements::functions::end_child();

			ImGui::SetCursorPos({ 220, 25 });

			if (menu::elements::functions::begin_child(("CONFIG"), { 205, 430 }))
			{
				/*menu::elements::functions::checkbox(xs("ENABLE AIMBOT"), xs("enables aimbot"), std::get< bool >(menu::config::config_map[features::enable_no_recoil]));
				menu::elements::functions::checkbox(xs("ENABLE SMOOTHING"), xs("enables smoothing"), std::get< bool >(menu::config::config_map[features::enable_no_recoil]));
				menu::elements::functions::checkbox(xs("ENABLE NIGGA BALLS"), xs("enables aimbot"), std::get< bool >(menu::config::config_map[features::enable_no_recoil]));
				menu::elements::functions::checkbox(xs("ENABLE RAT"), xs("enables aimbot"), std::get< bool >(menu::config::config_map[features::enable_no_recoil]));

				menu::elements::functions::slider_int(xs("SLIDER"), &lol, 0, 100, "%i");

				menu::elements::functions::combo("BONE", &std::get< int >(menu::config::config_map[features::assist_bone]), "head\0\rneck\0\rchest\0\0", 4);

				ImGui::Spacing(5);

				menu::elements::functions::checkbox(xs("FOV"), xs("enables aimbot"), std::get< bool >(menu::config::config_map[features::enable_no_recoil]));

				ImGui::ColorEdit4("CUSTOM COLOR", color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);

				menu::elements::functions::button("BUTTON", ImVec2(185, 22), 0);*/
			} menu::elements::functions::end_child();


			break;
		case 1:

			ImGui::SetCursorPos({ 500, 25 });

			if (menu::elements::functions::begin_child(("PREVIEW"), { 205, 430 }))
			{
				ImGui::GetCurrentWindow()->DrawList->AddImageRounded(image::d3dx::player_image, Position + ImVec2(475, 60), Position + ImVec2(765, 465), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255), 17, 15);
			} menu::elements::functions::end_child();


			break;
		case 3:

			break;
		case 4:

			break;
		default:
			break;
		}
	}
	ImGui::EndChild();

	ImGui::PopStyleVar();
}


void managers::menu_manager_t::render( )
{
	ImGui::SetNextWindowSize({ 60.5, 500 }, ImGuiCond_Once);

	ImVec2 DashPosition = { 0, 0 };

	ImGui::Begin(("Dashboard"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	{
		auto DrawList = ImGui::GetWindowDrawList();
		DashPosition = ImGui::GetWindowPos();

		DrawList->AddImageRounded(image::d3dx::intellisense_icon,
			DashPosition + ImVec2{ 15, 10 },
			DashPosition + ImVec2{ 40, 35 }, ImVec2{ 0, 0 }, ImVec2{ 1, 1 },
			ImColor{ 255, 255, 255 }, 0, 15);

		ImGui::SetCursorPos({ 5, 100 });

		ImGui::BeginGroup();
		{
			menu::elements::functions::tab(("AIMBOT"), (const char*)ICON_FA_BULLSEYE_ARROW, current_tab, 0);

			ImGui::Spacing();

			menu::elements::functions::tab(("VISUALS"), (const char*)ICON_FA_EYE, current_tab, 1);

			ImGui::Spacing();

			menu::elements::functions::tab(("MISC"), (const char*)ICON_FA_ARCHIVE, current_tab, 2);

			ImGui::Spacing();

			menu::elements::functions::tab(("WEAPON"), (const char*)ICON_FA_BOW_ARROW, current_tab, 3);

			ImGui::Spacing();

			menu::elements::functions::tab(("CFG"), (const char*)ICON_FA_COGS, current_tab, 4);

			//ImGui::Spacing(55);

			menu::elements::functions::tab(("EXIT"), (const char*)ICON_FA_SIGN_OUT, current_tab, 5);

			ImGui::Spacing();
		}
		ImGui::EndGroup();
	} ImGui::End();

	ImGui::SetNextWindowSize({ 760, 500 }, ImGuiCond_Once);

	ImGui::Begin(("EscapeFromTarkov"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	{
		ImGui::SetWindowPos(DashPosition + ImVec2{ 65, 0 }, ImGuiCond_Always);
		{
			Draw_Main();
		}
	}
	ImGui::End();

	/*ImGui::SetNextWindowSize( { 700, 500 }, ImGuiCond_Once );
	if ( ImGui::Begin( "Main", nullptr ) )
	{
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("tabbar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Aim"))
			{
				ImGui::Checkbox("Silent Aim", &mgrs.cfg_mgr.get<bool>(features_t::silent_aim));
				ImGui::Checkbox("Silent Aim LOS Check", &mgrs.cfg_mgr.get<bool>(features_t::silent_aim_linecast));
				ImGui::SliderFloat("Silent Aim FOV", &mgrs.cfg_mgr.get<float>(features_t::silent_aim_fov), 0, 100);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Visuals"))
			{
				ImGui::Combo("ESP Type", &mgrs.cfg_mgr.get<int>(features_t::esp_type), "none\0filled\0bounding\0corner\0");

				if (ImGui::Checkbox("Chams", &mgrs.cfg_mgr.get<bool>(features_t::chams_esp)))
				{
					const auto toggle = mgrs.cfg_mgr.get<bool>(features_t::chams_esp);

					mgrs.feature_mgr.globals.add_to_update_queue([toggle]() {
						const auto players = mgrs.feature_mgr.globals.get_players();

						for (const auto player : players)
						{
							if (player)
								toggle ? features::visuals::chams::callback(player) : features::visuals::chams::uncham(player);
						}
					});
				}

				ImGui::Checkbox("Name ESP", &mgrs.cfg_mgr.get<bool>(features_t::name_esp));
				ImGui::Checkbox("Health Bar", &mgrs.cfg_mgr.get<bool>(features_t::health_bar_esp));
				ImGui::Checkbox("Distance ESP", &mgrs.cfg_mgr.get<bool>(features_t::distance_esp));
				ImGui::Checkbox("Weapon ESP", &mgrs.cfg_mgr.get<bool>(features_t::weapon_esp));

				ImGui::Checkbox("Loot ESP", &mgrs.cfg_mgr.get<bool>(features_t::loot_esp));
				ImGui::SliderInt("Minimum Value", &mgrs.cfg_mgr.get<int>(features_t::loot_esp_min_value), 0, 250000);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc"))
			{
				ImGui::Checkbox("Infinite Stamina", &mgrs.cfg_mgr.get<bool>(features_t::infinite_stamina));
				ImGui::Checkbox("Run heal", &mgrs.cfg_mgr.get<bool>(features_t::run_heal));
				ImGui::Checkbox("No Bush Slowdown", &mgrs.cfg_mgr.get<bool>(features_t::remove_barbed));
				ImGui::Checkbox("Owl Look", &mgrs.cfg_mgr.get<bool>(features_t::owl_look));

				ImGui::SliderFloat("FOV Offset", &mgrs.cfg_mgr.get<float>(features_t::fov_offset), 0, 50);
				ImGui::Checkbox("No Visor", &mgrs.cfg_mgr.get<bool>(features_t::no_visor));
				ImGui::SliderInt("Aspect Ratio", &mgrs.cfg_mgr.get<int>(features_t::aspect_ratio), 0, 100);
				ImGui::Checkbox("Enable Time Changer", &mgrs.cfg_mgr.get<bool>(features_t::time_changer_enable));
				ImGui::SliderFloat("Time Changer Hour", &mgrs.cfg_mgr.get<float>(features_t::time_changer_hour), 0.f, 24.f);

				ImGui::Checkbox("Always Shoot", &mgrs.cfg_mgr.get<bool>(features_t::always_shoot));
				ImGui::Checkbox("Anti Malfunction", &mgrs.cfg_mgr.get<bool>(features_t::anti_malfunction));
				ImGui::Checkbox("No Recoil", &mgrs.cfg_mgr.get<bool>(features_t::no_recoil));
				ImGui::Checkbox("No Sway", &mgrs.cfg_mgr.get<bool>(features_t::no_sway));
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End( );*/
}