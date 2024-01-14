#pragma once

#include <Luha.h>
#include "MotorSpecification.h"
#include "Shader.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace DCM {

	class Simulation
	{
	public:
		// temporary
		friend class DirectCurrentMotor;
		//
		Simulation();
		~Simulation();

		Camera* GetCamera() { return m_Camera; }

		void SetData(MotorSpecification& spec, Camera& camera);
		void OnUpdate(Luha::Timestep ts);
		void OnEvent(Luha::Event& e);
		uint32_t GetData() const { return m_ColorAttachment; }

		void SetViewportSize(float width, float height);

		void DisplaySceneOptions();

	private:
		bool m_ShowAxes = true;

	private:
		MotorSpecification* m_Spec;
		Camera* m_Camera;

		glm::vec4 m_ClearColor = { 0.29f, 0.314f, 0.322f, 1.0f };

		float m_ViewportWidth = 0, m_ViewportHeight = 0;
		uint32_t m_RendererID = 0, m_ColorAttachment = 0, m_DepthAttachment = 0;

		// Engine
		Shader* m_EngineShader;
		unsigned int m_EngineVertexBuffer, m_EngineIndexBuffer, m_EngineVertexArray;
		unsigned int m_EngineIndicesCount = 0;
		glm::vec3 m_EngineInitialRotation{ glm::radians(90.0f), 0.0f, 0.0f};
		
		// Coordinate system
		Shader* m_CoordShader;
		unsigned int m_CoordVertexBuffer, m_CoordVertexArray;

	};

}