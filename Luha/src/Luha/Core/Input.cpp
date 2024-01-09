#include "lhpch.h"

#include "Luha/Core/Input.h"
#include "Luha/Core/Application.h"

#include "GLFW/glfw3.h"

namespace Luha {

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		auto window = Application::Get().GetWindow().GetNativeWindow();
		auto state = glfwGetKey(window, static_cast<int32_t>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto window = Application::Get().GetWindow().GetNativeWindow();
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = Application::Get().GetWindow().GetNativeWindow();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMousePositionX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMousePositionY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}