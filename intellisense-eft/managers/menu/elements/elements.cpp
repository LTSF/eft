#include <map>

#include "elements.hpp"
#include "../../../dependencies/imgui/tricks/tricks.hpp"

#include <managers/managers.hpp>

struct TabStruct {
	float size;
	float hovered;
};

bool menu::elements::functions::tab(const char* label, const char* icon, int& tab, int index)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImGuiID id = window->GetID(label);

	static std::map<ImGuiID, TabStruct>* circle_anim;
	if (!circle_anim)
	{
		circle_anim = new std::map<ImGuiID, TabStruct>();
	}
	auto it_circle = circle_anim->find(id);
	if (it_circle == circle_anim->end())
	{
		circle_anim->insert({ id, {50, 0} });
		it_circle = circle_anim->find(id);
	}

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = ImGui::CalcItemSize({ 50, it_circle->second.size }, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	ImRect bb(pos, pos + size);
	bool pressed = ImGui::InvisibleButton(label, size);

	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(7);

	if (pressed)
	{
		tab = index;
		static_vars::current_tab = label;
	}

	bool active = index == tab;
	ImGui::PushID(index);
	it_circle->second.hovered = ImTricks::Animations::FastFloatLerp(window->GetID("hover"), active, 0.f, 1.f, 0.2f);
	ImGui::PopID();

	ImGui::PushClipRect(bb.Min, bb.Max, false);

	window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor(28, 37, 54, int(it_circle->second.hovered * 255)), 5);

	window->DrawList->AddText(NULL, 9, bb.Min + ImVec2(25 - (ImGui::CalcTextSize(icon).x * 0.64f) / 2, 25), ImTricks::Animations::FastColorLerp(
		ImColor(133, 193, 228, 160),
		ImColor(92, 195, 255),
		it_circle->second.hovered), icon);

	ImGui::PopClipRect();


	return true;
}

bool menu::elements::functions::subtab_static(const char* label, const char* icon, int& tab, int index)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImGuiID id = window->GetID(label);

	std::map<ImGuiID, TabStruct> circle_anim;

	auto it_circle = circle_anim.find(id);
	if (it_circle == circle_anim.end())
	{
		circle_anim.insert({ id, {50, 0} });
		it_circle = circle_anim.find(id);
	}

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = ImGui::CalcItemSize({ 74, 66 }, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	ImRect bb(pos, pos + size);
	ImGui::InvisibleButton(label, size);

	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(7);

	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		tab = index;

	it_circle->second.hovered = ImTricks::Animations::FastFloatLerp(window->GetID(std::string(label).append("hover").c_str()), index == tab, 0.f, 1.f, 0.0625f);

	ImGui::PushClipRect(bb.Min, bb.Max, false);

	window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor(32, 39, 49, int(it_circle->second.hovered * 255)), 7);

	window->DrawList->AddText(NULL, 9, bb.Min + ImVec2(37 - 13 / 2, 25), ImTricks::Animations::FastColorLerp(
		ImColor(210, 210, 210, 160),
		ImColor(211, 243, 107),
		it_circle->second.hovered), icon);

	window->DrawList->AddText(NULL, 12, bb.Min + ImVec2(39 - (ImGui::CalcTextSize(label).x * 0.85f) / 2, 45), ImTricks::Animations::FastColorLerp(
		ImColor(210, 210, 210, 160),
		ImColor(211, 243, 107),
		it_circle->second.hovered),
		label);

	ImGui::PopClipRect();
}

void menu::elements::functions::checkbox(const char* label, const char* desc, bool& v)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	const ImRect check_bb(window->DC.CursorPos + ImVec2(5, 0), window->DC.CursorPos + ImVec2(5, 0) + ImVec2(label_size.y + style.FramePadding.y * 2, label_size.y + style.FramePadding.y * 2));
	ImGui::ItemSize(check_bb, style.FramePadding.y);

	ImRect total_bb = check_bb;
	if (label_size.x > 0)
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
	const ImRect text_bb(window->DC.CursorPos + ImVec2(5, 0) + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(5, 0) + ImVec2(0, style.FramePadding.y) + label_size);
	if (label_size.x > 0)
	{
		ImGui::ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
		total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
	}

	if (!ImGui::ItemAdd(total_bb, id))
		return;
	auto aslpha = float(ImGui::GetStyle().Alpha);
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
	if (pressed)
		v = !(v);
	static std::map<ImGuiID, float> hover_animation;
	auto it_hover = hover_animation.find(id);
	if (it_hover == hover_animation.end())
	{
		hover_animation.insert({ id, 0.f });
		it_hover = hover_animation.find(id);
	}
	it_hover->second = ImClamp(it_hover->second + (0.2f * ImGui::GetIO().DeltaTime * (hovered ? 1.f : -5.f)), 0.0f, 0.12f);
	it_hover->second *= aslpha;
	static std::map<ImGuiID, float> filled_animation;
	auto it_filled = filled_animation.find(id);
	if (it_filled == filled_animation.end())
	{
		filled_animation.insert({ id, 0.f });
		it_filled = filled_animation.find(id);
	}
	it_filled->second = ImClamp(it_filled->second + (2.35f * ImGui::GetIO().DeltaTime * ((v) ? 2.f : -2.f)), it_hover->second, 1.f);
	it_filled->second *= aslpha;
	const float check_sz = ImMin(check_bb.GetWidth() - 5, check_bb.GetHeight() - 5);
	const float pad = ImMax(1.0f, (float)(int)(check_sz / 4.0f)); //133, 193, 228, 160 // 92, 195, 255
	ImVec4 text = ImLerp(ImVec4{ 133 / 255.f, 193 / 255.f, 228 / 255.f, 0.6f }, ImVec4{ 92 / 255.f, 195 / 255.f, 255 / 255.f, 1.f }, it_filled->second);
	// RenderFrame(check_bb.Min + ImVec2(pad, pad), check_bb.Max - ImVec2(pad, pad), ImColor(26, 29, 34, int(255 * GetStyle().Alpha)), false, 4);
	window->DrawList->AddCircle(check_bb.Min + ImVec2(pad * 2.75, pad * 2.75), 8, ImColor(text), 0);
	window->DrawList->AddCircleFilled(check_bb.Min + ImVec2(pad * 2.75, pad * 2.75), v || it_filled->second > 0.12f ? 6 * it_filled->second : 0, ImColor(text), 0);
	if (label_size.x > 0.0f) {
		ImGui::GetWindowDrawList()->AddText(managers::mgrs.interface_mgr.fonts.px18, 14, text_bb.GetTL() - ImVec2{ 2, 2 }, ImColor(215, 215, 215, int(155 * ImGui::GetStyle().Alpha)), label);
		ImGui::GetWindowDrawList()->AddText(managers::mgrs.interface_mgr.fonts.px18, 14, text_bb.GetTL() - ImVec2{ 2, 2 }, ImColor(225, 225, 225, int(255 * it_filled->second * ImGui::GetStyle().Alpha)), label);
	}
}

void ImFormatStringToTempBuffer(const char** out_buf, const char** out_buf_end, const char* fmt, ...)
{
	ImGuiContext& g = *GImGui;
	va_list args;
	va_start(args, fmt);
	int buf_len = ImFormatStringV(g.TempBuffer, sizeof(g.TempBuffer), fmt, args);
	*out_buf = g.TempBuffer;
	if (out_buf_end) { *out_buf_end = g.TempBuffer + buf_len; }
	va_end(args);
}

bool BeginChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* parent_window = g.CurrentWindow;

	flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
	flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);  // Inherit the NoMove flag

	// Size
	const ImVec2 content_avail = ImGui::GetContentRegionAvail();
	ImVec2 size = ImFloor(size_arg);
	const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
	if (size.x <= 0.0f)
		size.x = ImMax(content_avail.x + size.x, 4.0f); // Arbitrary minimum child size (0.0f causing too much issues)
	if (size.y <= 0.0f)
		size.y = ImMax(content_avail.y + size.y, 4.0f);

	ImGui::SetNextWindowPos(ImVec2(parent_window->DC.CursorPos.x, parent_window->DC.CursorPos.y + 22.f));
	ImGui::SetNextWindowSize(size - ImVec2(0, 27));

	parent_window->DrawList->AddRectFilled(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, 19), ImColor(20, 29, 43, 255), 3.f);
	parent_window->DrawList->AddRectFilled(parent_window->DC.CursorPos + ImVec2(0, size.y - 35), parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(20, 29, 43, 255), 3.f);
	parent_window->DrawList->AddRectFilled(parent_window->DC.CursorPos + ImVec2(0, 19), parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(20, 29, 43, 255), 3.f);
	parent_window->DrawList->AddRect(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(255, 255, 255, 15), 3.f);
	parent_window->DrawList->AddLine(parent_window->DC.CursorPos + ImVec2(0, 19), parent_window->DC.CursorPos + ImVec2(size.x, 19), ImColor(255, 255, 255, 15));
	parent_window->DrawList->AddText(parent_window->DC.CursorPos + ImVec2(10, 2), ImColor(255, 255, 255, 255), name);

	// Build up name. If you need to append to a same child from multiple location in the ID stack, use BeginChild(ImGuiID id) with a stable value.
	const char* temp_window_name;
	if (name)
		ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id);
	else
		ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id);

	const float backup_border_size = g.Style.ChildBorderSize;
	if (!border)
		g.Style.ChildBorderSize = 0.0f;
	bool ret = ImGui::Begin(temp_window_name, NULL, flags);
	g.Style.ChildBorderSize = backup_border_size;

	ImGuiWindow* child_window = g.CurrentWindow;
	child_window->ChildId = id;
	child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;

	// Set the cursor to handle case where the user called SetNextWindowPos()+BeginChild() manually.
	// While this is not really documented/defined, it seems that the expected thing to do.
	if (child_window->BeginCount == 1)
		parent_window->DC.CursorPos = child_window->Pos;

	// Process navigation-in immediately so NavInit can run on first frame
	if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavHasScroll))
	{
		ImGui::FocusWindow(child_window);
		ImGui::NavInitWindow(child_window, false);
		ImGui::SetActiveID(id + 1, child_window); // Steal ActiveId with another arbitrary id so that key-press won't activate child item
		g.ActiveIdSource = ImGuiInputSource_Nav;
	}
	return ret;
}

bool BeginChild(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	return BeginChildEx(str_id, window->GetID(str_id), size_arg, border, extra_flags);
}

bool BeginChild(ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
	IM_ASSERT(id != 0);
	return BeginChildEx(NULL, id, size_arg, border, extra_flags);
}

void EndChild()
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	IM_ASSERT(g.WithinEndChild == false);
	IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);   // Mismatched BeginChild()/EndChild() calls

	g.WithinEndChild = true;
	if (window->BeginCount > 1)
	{
		ImGui::End();
	}
	else
	{
		ImVec2 sz = window->Size;
		if (window->AutoFitChildAxises & (1 << ImGuiAxis_X)) // Arbitrary minimum zero-ish child size of 4.0f causes less trouble than a 0.0f
			sz.x = ImMax(4.0f, sz.x);
		if (window->AutoFitChildAxises & (1 << ImGuiAxis_Y))
			sz.y = ImMax(4.0f, sz.y);
		ImGui::End();

		ImGuiWindow* parent_window = g.CurrentWindow;
		ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
		ImGui::ItemSize(sz);
		if ((window->DC.NavLayersActiveMask != 0 || window->DC.NavHasScroll) && !(window->Flags & ImGuiWindowFlags_NavFlattened))
		{
			ImGui::ItemAdd(bb, window->ChildId);
			ImGui::RenderNavHighlight(bb, window->ChildId);

			// When browsing a window that has no activable items (scroll only) we keep a highlight on the child
			if (window->DC.NavLayersActiveMask == 0 && window == g.NavWindow)
				ImGui::RenderNavHighlight(ImRect(bb.Min - ImVec2(2, 2), bb.Max + ImVec2(2, 2)), g.NavId, ImGuiNavHighlightFlags_TypeThin);
		}
		else
		{
			// Not navigable into
			ImGui::ItemAdd(bb, 0);
		}
		/*
		if (g.HoveredWindow == window)
			g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
			*/
	}
	g.WithinEndChild = false;
	g.LogLinePosY = -FLT_MAX; // To enforce a carriage return
}

bool menu::elements::functions::begin_child(const char* str_id, const ImVec2 size_arg, bool border, ImGuiWindowFlags extra_flags)
{
	if (!BeginChild(str_id, size_arg, border, extra_flags | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoMove))
		return false;

	ImGui::SetCursorPos({ 10,10 });
	ImGui::BeginGroup();

	return true;
}

void menu::elements::functions::end_child()
{
	ImGui::EndGroup();
	ImGui::EndChild();
}

static const char* PatchFormatStringFloatToInt(const char* fmt)
{
	if (fmt[0] == '%' && fmt[1] == '.' && fmt[2] == '0' && fmt[3] == 'f' && fmt[4] == 0) // Fast legacy path for "%.0f" which is expected to be the most common case.
		return "%d";
	const char* fmt_start = ImParseFormatFindStart(fmt);    // Find % (if any, and ignore %%)
	const char* fmt_end = ImParseFormatFindEnd(fmt_start);  // Find end of format specifier, which itself is an exercise of confidence/recklessness (because snprintf is dependent on libc or user).
	if (fmt_end > fmt_start && fmt_end[-1] == 'f')
	{
		if (fmt_start == fmt && fmt_end[0] == 0)
			return "%d";
		ImGuiContext& g = *GImGui;
		ImFormatString(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), "%.*s%%d%s", (int)(fmt_start - fmt), fmt, fmt_end); // Honor leading and trailing decorations, but lose alignment/precision.
		return g.TempBuffer;
	}
	return fmt;
}

bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, float power, ImGuiSliderFlags flags = 0)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const float w = ImGui::CalcItemWidth();
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	const ImRect frame_bb(window->DC.CursorPos + ImVec2(8, 8), window->DC.CursorPos + ImVec2(window->Size.x - 27, 16 + label_size.y + style.FramePadding.y * 2.0f));
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
		return false;

	// Default format string when passing NULL
	if (format == NULL)
		format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;
	else if (data_type == ImGuiDataType_S32 && strcmp(format, "%d") != 0) // (FIXME-LEGACY: Patch old "%.0f" format string to use "%d", read function more details.)
		format = PatchFormatStringFloatToInt(format);

	// Tabbing or CTRL-clicking on Slider turns it into an input box
	const bool hovered = ImGui::ItemHoverable(frame_bb, id);
	const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
	bool temp_input_is_active = temp_input_allowed && ImGui::TempInputIsActive(id);
	if (!temp_input_is_active)
	{
		const bool focus_requested = temp_input_allowed && ImGui::FocusableItemRegister(window, id);
		const bool clicked = (hovered && g.IO.MouseClicked[0]);
		if (focus_requested || clicked || g.NavActivateId == id || g.NavActivateId == id)
		{
			ImGui::SetActiveID(id, window);
			ImGui::SetFocusID(id, window);
			ImGui::FocusWindow(window);
			g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
			if (temp_input_allowed && (focus_requested || (clicked && g.IO.KeyCtrl) || g.NavActivateId == id))
			{
				temp_input_is_active = true;
				ImGui::FocusableItemUnregister(window);
			}
		}
	}

	if (temp_input_is_active)
	{
		// Only clamp CTRL+Click input when ImGuiSliderFlags_AlwaysClamp is set
		const bool is_clamp_input = (flags & ImGuiSliderFlags_AlwaysClamp) != 0;
		return ImGui::TempInputScalar(frame_bb, id, label, data_type, p_data, format, is_clamp_input ? p_min : NULL, is_clamp_input ? p_max : NULL);
	}
	float deltatime = 1.5f * ImGui::GetIO().DeltaTime;
	static std::map<ImGuiID, float> hover_animation;
	auto it_hover = hover_animation.find(id);
	if (it_hover == hover_animation.end())
	{
		hover_animation.insert({ id, 0.f });
		it_hover = hover_animation.find(id);
	}
	it_hover->second = ImClamp(it_hover->second + (2.15f * ImGui::GetIO().DeltaTime * (hovered || ImGui::IsItemActive() ? 1.f : -1.f)), 0.0f, 1.f);
	it_hover->second *= ImGui::GetStyle().Alpha;

	static std::map<ImGuiID, float> filled_animation;
	auto it_filled = filled_animation.find(id);
	if (it_filled == filled_animation.end())
	{
		filled_animation.insert({ id, 0.f });
		it_filled = filled_animation.find(id);
	}
	it_filled->second = ImClamp(it_filled->second + (1.15f * ImGui::GetIO().DeltaTime), 0.0f, 1.f);
	//it_filled->second *= min(GetStyle().Alpha * 1.5, 1.f);
	// Draw frame
	const ImU32 frame_col = ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : g.HoveredId == id ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
	ImGui::RenderNavHighlight(frame_bb, id);
	window->DrawList->AddRectFilled(ImVec2(frame_bb.Min.x, frame_bb.Min.y + 9), ImVec2(frame_bb.Max.x - 3, frame_bb.Max.y - 3 - 8), ImColor(18, 25, 35), 4);

	// Slider behavior
	ImRect grab_bb;
	const bool value_changed = ImGui::SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
	if (value_changed)
		ImGui::MarkItemEdited(id);

	// Render grab
	if (grab_bb.Max.x > grab_bb.Min.x) {//133, 193, 228
		window->DrawList->AddRectFilled(ImVec2(frame_bb.Min.x, frame_bb.Min.y + 9), ImVec2(grab_bb.Max.x - 2, grab_bb.Max.y - 9), ImColor(133 / 255.f, 193 / 255.f, 228 / 255.f, 0.6f * ImGui::GetStyle().Alpha), 4);
		window->DrawList->AddRectFilled(ImVec2(grab_bb.Min.x - 2, grab_bb.Min.y + 6), ImVec2(grab_bb.Max.x, grab_bb.Max.y - 8), ImColor(92 / 255.f, 195 / 255.f, 255 / 255.f, 1 * ImGui::GetStyle().Alpha), 3);
	}
	// Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
	char value_buf[64];
	const char* value_buf_end = value_buf + ImGui::DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

	window->DrawList->AddText(ImVec2(frame_bb.Max.x - 2 - ImGui::CalcTextSize(value_buf).x, window->DC.CursorPos.y - 26), ImColor(255, 255, 255), value_buf, value_buf_end);

	if (label_size.x > 0.0f)
		ImGui::RenderText(ImVec2(window->DC.CursorPos.x + 8, window->DC.CursorPos.y - 28), label);
	ImGui::Spacing();
	IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
	return value_changed;
}

bool menu::elements::functions::slider_float(const char* label, float* v, float v_min, float v_max, const char* format, float power)
{
	return SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, power);
}

bool menu::elements::functions::slider_int(const char* label, int* v, int v_min, int v_max, const char* format)
{
	return SliderScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, 1);
}

bool SliderScalarStretched(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, float power, ImGuiSliderFlags flags)
{
	//ImGuiWindow* window = ImGui::GetCurrentWindow();
	//if (window->SkipItems)
	//	return false;

	//ImGuiContext& g = *GImGui;
	//const ImGuiStyle& style = g.Style;
	//const ImGuiID id = window->GetID(label);
	//const float w = ImGui::CalcItemWidth();
	//const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	//const ImRect frame_bb(window->DC.CursorPos + ImVec2(8, 8), window->DC.CursorPos + ImVec2(window->Size.x - 8, 16 + label_size.y + style.FramePadding.y * 2.0f));
	//const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

	//ImGui::ItemSize(total_bb, style.FramePadding.y);
	//if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
	//	return false;

	// Default format string when passing NULL
	//if (format == NULL)
	//	format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;
	//else if (data_type == ImGuiDataType_S32 && strcmp(format, "%d") != 0) // (FIXME-LEGACY: Patch old "%.0f" format string to use "%d", read function more details.)
	//	format = PatchFormatStringFloatToInt(format);

	// Tabbing or CTRL-clicking on Slider turns it into an input box
	//const bool hovered = ImGui::ItemHoverable(frame_bb, id);
	//const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
	//bool temp_input_is_active = temp_input_allowed && ImGui::TempInputIsActive(id);
	//if (!temp_input_is_active)
	//{
	//	const bool focus_requested = temp_input_allowed && ImGui::FocusableItemRegister(window, id);
	//	const bool clicked = (hovered && g.IO.MouseClicked[0]);
	//	if (focus_requested || clicked || g.NavActivateId == id || g.NavInputId == id)
	//	{
	//		ImGui::SetActiveID(id, window);
	//		ImGui::SetFocusID(id, window);
	//		ImGui::FocusWindow(window);
	//		g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
	//		if (temp_input_allowed && (focus_requested || (clicked && g.IO.KeyCtrl) || g.NavInputId == id))
	//		{
	//			temp_input_is_active = true;
	//			ImGui::FocusableItemUnregister(window);
	//		}
	//	}
	//}

	//if (temp_input_is_active)
	//{
	//	 Only clamp CTRL+Click input when ImGuiSliderFlags_AlwaysClamp is set
	//	const bool is_clamp_input = (flags & ImGuiSliderFlags_AlwaysClamp) != 0;
	//	return ImGui::TempInputScalar(frame_bb, id, label, data_type, p_data, format, is_clamp_input ? p_min : NULL, is_clamp_input ? p_max : NULL);
	//}
	//float deltatime = 1.5f * ImGui::GetIO().DeltaTime;
	//static std::map<ImGuiID, float> hover_animation;
	//auto it_hover = hover_animation.find(id);
	//if (it_hover == hover_animation.end())
	//{
	//	hover_animation.insert({ id, 0.f });
	//	it_hover = hover_animation.find(id);
	//}
	//it_hover->second = ImClamp(it_hover->second + (2.15f * ImGui::GetIO().DeltaTime * (hovered || ImGui::IsItemActive() ? 1.f : -1.f)), 0.0f, 1.f);
	//it_hover->second *= ImGui::GetStyle().Alpha;

	//static std::map<ImGuiID, float> filled_animation;
	//auto it_filled = filled_animation.find(id);
	//if (it_filled == filled_animation.end())
	//{
	//	filled_animation.insert({ id, 0.f });
	//	it_filled = filled_animation.find(id);
	//}
	//it_filled->second = ImClamp(it_filled->second + (1.15f * ImGui::GetIO().DeltaTime), 0.0f, 1.f);
	//it_filled->second *= min(GetStyle().Alpha * 1.5, 1.f);
	// Draw frame
	//const ImU32 frame_col = ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : g.HoveredId == id ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
	//ImGui::RenderNavHighlight(frame_bb, id);
	//window->DrawList->AddRectFilled(ImVec2(frame_bb.Min.x, frame_bb.Min.y + 9), ImVec2(frame_bb.Max.x - 2, frame_bb.Max.y - 3 - 8), ImColor(18, 25, 35), 4);

	// Slider behavior
	//ImRect grab_bb;
	//const bool value_changed = ImGui::SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
	//if (value_changed)
	//	ImGui::MarkItemEdited(id);

	// Render grab
	//if (grab_bb.Max.x > grab_bb.Min.x) {
	//	window->DrawList->AddRectFilled(ImVec2(frame_bb.Min.x, frame_bb.Min.y + 9), ImVec2(grab_bb.Max.x - 2, grab_bb.Max.y - 9), ImColor(45 / 255.f, 143 / 255.f, 206 / 255.f, 0.9f * GetStyle().Alpha), 4);
	//	window->DrawList->AddRectFilled(ImVec2(grab_bb.Min.x - 2, grab_bb.Min.y + 6), ImVec2(grab_bb.Max.x, grab_bb.Max.y - 8), ImColor(45 / 255.f, 143 / 255.f, 206 / 255.f, 1 * GetStyle().Alpha), 3);
	//}
	// Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
	//char value_buf[64];
	//const char* value_buf_end = value_buf + ImGui::DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

	//window->DrawList->AddText(ImVec2(frame_bb.Max.x - 2 - ImGui::CalcTextSize(value_buf).x, window->DC.CursorPos.y - 26), ImGui::GetColorU32(ImGuiCol_Text), value_buf, value_buf_end);

	//if (label_size.x > 0.0f)
	//	ImGui::RenderText(ImVec2(window->DC.CursorPos.x + 8, window->DC.CursorPos.y - 28), label);
	//ImGui::Spacing();
	//IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
	//return value_changed;
}

bool menu::elements::functions::slider_float_stretched(const char* label, float* v, float v_min, float v_max, const char* format, float power)
{
	return SliderScalarStretched(label, ImGuiDataType_Float, v, &v_min, &v_max, format, power, 0);
}

bool menu::elements::functions::slider_int_stretched(const char* label, int* v, int v_min, int v_max, const char* format)
{
	return SliderScalarStretched(label, ImGuiDataType_S32, v, &v_min, &v_max, format, 1, 0);
}

static float CalcMaxPopupHeightFromItemCount(int items_count)
{
	ImGuiContext& g = *GImGui;
	if (items_count <= 0)
		return FLT_MAX;
	return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

struct combo_anim
{
	int active_outline_anim;
	float active_circle_anim;
	float open_anim;
};

bool menu::elements::functions::begin_combo(const char* label, const char* preview_value, ImGuiComboFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
	g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
	if (window->SkipItems)
		return false;

	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together
	const float size = ImGui::GetWindowWidth() - 20;

	const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(size, 44));
	const ImRect clickable(window->DC.CursorPos + ImVec2(0, 18), window->DC.CursorPos + ImVec2(size, 44));
	ImGui::ItemSize(rect, style.FramePadding.y);
	if (!ImGui::ItemAdd(clickable, id, &rect))
		return false;

	// Open on click
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(clickable, id, &hovered, &held);
	const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
	bool popup_open = ImGui::IsPopupOpen(popup_id, ImGuiPopupFlags_None);
	if (pressed && !popup_open)
	{
		ImGui::OpenPopupEx(popup_id, ImGuiPopupFlags_None);
		popup_open = true;
	}

	static std::map <ImGuiID, combo_anim> anim;
	auto it_anim = anim.find(id);
	if (it_anim == anim.end())
	{
		anim.insert({ id, {0, 0.f, 0.f} });
		it_anim = anim.find(id);
	}

	if (popup_open)
		it_anim->second.active_outline_anim += 4 * (1.f - ImGui::GetIO().DeltaTime);
	else
		it_anim->second.active_outline_anim -= 4 * (1.f - ImGui::GetIO().DeltaTime);

	if (it_anim->second.active_outline_anim > 20)
		it_anim->second.active_outline_anim = 20;
	else if (it_anim->second.active_outline_anim < 0)
		it_anim->second.active_outline_anim = 0;

	if (popup_open)
		it_anim->second.active_circle_anim += 0.2f * (1.f - ImGui::GetIO().DeltaTime);
	else
		it_anim->second.active_circle_anim -= 0.2f * (1.f - ImGui::GetIO().DeltaTime);

	if (it_anim->second.active_circle_anim > 0.8f)
		it_anim->second.active_circle_anim = 0.8f;
	else if (it_anim->second.active_circle_anim < 0)
		it_anim->second.active_circle_anim = 0;

	if (popup_open)
		it_anim->second.open_anim += 0.05f * (1.f - ImGui::GetIO().DeltaTime);
	else
		it_anim->second.open_anim -= 0.05f * (1.f - ImGui::GetIO().DeltaTime);

	if (it_anim->second.open_anim > 1)
		it_anim->second.open_anim = 1;
	else if (it_anim->second.open_anim < 0)
		it_anim->second.open_anim = 0;


	window->DrawList->AddRectFilled(clickable.Min, clickable.Max - 0.5f, ImColor(14, 20, 33), 3.f);

	window->DrawList->AddRect(clickable.Min, clickable.Max - 0.5f, ImColor(255, 255, 255, (15 + it_anim->second.active_outline_anim)), 3.f);

	window->DrawList->AddText({ rect.Min.x + 5, rect.Min.y + 2 }, ImColor(255, 255, 255, 255), label);

	window->DrawList->PushClipRect(clickable.Min, clickable.Max - ImVec2(29, 0));
	window->DrawList->AddText(clickable.Min + ImVec2(5, 5), ImColor(255, 255, 255, 255), preview_value);
	window->DrawList->PopClipRect();

	window->DrawList->AddCircleFilled(rect.Max - ImVec2(11, 13), 3.f + it_anim->second.active_circle_anim, ImColor(92 / 255.f, 195 / 255.f, 255 / 255.f, 1 * ImGui::GetStyle().Alpha));

	if (!popup_open)
		return false;

	g.NextWindowData.Flags = backup_next_window_data_flags;
	if (!ImGui::IsPopupOpen(popup_id, ImGuiPopupFlags_None))
	{
		g.NextWindowData.ClearFlags();
		return false;
	}

	// Set popup size
	float w = clickable.GetWidth();
	if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint)
	{
		g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
	}
	else
	{
		if ((flags & ImGuiComboFlags_HeightMask_) == 0)
			flags |= ImGuiComboFlags_HeightRegular;
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_)); // Only one
		int popup_max_height_in_items = -1;
		if (flags & ImGuiComboFlags_HeightRegular)     popup_max_height_in_items = 8;
		else if (flags & ImGuiComboFlags_HeightSmall)  popup_max_height_in_items = 4;
		else if (flags & ImGuiComboFlags_HeightLarge)  popup_max_height_in_items = 20;
		ImGui::SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items) * it_anim->second.open_anim));
	}

	// This is essentially a specialized version of BeginPopupEx()
	char name[16];
	ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

	// Set position given a custom constraint (peak into expected window size so we can position it)
	// FIXME: This might be easier to express with an hypothetical SetNextWindowPosConstraints() function?
	// FIXME: This might be moved to Begin() or at least around the same spot where Tooltips and other Popups are calling FindBestWindowPosForPopupEx()?
	if (ImGuiWindow* popup_window = ImGui::FindWindowByName(name))
		if (popup_window->WasActive)
		{
			// Always override 'AutoPosLastDirection' to not leave a chance for a past value to affect us.
			ImVec2 size_expected = ImGui::CalcWindowNextAutoFitSize(popup_window);
			popup_window->AutoPosLastDirection = (flags & ImGuiComboFlags_PopupAlignLeft) ? ImGuiDir_Left : ImGuiDir_Down; // Left = "Below, Toward Left", Down = "Below, Toward Right (default)"
			ImRect r_outer = ImGui::GetPopupAllowedExtentRect(popup_window);
			ImVec2 pos = ImGui::FindBestWindowPosForPopupEx(clickable.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, clickable, ImGuiPopupPositionPolicy_ComboBox);
			ImGui::SetNextWindowPos(pos + ImVec2(0, 2));
		}

	// We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5)); // Horizontally align ourselves with the framed text
	ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.f);
	ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.f);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(ImColor(12, 18, 31, 255)));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(ImColor(255, 255, 255, (15 + it_anim->second.active_outline_anim))));
	bool ret = ImGui::Begin(name, NULL, window_flags);
	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(2);
	if (!ret)
	{
		ImGui::EndPopup();
		IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
		return false;
	}
	return true;
}

void menu::elements::functions::end_combo()
{
	ImGui::EndPopup();
}

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}

static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
{
	// FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
	const char* items_separated_by_zeros = (const char*)data;
	int items_count = 0;
	const char* p = items_separated_by_zeros;
	while (*p)
	{
		if (idx == items_count)
			break;
		p += strlen(p) + 1;
		items_count++;
	}
	if (!*p)
		return false;
	if (out_text)
		*out_text = p;
	return true;
}

bool Button(const char* label, ImVec2 size_arg, bool active)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	int flags = 0;
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

	if (hovered)
		ImGui::SetMouseCursor(7);

	float Hovered = ImTricks::Animations::FastFloatLerp(id, active, 0.f, 1.f, 0.09f);

	window->DrawList->AddText(bb.Min + ImVec2(10, size_arg.y / 2 - ImGui::CalcTextSize(label).y / 2), ImColor(230, 230, 230, 190 + int(Hovered * 60.f)), label);

	return pressed;
}

bool menu::elements::functions::combo(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items)
{
	ImGuiContext& g = *GImGui;

	// Call the getter to obtain the preview string which is a parameter to BeginCombo()
	const char* preview_value = NULL;
	if (*current_item >= 0 && *current_item < items_count)
		items_getter(data, *current_item, &preview_value);

	// The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
	if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
		ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

	if (!begin_combo(label, preview_value, ImGuiComboFlags_None))
		return false;

	// Display items
	// FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
	bool value_changed = false;
	for (int i = 0; i < items_count; i++)
	{
		ImGui::PushID(i);
		const bool item_selected = (i == *current_item);
		const char* item_text;
		if (!items_getter(data, i, &item_text))
			item_text = "*Unknown item*";
		if (ImGui::Selectable(item_text, item_selected))
		{
			value_changed = true;
			*current_item = i;
		}
		if (item_selected)
			ImGui::SetItemDefaultFocus();
		ImGui::PopID();
	}

	end_combo();

	if (value_changed)
		ImGui::MarkItemEdited(g.LastItemData.ID);

	return value_changed;
}

// Combo box helper allowing to pass an array of strings.
bool menu::elements::functions::combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
{
	const bool value_changed = combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
	return value_changed;
}

// Combo box helper allowing to pass all items in a single string literal holding multiple zero-terminated items "item1\0item2\0"
bool menu::elements::functions::combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
{
	int items_count = 0;
	const char* p = items_separated_by_zeros;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
	while (*p)
	{
		p += strlen(p) + 1;
		items_count++;
	}
	bool value_changed = combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
	return value_changed;
}

struct button_anim
{
	bool clicked;
	int active_outline_anim;
	int active_rect_alpha;
};

bool menu::elements::functions::button(const char* label, ImVec2 size_arg, ImGuiButtonFlags flags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect rect(pos, pos + size);
	ImGui::ItemSize(size, style.FramePadding.y);
	if (!ImGui::ItemAdd(rect, id))
		return false;

	if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, flags);

	static std::map <ImGuiID, button_anim> anim;
	auto it_anim = anim.find(id);
	if (it_anim == anim.end())
	{
		anim.insert({ id, {false, 0, 0} });
		it_anim = anim.find(id);
	}

	if (it_anim->second.clicked)
		it_anim->second.active_outline_anim += 4 * (1.f - ImGui::GetIO().DeltaTime);
	else
		it_anim->second.active_outline_anim -= 4 * (1.f - ImGui::GetIO().DeltaTime);

	if (it_anim->second.active_outline_anim > 20)
		it_anim->second.active_outline_anim = 20;
	else if (it_anim->second.active_outline_anim < 0)
		it_anim->second.active_outline_anim = 0;

	if (it_anim->second.clicked)
		it_anim->second.active_rect_alpha += 25 * (1.f - ImGui::GetIO().DeltaTime);
	else
		it_anim->second.active_rect_alpha -= 25 * (1.f - ImGui::GetIO().DeltaTime);

	if (it_anim->second.active_rect_alpha > 255)
		it_anim->second.active_rect_alpha = 255;
	else if (it_anim->second.active_rect_alpha < 0)
		it_anim->second.active_rect_alpha = 0;

	if (pressed)
		it_anim->second.clicked = true;

	if (it_anim->second.active_outline_anim >= 20 && it_anim->second.active_rect_alpha >= 255)
		it_anim->second.clicked = false;

	window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(14, 20, 33, 255), 3.f);
	window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(118, 209, 227, it_anim->second.active_rect_alpha), 3.f);
	window->DrawList->AddRect(rect.Min, rect.Max, ImColor(255, 255, 255, (15 + it_anim->second.active_outline_anim)), 3.f); 
	ImGui::RenderTextClipped(rect.Min, rect.Max, label, NULL, &label_size, ImVec2(0.5f, 0.5f));

	IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
	return pressed;
}