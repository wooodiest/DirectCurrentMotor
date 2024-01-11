#pragma once

#include <Luha.h>
#include <glm/glm.hpp>

namespace DCM {

	class Camera
	{
	public:
		Camera();
		~Camera();

		void OnUpdate(Luha::Timestep ts);
		void OnEvent(Luha::Event& e);

		void SetViewportSize(float width, float height);

		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::mat4& GetView() const { return m_View; }
		const glm::mat4& GetViewProjection() const { return m_Projection * m_View; }

	private:
		void RecalculateProjection();
		void RecalculateView();

		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;
		glm::vec3 CalculatePosition() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_View;

		glm::vec3 m_Position             = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint           = { 0.0f, 0.0f, 0.0f };
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_ViewportWidth  = 1280.0f;
		float m_ViewportHeight = 720.0f;
		float m_Pitch          = 0.0f;
		float m_Yaw            = 0.0f;
		float m_Distance       = 2.0f;

		float m_FOV = 45.0f;
		float m_Near           = 0.01f;
		float m_Far            = 1000.0f;
		float m_AspectRatio    = 1.778f;
	};

}