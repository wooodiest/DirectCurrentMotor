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

	void Camera::OnUpdate(Luha::Timestep ts, bool active)
	{
		const glm::vec2& mouse = { Luha::Input::GetMousePositionX(), Luha::Input::GetMousePositionY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;

		if (active && Luha::Input::IsMouseButtonPressed(Luha::Mouse::ButtonLeft))
		{
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

	void Camera::ResetCamera()
	{
		m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		m_Position = { 0.0f, 0.0f, 0.0f };
		m_Pitch = 0.0f;
		m_Yaw = 0.0f;
		m_Distance = 15.0f;

		RecalculateView();
	}

	void Camera::SetDefaultValues()
	{
		m_Pitch          = glm::radians(30.0f);
		m_Yaw            = glm::radians(-30.0f);
		m_Distance       = 15.0f;
		m_MaxDistance    = 100.0f;
		m_MinDistance    = 10.0f;
		m_FOV            = 45.0f;
		m_Near           = 0.01f;
		m_Far            = 1000.0f;

		m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		m_Position = { 0.0f, 0.0f, 0.0f };

		RecalculateView();
		RecalculateProjection();
	}

	void Camera::DisplayCameraSettings()
	{
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		const float buttonSpacing = 5.0f;
		ImVec2 buttonSize = { (viewportSize.x - buttonSpacing) / 2, 0.0f };

		ImGui::Separator();

		if (ImGui::Button("Reset##camera", buttonSize))
			ResetCamera();
		ImGui::SameLine(0.0f, buttonSpacing);
		if (ImGui::Button("Set default##camera", buttonSize))
			SetDefaultValues();

		ImGui::Separator();

		if (ImGui::DragFloat3("Focal point", &m_FocalPoint.x))
			RecalculateView();
		float pitch = Utils::RadToDeg(m_Pitch);
		if (ImGui::DragFloat("Pitch", &pitch, 0.5f))
		{
			m_Pitch = Utils::DegToRad(pitch);
			RecalculateView();
		}
		float yaw = Utils::RadToDeg(m_Yaw);
		if (ImGui::DragFloat("Yaw", &yaw, 0.5f))
		{
			m_Yaw = Utils::DegToRad(yaw);
			RecalculateView();
		}
		if (ImGui::DragFloat("Distance", &m_Distance, 0.25f, m_MinDistance, m_MaxDistance))
			RecalculateView();

		ImGui::DragFloat("Min distance", &m_MinDistance);
		ImGui::DragFloat("Max distance", &m_MaxDistance);

		ImGui::Separator();

		if (ImGui::DragFloat("Fov", &m_FOV, 0.5f, 1.0f, 90.0f))
			RecalculateProjection();
		if (ImGui::DragFloat("Near clip", &m_Near, 0.5f))
			RecalculateProjection();
		if (ImGui::DragFloat("Far clip", &m_Far, 0.5f))
			RecalculateProjection();

		ImGui::Separator();

		ImGui::Text("Viewport size: (%f, %f)", m_ViewportWidth, m_ViewportHeight);
		ImGui::Text("Asspect ratio: %f", m_AspectRatio);
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
		if (m_Distance < m_MinDistance)
			m_Distance = m_MinDistance;
		else if (m_Distance > m_MaxDistance)
			m_Distance = m_MaxDistance;
		
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
		float distance = m_Distance * 0.3f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}
}