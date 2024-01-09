#pragma once

#include "imgui.h"
#include "glm/glm.hpp"

namespace Utils
{
	ImVec2 GlmVec2_To_ImVec2(const glm::vec2& vec);
	ImVec4 GlmVec4_To_ImVec4(const glm::vec4& vec);

	glm::vec2 ImVec2_To_GlmVec2(const ImVec2& vec);
	glm::vec4 ImVec4_To_GlmVec4(const ImVec4& vec);
	
}