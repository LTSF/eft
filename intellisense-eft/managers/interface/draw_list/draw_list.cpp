#include "draw_list.hpp"

void managers::draw_list::draw_list_t::render() {
	const auto command_list = [&]() -> std::optional<std::vector<std::shared_ptr<base_command_t>>> 
	{
		switch (status)
		{
		case e_render_status::idle:
			return cmd;
		case e_render_status::use_cache:
		case e_render_status::rendered:
			return cache;
		default:
			return std::nullopt;
		}
	}();

	if (command_list)
	{
		status = e_render_status::rendering;

		for (const auto command : *command_list)
		{
			if (command)
				command->render();
		}

		status = e_render_status::rendered;
	}
}