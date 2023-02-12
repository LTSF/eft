#include "no_visor.hpp"

#include <managers/managers.hpp>
using namespace managers;

void features::visuals::no_visor::callback()
{
	static const auto get_instance = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("", "\uE7CC")->get_method_from_name("get_Instance", -1) //updated 8/11/2022
		->get_function_pointer< void* (*)(void) >();

	void* object_manager = get_instance(); // i have zero fucking idea what this should be called
	if (!object_manager)
		return;

	static const auto get_visor_effect = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("", "\uE7CC")->get_method_from_name("get_VisorEffect", -1) //updated 8/11/2022
		->get_function_pointer< void* (*)(void*) >();

	void* visor_effect = get_visor_effect(object_manager);
	if (!visor_effect)
		return;

	static const auto set_visible = mono::structures::image_t::get_image_by_name("Assembly-CSharp")
		->get_class("", "VisorEffect")->get_method_from_name("set_Visible", -1)
		->get_function_pointer< void (*)(void*, bool) >();

	set_visible(visor_effect, !mgrs.cfg_mgr.get<bool>(features_t::no_visor));
}
