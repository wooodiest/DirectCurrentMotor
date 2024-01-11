#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <imgui.h>

namespace DCM {

	Camera::Camera()
	{
		RecalculateProjection();
		RecalculateView();
	}

	Camera::~Camera()
	{
	}

	void Camera::OnUpdate(Luha::Timestep ts)
	{
		
		if (Luha::Input::IsKeyPressed(Luha::KeyCode::LeftControl) && Luha::Input::IsMouseButtonPressed(Luha::Mouse::ButtonLeft))
		{
			const glm::vec2& mouse = { Luha::Input::GetMousePositionX(), Luha::Input::GetMousePositionY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			MouseRotate(delta);
			RecalculateView();	
		}

	}

	void Camera::OnEvent(Luha::Event& e)
	{
		if (e.GetEventType() == Luha::EventType::MouseScrolled)
		{
			auto ev = (Luha::MouseScrolledEvent&)e;
			float delta = ev.GetYOffset() * 0.1f;
			MouseZoom(delta);
			RecalculateView();
		}
	}

	void Camera::SetViewportSize(float width, float height)
	{
		if (m_ViewportWidth != width || m_ViewportHeight != height)
		{
			m_ViewportWidth = width;
			m_ViewportHeight = height;

			m_AspectRatio = m_ViewportWidth / m_ViewportHeight;

			RecalculateProjection();
		}	
	}

	void Camera::RecalculateView()
	{
		m_Position = CalculatePosition();
		glm::quat orientation = GetOrientation();
		m_View = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_View = glm::inverse(m_View);
	}

	void Camera::RecalculateProjection()
	{
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
	}

	void Camera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void Camera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 Camera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 Camera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 Camera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat Camera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	glm::vec3 Camera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	float Camera::RotationSpeed() const
	{
		return 0.8f;
	}

	float Camera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.4f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}

	

}