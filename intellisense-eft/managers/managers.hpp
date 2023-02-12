#pragma once

#include <dependencies/common_includes.hpp>

#include <managers/interface/interface.hpp>
#include <managers/offset/offset.hpp>
#include <managers/hook/hook.hpp>
#include <managers/config/config.hpp>
#include <managers/menu/menu.hpp>
#include <managers/singleton.hpp>
#include <managers/mono/mono.hpp>
#include <managers/feature/feature.hpp>

namespace managers
{
	class managers_t
	{
	public:
		interface_manager_t& interface_mgr = singleton_t<interface_manager_t>::get( );

		offset_manager_t& offset_mgr = singleton_t<offset_manager_t>::get( );

		hook_manager_t& hook_mgr = singleton_t<hook_manager_t>::get( );
		menu_manager_t& menu_mgr = singleton_t<menu_manager_t>::get( );

		config_manager_t& cfg_mgr = singleton_t<config_manager_t>::get( );

		feature_manager_t& feature_mgr = singleton_t<feature_manager_t>::get( );
	};

	const managers_t mgrs;
}