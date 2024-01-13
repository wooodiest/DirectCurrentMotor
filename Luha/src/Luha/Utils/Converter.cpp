#include "lhpch.h"

#include "Luha/Utils/Converter.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

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

	float DegToRad(float deg)
	{
		return deg * M_PI / 180.0f;
	}

	float RadToDeg(float rad)
	{
		return rad * 180.0f / M_PI;
	}

    void ConvertObjToArray(const std::string& filepath, float*& v, unsigned int*& i, size_t& vertexCount, size_t& indexCount)
    {
        std::ifstream objFile(filepath);
        std::string line;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        while (std::getline(objFile, line))
        {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "v")
            {
                float x, y, z;
                iss >> x >> y >> z;
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
            else if (type == "f")
            {
               
            }
        }

        v = new float[vertices.size()];
        i = new unsigned int[indices.size()];
        vertexCount = vertices.size();
        indexCount = indices.size();

        for (int j = 0; j < vertexCount; j++)
            v[j] = vertices[j];

        for (int j = 0; j < indexCount; j++)
            i[j] = indices[j];
    }
	
}