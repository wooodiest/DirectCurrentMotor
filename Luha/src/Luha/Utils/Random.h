#pragma once

#include <random>
#include <glm/glm.hpp>

namespace Luha {

	class Random
	{
	public:

		static uint32_t UInt()
		{
			return s_Distribution(s_RandomEngine);
		}

		static uint32_t UInt(uint32_t min, uint32_t max)
		{
			return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
		}

		static int32_t Int()
		{
			return static_cast<int>(UInt());
		}

		static int32_t Int(int32_t min, int32_t max)
		{
			uint32_t uintValue = UInt(static_cast<uint32_t>(min), static_cast<uint32_t>(max));
			return static_cast<int32_t>(uintValue);
		}

		static float Float()
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

		static float Float(float min, float max)
		{
			return min + (max - min) * Float();
		}

		static glm::vec2 Vec2()
		{
			return glm::vec2(Float(), Float());
		}

		static glm::vec2 Vec2(float min, float max)
		{
			return glm::vec2(Float(min, max), Float(min, max));
		}

		static glm::vec3 Vec3()
		{
			return glm::vec3(Float(), Float(), Float());
		}

		static glm::vec3 Vec3(float min, float max)
		{
			return glm::vec3(Float(min, max), Float(min, max), Float(min, max));
		}

		static glm::vec4 Vec4()
		{
			return glm::vec4(Float(), Float(), Float(), Float());
		}

		static glm::vec4 Vec4(float min, float max)
		{
			return glm::vec4(Float(min, max), Float(min, max), Float(min, max), Float(min, max));
		}

		static glm::vec3 InUnitSphere()
		{
			return glm::normalize(Vec3(-1.0f, 1.0f));
		}

	private:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}
		friend class Application;

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

}