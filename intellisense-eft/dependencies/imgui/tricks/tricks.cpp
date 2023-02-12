#include "tricks.hpp"

namespace ImTricks {

	namespace Animations {

		float GetFrameSpeed(float speed, float min, float max)
		{
			const float deltav = max - min;
			const float frameRateSpeed = (ImGui::GetIO().DeltaTime) * deltav / speed;
			return frameRateSpeed;
		}

		int FastIntLerp(ImGuiID identifier, bool state, int min, int max, int speed) {

			static std::unordered_map<ImGuiID, int>* valuesMapInt = nullptr;
			if (!valuesMapInt)
			{
				valuesMapInt = new std::unordered_map<ImGuiID, int>();
			}

			auto value = valuesMapInt->find(identifier);

			if (value == valuesMapInt->end()) {
				valuesMapInt->insert({ identifier,  min });
				value = valuesMapInt->find(identifier);
			}

			const float frameRateSpeed = GetFrameSpeed(speed, min, max);

			if (state) {
				if (value->second < max)
					value->second += frameRateSpeed;
			}
			else {
				if (value->second > min)
					value->second -= frameRateSpeed;
			}

			value->second = std::clamp(value->second, min, max);

			return value->second;
		}

		float FastFloatLerp(ImGuiID identifier, bool state, float min, float max, float speed) {

			static std::unordered_map<ImGuiID, float>* valuesMapFloat = nullptr;
			if (!valuesMapFloat)
			{
				valuesMapFloat = new std::unordered_map<ImGuiID, float>();
			}

			auto value = valuesMapFloat->find(identifier);

			if (value == valuesMapFloat->end()) {
				valuesMapFloat->insert({ identifier, min });
				value = valuesMapFloat->find(identifier);
			}

			const float frameRateSpeed = GetFrameSpeed(speed, min, max);

			if (state) {
				if (value->second < max)
					value->second += frameRateSpeed;
			}
			else {
				if (value->second > min)
					value->second -= frameRateSpeed;
			}

			value->second = std::clamp(value->second, min, max);

			return value->second;
		}

		ImVec2 FastImVec2Lerp(ImGuiID identifier, bool state, ImVec2 min, ImVec2 max, float speed) {

			static std::unordered_map<ImGuiID, ImVec2>* valuesMapFloat = nullptr;
			if (!valuesMapFloat)
			{
				valuesMapFloat = new std::unordered_map<ImGuiID, ImVec2>();
			}


			auto value = valuesMapFloat->find(identifier);

			if (value == valuesMapFloat->end()) {
				valuesMapFloat->insert({ identifier, min });
				value = valuesMapFloat->find(identifier);
			}

			const float frameRateSpeedx = GetFrameSpeed(speed, min.x, max.x);
			const float frameRateSpeedy = GetFrameSpeed(speed, min.y, max.y);
			const float frameRateSpeed = (frameRateSpeedx + frameRateSpeedy) * .5f;

			if (state) {
				if (value->second.x < max.x)
					value->second.x += frameRateSpeed;

				if (value->second.y < max.y)
					value->second.y += frameRateSpeed;
			}
			else {
				if (value->second.x > min.x)
					value->second.x -= frameRateSpeed;

				if (value->second.y > min.y)
					value->second.y -= frameRateSpeed;
			}

			value->second.x = std::clamp(value->second.x, min.x, max.x);
			value->second.y = std::clamp(value->second.y, min.y, max.y);

			return value->second;
		}

		ImColor FastColorLerp(ImColor start, ImColor end, float stage)
		{
			ImVec4 lerp = ImLerp(
				ImVec4(start.Value.x, start.Value.y, start.Value.z, start.Value.w),
				ImVec4(end.Value.x, end.Value.y, end.Value.z, end.Value.w),
				stage);

			return ImGui::ColorConvertFloat4ToU32(lerp);
		}

		ImTrickyColor FastColorLerp(ImTrickyColor start, ImTrickyColor end, float stage)
		{
			ImVec4 lerp = ImLerp(
				ImVec4(start[0], start[1], start[2], start[3]),
				ImVec4(end[0], end[1], end[2], end[3]),
				stage);

			return ImTrickyColor(lerp.x, lerp.y, lerp.z, lerp.w);
		}
	}

	namespace Widgets {
		void ColorEdit3(const char* label, ImColor& color, ImGuiColorEditFlags flags) {
			float col[3] = { color.Value.x, color.Value.y, color.Value.z };

			if (ImGui::ColorEdit3(label, col, flags))
				color = ImColor(col[0], col[1], col[2]);
		}

		void ColorEdit4(const char* label, ImColor& color, ImGuiColorEditFlags flags) {
			float col[4] = { color.Value.x, color.Value.y, color.Value.z , color.Value.w };

			if (ImGui::ColorEdit4(label, col, flags))
				color = ImColor(col[0], col[1], col[2], col[3]);
		}

		void ColorEdit3(const char* label, ImTrickyColor& color, ImGuiColorEditFlags flags) {
			float col[3] = { color[0], color[1], color[2] };

			if (ImGui::ColorEdit3(label, col, flags))
				color = ImTrickyColor(col[0], col[1], col[2]);
		}

		void ColorEdit4(const char* label, ImTrickyColor& color, ImGuiColorEditFlags flags) {
			float col[4] = { color[0], color[1], color[2] , color[3] };

			if (ImGui::ColorEdit4(label, col, flags))
				color = ImTrickyColor(col[0], col[1], col[2], col[3]);
		}
	}
}