#pragma once

#include <mutex>
#include <vector>

#include "imgui.h"

namespace ImGui {
	extern void outline_text(
		ImDrawList* dl,
		const ImFont* font,
		float            size,
		const ImVec2& pos,
		ImColor            fill,
		ImColor            outline,
		std::string_view txt
	) noexcept;

	inline __forceinline void outline_text(ImDrawList* dl, const ImVec2& pos, ImColor fill, ImColor outline, std::string_view txt) noexcept
	{
		outline_text(dl, nullptr, 0.f, pos, fill, outline, txt);
	}
}// namespace ImGui

class ImGuiDrawCmds {
public:
	ImGuiDrawCmds() noexcept = default;

	// Must call drawing funcs between these two calls.
	void begin() noexcept;
	void end() noexcept;

	// Clear all commands.
	void clear() noexcept;

	// Drawing functions.
	void line(const ImVec2& start, const ImVec2& end, ImColor color, float thickness = 1.f) noexcept;
	void rect_filled(const ImVec2& pos, const ImVec2& pos2, ImColor color, float rounding = 0.f) noexcept;
	void rect(const ImVec2& pos, const ImVec2& pos2, ImColor color, float rounding = 0.f, float thickness = 1.f) noexcept;
	void text(const ImVec2& pos, ImColor color, std::string_view txt, bool outline = false, bool center = false) noexcept;
	void circle(const ImVec2& center, float radius, ImU32 col, int num_segments = 0, float thickness = 1.f) noexcept;

	// Add commands to ImDrawList.
	void draw(ImDrawList* dl) noexcept;

private:
	enum class CmdType : uint8_t {
		Line = 0,
		RectFilled,
		Rect,
		Text,
		Circle
	};

	struct DrawCmd {
		CmdType m_type;

		struct Line {
			ImVec2 m_start;
			ImVec2 m_end;
			ImColor m_color;
			float m_thickness;
		} m_line;

		struct RectFilled {
			ImVec2 m_min;
			ImVec2 m_max;
			ImColor m_color;
			float m_rounding{};
		} m_rect_filled;

		struct Rect {
			ImVec2 m_min;
			ImVec2 m_max;
			ImColor m_color;
			float m_rounding;
			float m_thickness;
		} m_rect;

		struct Text {
			ImFont* m_font{};
			float m_size{};
			ImVec2 m_pos;
			ImColor m_color;
			std::string m_txt;
			bool m_outline;
			bool m_center;
		} m_text;

		struct Circle {
			ImVec2 m_center;
			float m_radius;
			ImColor m_color;
			int num_segments;
			float thickness;
		} m_circle;
	};

	std::mutex m_mutex;
	std::vector<DrawCmd> m_cmds;
};