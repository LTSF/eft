#pragma once

#include <functional>

#include "../menu.hpp"

namespace menu::elements
{
	enum button_flags
	{
		BUTTON_PURPLE,
		BUTTON_GREEN,
		BUTTON_GRAY
	};

	namespace static_vars
	{
		inline std::string current_tab = ( "LEGIT" );
	}

	namespace functions
	{
		extern void tab(const char* label, int& tab, int index, std::function<void()> cb = nullptr);

		extern bool begin_child(const char* str_id, const ImVec2 size_arg, bool border = false, ImGuiWindowFlags extra_flags = NULL);
		extern void end_child();

		extern void checkbox(const char* label, const char* desc, bool& v);
		extern void separator();

		extern bool hotkey(const char* label, int* k);
		extern bool hotkey_stretched(const char* label, int* k);

		extern void information(const char* label, const char* desc, float p_data, const char* format);
		extern void information(const char* label, const char* desc, int p_data, const char* format);
		extern void information(const char* label, const char* desc, const char* p_data, const char* icon);
		extern void information(const char* label, const char* desc);

		extern bool slider_float(const char* label, float* v, float v_min, float v_max, const char* format, float power);
		extern bool slider_int(const char* label, int* v, int v_min, int v_max, const char* format);
		extern bool slider_float_stretched(const char* label, float* v, float v_min, float v_max, const char* format, float power);
		extern bool slider_int_stretched(const char* label, int* v, int v_min, int v_max, const char* format);

		extern bool begin_combo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
		extern void end_combo();

		extern bool combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
		extern bool combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);
		extern bool combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);

		extern void color_picker(const char* identifier, float* col);
		extern bool button(const char* label, ImVec2 size_arg, ImGuiButtonFlags flags);
		extern bool config_button(const char* label, const char* data, const char* author);

		extern bool tab(const char* label, const char* icon, int& tab, int index);
		extern bool subtab_static(const char* label, const char* icon, int& tab, int index);
	}
}