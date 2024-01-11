#pragma once

#include <Luha.h>
#include "MotorSpecification.h"
#include "Shader.h"
#include "Camera.h"

#include <memory>
#include <vector>

namespace DCM {

	class Engine
	{
	public:
		Engine();
		~Engine();

		Camera* GetCamera() { return m_Camera; }

		void SetData(MotorSpecification& spec, Camera& camera);
		void OnUpdate(Luha::Timestep ts);
		void OnEvent(Luha::Event& e);
		uint32_t GetData() const { return m_ColorAttachment; }

		void SetViewportSize(float width, float height);

	private:
		MotorSpecification* m_Spec;
		Camera* m_Camera;

		float m_ViewportWidth = 0, m_ViewportHeight = 0;

		uint32_t m_RendererID = 0, m_ColorAttachment = 0, m_DepthAttachment = 0;

		/// Test
		unsigned int m_VertexBuffer, m_VertexArray;
		Shader* m_Shader;
	};

}