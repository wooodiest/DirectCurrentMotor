#include "lhpch.h"

#include "Luha/Core/Window.h"
#include "Luha/Core/Application.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"

#include "Luha/Events/Event.h"
#include "Luha/Events/ApplicationEvent.h"
#include "Luha/Events/KeyEvent.h"
#include "Luha/Events/MouseEvent.h"

namespace Luha {

	static void GLFWErrorCallback(int error, const char* description)
	{
		LH_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window::Window(const Luha::ApplicationSpecification& spec)
	{
		LH_PROFILE_FUNCTION();

		// Check window size properties
		if ((spec.Window_Min_Height > spec.Window_Height)     || (spec.Window_Max_Height == 0 ? false : spec.Window_Height > spec.Window_Max_Height) ||
			(spec.Window_Min_Width  > spec.Window_Width)      || (spec.Window_Max_Width  == 0 ? false : spec.Window_Width  > spec.Window_Max_Width)  ||
			(spec.Window_Max_Height == 0 ? false : spec.Window_Min_Height > spec.Window_Max_Height) ||
			(spec.Window_Max_Width  == 0 ? false : spec.Window_Min_Width > spec.Window_Max_Width))
		{
			LH_CORE_ASSERT(false, "Invalid window size spec");
		}
		
		// Assign data
		m_Data.Title  = spec.Name;
		m_Data.Width  = spec.Window_Width;
		m_Data.Height = spec.Window_Height;
		m_Data.Vsync  = spec.VSync;
		m_Resizeable  = spec.Window_Resizeable;

		// GLFW init
		{
			LH_PROFILE_SCOPE("GLFW init");

			int success = glfwInit();
			LH_CORE_ASSERT(success, "Could not initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);

			if(success)
				LH_CORE_TRACE("GLFW has been initialized successfully");
		}

		// Create window and set size limits
		{
			LH_PROFILE_SCOPE("Creating window");

			m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
			LH_CORE_TRACE("Created window : {0} ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);
		}
		
		if (spec.Window_Resizeable)
		{
			glfwSetWindowSizeLimits(m_Window, spec.Window_Min_Width, spec.Window_Min_Height,
				spec.Window_Max_Width == 0 ? GLFW_DONT_CARE : spec.Window_Max_Width, spec.Window_Max_Height == 0 ? GLFW_DONT_CARE : spec.Window_Max_Height);
		}
		else
		{
			glfwSetWindowSizeLimits(m_Window, spec.Window_Width, spec.Window_Height, spec.Window_Width, spec.Window_Height);
		}
		
		glfwMakeContextCurrent(m_Window);

		// Glad init
		{
			LH_PROFILE_SCOPE("Glad init");

			int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			LH_CORE_ASSERT(success, "Failed to initialize Glad!");

			if (success)
				LH_CORE_TRACE("Glad has been initialized successfully");
		}
		
		LH_CORE_TRACE("Vendor: {0}", (char*)glGetString(GL_VENDOR));
		LH_CORE_TRACE("Renderer: {0}", (char*)glGetString(GL_RENDERER));
		LH_CORE_TRACE("Version: {0}", (char*)glGetString(GL_VERSION));
		LH_CORE_TRACE("Shading: {0}", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		LH_CORE_TRACE("Blending: ONE_MINUS_SRC_ALPHA");

		// Set local window data
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(m_Data.Vsync);

		// Set GLFW callbacks
		{
			LH_PROFILE_SCOPE("Set GLFW callbacks");
			
			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					data.Width = width;
					data.Height = height;

					WindowResizeEvent event(width, height);
					data.EventCallback(event);
				});

			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					WindowCloseEvent event;
					data.EventCallback(event);
				});

			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					switch (action)
					{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(static_cast<KeyCode>(key), 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(static_cast<KeyCode>(key));
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(static_cast<KeyCode>(key), 1);
						data.EventCallback(event);
						break;
					}
					}
				});

			glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					KeyTypedEvent event(static_cast<KeyCode>(keycode));
					data.EventCallback(event);
				});

			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					switch (action)
					{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(static_cast<MouseCode>(button));
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
						data.EventCallback(event);
						break;
					}
					}
				});

			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					MouseScrolledEvent event((float)xOffset, (float)yOffset);
					data.EventCallback(event);
				});

			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					MouseMovedEvent event((float)xPos, (float)yPos);
					data.EventCallback(event);
				});
		}
	}
	Window::~Window()
	{
		Shutdown();
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	bool Window::IsMaximized() const
	{
		return (bool)glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED);
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
		m_FrameCount++;
	}

	void Window::SetEventCallback(const EventCallbackFn& callback)
	{
		m_Data.EventCallback = callback;
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.Vsync = enabled;
		Application::Get().m_AppSpec.VSync = enabled;

		char* ch = enabled ? "enabled" : "disabled";
		LH_CORE_TRACE("VSync: {0}", ch);
	}
}