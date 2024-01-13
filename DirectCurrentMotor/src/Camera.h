#pragma once

#include <Luha.h>
#include <glm/glm.hpp>

namespace DCM {

	class Camera
	{
	public:
		Camera();
		~Camera();

		void OnUpdate(Luha::Timestep ts, bool active);
		void OnEvent(Luha::Event& e);

		void SetViewportSize(float width, float height);
		void ResetCamera();

		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::mat4& GetView() const { return m_View; }
		const glm::mat4& GetViewProjection() const { return m_Projection * m_View; }
		float GetDistance() const { return m_Distance; }

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
		float m_Pitch          = glm::radians(30.0f);
		float m_Yaw            = glm::radians(-30.0f);
		float m_Distance       = 15.0f;
		float m_MaxDistance    = 100.0f;
		float m_MinDistance    = 10.0f;

		float m_FOV            = 45.0f;
		float m_Near           = 0.01f;
		float m_Far            = 1000.0f;
		float m_AspectRatio    = 1.778f;
	};

}