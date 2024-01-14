#pragma once

#include <string>
#include <glm/glm.hpp>

namespace DCM {

	class Shader
	{
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void Bind();

		void SetFloat4(const std::string& name, const glm::vec4& v);
		void SetMat4(const std::string& name, const glm::mat4& v);
		void SetInt(const std::string& name, int v);

	private:
		int m_RendererID;
	};

}