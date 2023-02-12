#include "chams.hpp"

#include <managers/managers.hpp>

using namespace managers;

void features::visuals::chams::callback( structures::player_t* player )
{
	if (!mgrs.cfg_mgr.get<bool>(features_t::chams_esp))
		return;

	for ( const auto renderers = player->get_player_body( )->get_renderers( ); const auto renderer : renderers )
	{
		if ( const auto material = renderer->get_material( ) )
		{
			if ( !mgrs.feature_mgr.shaders.cham_shader )
			{
				mgrs.feature_mgr.shaders.cham_shader = structures::unity::shader_t::find_shader( L"Hidden/Internal-Colored" );
			}

			const auto current_shader = material->get_shader();

			if (current_shader != mgrs.feature_mgr.shaders.cham_shader)
				mgrs.feature_mgr.globals.cache_shader(player, current_shader);

			material->set_shader( mgrs.feature_mgr.shaders.cham_shader );

			material->set_int( L"_SrcBlend", 5 );
			material->set_int( L"_DstBlend", 10 );
			material->set_int( L"_Cull", 0 );
			material->set_int( L"_ZTest", 8 );
			material->set_int( L"_ZWrite", 0 );
			material->set_color( L"_Color", { 0.537254902, 0.768627451, 0.9568627451, 1 } );
		}
	}
}

void features::visuals::chams::uncham(structures::player_t* player)
{
	for (const auto renderers = player->get_player_body()->get_renderers(); const auto renderer : renderers)
	{
		if (const auto material = renderer->get_material())
		{
			if( const auto old_shader = mgrs.feature_mgr.globals.get_old_shader(player))
				material->set_shader(old_shader);
		}
	}
}
