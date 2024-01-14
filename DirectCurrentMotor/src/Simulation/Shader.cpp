#include "Shader.h"

#include <Glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

namespace DCM {

	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE NOT SUCCESFULLY READ" << std::endl;
		}

		const char* vertexShader_CStr = vertexCode.c_str();
		const char* fragmentShader_CStr = fragmentCode.c_str();

		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShader_CStr, NULL);
		glCompileShader(vertexShader);
		{ // Check shader compile
			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION FAILED\n" << infoLog << std::endl;
			}
		}
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShader_CStr, NULL);
		glCompileShader(fragmentShader);
		{ // Check shader compile
			int success;
			char infoLog[512];
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION FAILED\n" << infoLog << std::endl;
			}
		}
		// Create shader program
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		glLinkProgram(m_RendererID);
		{ // Check linking
			int success;
			char infoLog[512];
			glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::LINKING FAILED\n" << infoLog << std::endl;
			}
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader::~Shader()
	{
	}
	void Shader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& v)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, v.x, v.y, v.z, v.w);
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& v)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(v));
	}

	void Shader::SetInt(const std::string& name, int v)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, v);
	}

}