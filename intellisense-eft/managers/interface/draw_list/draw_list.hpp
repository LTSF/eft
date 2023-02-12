#pragma once

#include <dependencies/common_includes.hpp>

struct base_command_t
{
	virtual void render() = 0;
};

struct text_t : public base_command_t
{
	text_t(ImVec2 pos, ImColor col, const std::string& text, bool center = false)
		: pos{ pos }, col{ col }, text{ text }, center{ center } { }

	std::string text;
	ImColor col;
	ImVec2 pos;
	bool center;
	void render() override
	{
		if (center)
			pos.x -= ImGui::CalcTextSize(text.c_str()).x;
		ImGui::GetBackgroundDrawList()->AddText(pos, col, text.c_str());
	}
};

struct rect_t : public base_command_t
{
	rect_t(ImVec2 point1, ImVec2 point2, ImColor col)
		: point1{ point1 }, point2{ point2 }, col{ col } { }

	ImVec2 point1, point2;
	ImColor col;
	void render() override
	{
		ImGui::GetBackgroundDrawList()->AddRect(point1, point2, col);
	}
};

struct line_t : public base_command_t
{
	line_t(ImVec2 point1, ImVec2 point2, ImColor col)
		: point1{ point1 }, point2{ point2 }, col{ col } { }

	ImVec2 point1, point2;
	ImColor col;
	void render() override
	{
		ImGui::GetBackgroundDrawList()->AddLine(point1, point2, col);
	}
};

struct circle_t : public base_command_t
{
	circle_t(ImVec2 center, float radius, ImColor col)
		: center{ center }, radius{ radius }, col{ col } { }

	ImVec2 center;
	float radius;
	ImColor col;

	void render() override
	{
		ImGui::GetBackgroundDrawList()->AddCircle(center, radius, col);
	}
};

namespace managers::draw_list {
	enum e_render_status
	{
		not_initialized,
		rendered,
		use_cache,
		rendering,
		idle,
		no_render
	};

	class draw_list_t {
	public:
		e_render_status status = e_render_status::not_initialized;
		std::vector<std::shared_ptr<base_command_t>> cmd;
		std::vector<std::shared_ptr<base_command_t>> cache;

		template< typename t, typename ... args_t >
		void add( const args_t&... args )
		{
			cmd.emplace_back(new t{ args... });
		}

		void kill_frame() { status = e_render_status::no_render; };
		void begin() { status = e_render_status::use_cache; if (cmd.size()) { cmd.clear(); } };
		void end_frame() { 
			status = e_render_status::idle;

			cache.clear();

			for ( const auto command : cmd)
				cache.push_back(command);
		};
		bool not_safe() { return (status != e_render_status::not_initialized && status != e_render_status::no_render && status != e_render_status::rendered); };
		void render();
	};
}