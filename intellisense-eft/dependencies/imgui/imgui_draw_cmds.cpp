#include "imgui_draw_cmds.hpp"
#include <dependencies/imgui/imgui_internal.h>

void ImGui::outline_text(ImDrawList* dl, const ImFont* font, float size, const ImVec2& pos, ImColor fill, ImColor outline, std::string_view txt) noexcept {
	dl->AddText(font, size, { pos.x - 1.f, pos.y }, outline, txt.data());
	dl->AddText(font, size, { pos.x - 1.f, pos.y - 1.f }, outline, txt.data());
	dl->AddText(font, size, { pos.x, pos.y - 1.f }, outline, txt.data());
	dl->AddText(font, size, { pos.x + 1.f, pos.y - 1.f }, outline, txt.data());
	dl->AddText(font, size, { pos.x + 1.f, pos.y }, outline, txt.data());
	dl->AddText(font, size, { pos.x + 1.f, pos.y + 1.f }, outline, txt.data());
	dl->AddText(font, size, { pos.x, pos.y + 1.f }, outline, txt.data());
	dl->AddText(font, size, { pos.x - 1.f, pos.y + 1.f }, outline, txt.data());
	dl->AddText(font, size, pos, fill, txt.data());
}

void ImGuiDrawCmds::line(const ImVec2& start, const ImVec2& end, ImColor color, float thickness) noexcept {
	auto& cmd = m_cmds.emplace_back();
	cmd.m_type = CmdType::Line;
	cmd.m_line.m_start = { IM_FLOOR(start.x), IM_FLOOR(start.y) };
	cmd.m_line.m_end = { IM_FLOOR(end.x), IM_FLOOR(end.y) };
	cmd.m_line.m_color = color;
	cmd.m_line.m_thickness = thickness;
}

void ImGuiDrawCmds::rect_filled(const ImVec2& pos, const ImVec2& pos2, ImColor color, float rounding) noexcept {
	auto& cmd = m_cmds.emplace_back();
	cmd.m_type = CmdType::RectFilled;
	cmd.m_rect_filled.m_min = { IM_FLOOR(pos.x), IM_FLOOR(pos.y) };
	cmd.m_rect_filled.m_max = { IM_FLOOR(pos2.x), IM_FLOOR(pos2.y) };
	cmd.m_rect_filled.m_color = color;
	cmd.m_rect_filled.m_rounding = rounding;
}

void ImGuiDrawCmds::rect(const ImVec2& pos, const ImVec2& pos2, ImColor color, float rounding, float thickness) noexcept {
	auto& cmd = m_cmds.emplace_back();
	cmd.m_type = CmdType::Rect;
	cmd.m_rect.m_min = { IM_FLOOR(pos.x), IM_FLOOR(pos.y) };
	cmd.m_rect.m_max = { IM_FLOOR(pos2.x), IM_FLOOR(pos2.y) };
	cmd.m_rect.m_color = color;
	cmd.m_rect.m_rounding = rounding;
	cmd.m_rect.m_thickness = thickness;
}

void ImGuiDrawCmds::text(const ImVec2& pos, ImColor color, std::string_view txt, bool outline, bool center) noexcept {
	auto& cmd = m_cmds.emplace_back();
	cmd.m_type = CmdType::Text;
	cmd.m_text.m_pos = { pos.x, pos.y };
	cmd.m_text.m_color = color;
	cmd.m_text.m_txt = txt;
	cmd.m_text.m_outline = outline;
	cmd.m_text.m_center = center;
}

void ImGuiDrawCmds::circle(const ImVec2& center, float radius, ImU32 col, int num_segments, float thickness) noexcept
{
	auto& cmd = m_cmds.emplace_back();
	cmd.m_type = CmdType::Circle;
	cmd.m_circle.m_center = center;
	cmd.m_circle.m_color = col;
	cmd.m_circle.m_radius = radius;
	cmd.m_circle.num_segments = num_segments;
	cmd.m_circle.thickness = thickness;
}

void ImGuiDrawCmds::begin() noexcept {
	m_mutex.lock();
}

void ImGuiDrawCmds::end() noexcept {
	m_mutex.unlock();
}

void ImGuiDrawCmds::clear() noexcept {
	m_cmds.clear();
}

void ImGuiDrawCmds::draw(ImDrawList* dl) noexcept {
	for (const auto& c : m_cmds) {
		switch (c.m_type) {
		case CmdType::Line: {
			const auto& data = c.m_line;
			dl->AddLine(data.m_start, data.m_end, data.m_color, data.m_thickness);
			break;
		}

		case CmdType::RectFilled: {
			const auto& data = c.m_rect_filled;
			dl->AddRectFilled(data.m_min, data.m_max, data.m_color, data.m_rounding);
			break;
		}

		case CmdType::Rect: {
			const auto& data = c.m_rect;
			dl->AddRect(data.m_min, data.m_max, data.m_color, data.m_rounding, 0, data.m_thickness);
			break;
		}

		case CmdType::Circle: {
			const auto& data = c.m_circle;
			dl->AddCircle(data.m_center, data.m_radius, data.m_color, data.num_segments, data.thickness);
			break;
		}

		case CmdType::Text: {
			const auto& data = c.m_text;
			auto pos = data.m_pos;
			if (data.m_center) {
				const auto size = ImGui::CalcTextSize(data.m_txt.c_str());
				pos.x -= (size.x / 2.f);
			}

			if (!data.m_outline) {
				dl->AddText(data.m_font, data.m_size, pos, data.m_color, data.m_txt.c_str());
			}
			else {
				ImGui::outline_text(dl, data.m_font, data.m_size, pos, data.m_color, { 0, 0, 0, 192 }, data.m_txt);
			}

			break;
		}
		}
	}
}