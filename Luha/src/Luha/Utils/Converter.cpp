#include "lhpch.h"

#include "Luha/Utils/Converter.h"

namespace Utils {

	ImVec2 GlmVec2_To_ImVec2(const glm::vec2& vec)
	{
		return { vec.x, vec.y };
	}

	ImVec4 GlmVec4_To_ImVec4(const glm::vec4& vec)
	{
		return { vec.x, vec.y, vec.z, vec.w };
	}

	glm::vec2 ImVec2_To_GlmVec2(const ImVec2& vec)
	{
		return { vec.x, vec.y };
	}

	glm::vec4 ImVec4_To_GlmVec4(const ImVec4& vec)
	{
		return { vec.x, vec.y, vec.z, vec.w };
	}
	
}