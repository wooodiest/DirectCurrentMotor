#include "lhpch.h"

#include "Luha/Core/Application.h"
#include "Luha/Core/Input.h"

#include "Luha/Utils/Timestep.h"
#include "Luha/Utils/Time.h"
#include "Luha/Utils/Random.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "implot.h"
#include "implot_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <fstream>
#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace Luha {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& spec)
	{
		LH_PROFILE_FUNCTION();

		Init(spec);
	}

	Application::~Application()
	{
		LH_PROFILE_FUNCTION();

		Close();
		SerializeApplication();
	}

	void Application::Run()
	{
		LH_PROFILE_FUNCTION();

		while (m_Running)
		{
			LH_PROFILE_SCOPE("RunLoop");

			// Renderer commands
			glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
			glClear(GL_COLOR_BUFFER_BIT);

			// Calculate delta time
			Timestep time = GetTime();
			m_DeltaTime = time - m_LastFrameTime;
			m_DeltaTime = m_DeltaTime > 0.016f ? 0.016f : m_DeltaTime;
			m_LastFrameTime = time;
		
			// Update layer stack
			{
				LH_PROFILE_SCOPE("Update layer stack");

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(m_DeltaTime);							
			}

			// If font has been changed: reload it and create atlas
			if (m_FontChanged)
				LoadFont();

			// Renderer submissions
			BeginImGuiFrame();
			ImGui::PushFont(m_Font);

			// Main menu
			{   LH_PROFILE_SCOPE("Render main menu");

				if (m_AppSpec.MenuBar && ImGui::BeginMainMenuBar())
				{
					RenderMainMenu();

					for (Layer* layer : m_LayerStack)
						layer->OnMenuRender();

					ImGui::EndMainMenuBar();
				}
			}

			// Layer stack
			{   LH_PROFILE_SCOPE("Render layer stack");
				
				BeginMainWindow(); // Main docking window - windows still can be dragged outside

				for (Layer* layer : m_LayerStack)
					layer->OnRender();

				// Additional windows (Debug tools)
				RenderAdditionalWindows();

				EndMainWindow();
			}

			// Render
			ImGui::PopFont();
			RenderImGuiFrame();

			// Update window, pool events, swap buffers
			m_Window->OnUpdate();
		}
	}

	void Application::Init(const ApplicationSpecification& spec)
	{
		s_Instance = this;
		m_AppSpec = spec;

#ifdef LH_SERIALIZING

		bool status = DeserializeApplication();
		Log::Init();
		if(status)
			LH_CORE_INFO("Application data loaded successfully");
		else
			LH_CORE_ERROR("Cannot load application data");

#endif

		m_Window = CreateScope<Window>(m_AppSpec);
		m_Window->SetEventCallback(LH_BIND_EVENT_FN(Application::OnEvent));

		InitImGui();
		Random::Init();;
	}

	void Application::Close()
	{
		LH_PROFILE_FUNCTION();

		ShutdownImGui();
		s_Instance = nullptr;
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(LH_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(LH_BIND_EVENT_FN(Application::OnWindowResize));
		dispatcher.Dispatch<KeyPressedEvent>(LH_BIND_EVENT_FN(Application::OnKeyPressed));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	float Application::GetTime() const
	{
		return (float)glfwGetTime();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		LH_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		m_AppSpec.Window_Height = e.GetHeight();
		m_AppSpec.Window_Width = e.GetWidth();

		return false;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
			case Key::S:
			{
				if (control)
				{
					Serialize();
				}
				break;
			}
			case Key::F4:
			{
				if (Input::IsKeyPressed(Key::LeftAlt))
				{
				}
			}
		}
		return false;
	}

	void Application::Serialize()
	{
#ifdef LH_SERIALIZING

		SerializeApplication();

		for (Layer* layer : m_LayerStack)
			layer->Serialize();

#endif
	}

	void Application::SerializeApplication()
	{
#ifdef LH_SERIALIZING
		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "Name"              << YAML::Value << m_AppSpec.Name;
		out << YAML::Key << "Window_Width"      << YAML::Value << m_AppSpec.Window_Width;
		out << YAML::Key << "Window_Height"     << YAML::Value << m_AppSpec.Window_Height;
		out << YAML::Key << "Window_Min_Width"  << YAML::Value << m_AppSpec.Window_Min_Width;
		out << YAML::Key << "Window_Min_Height" << YAML::Value << m_AppSpec.Window_Min_Height;
		out << YAML::Key << "Window_Max_Width"  << YAML::Value << m_AppSpec.Window_Max_Width;
		out << YAML::Key << "Window_Max_Height" << YAML::Value << m_AppSpec.Window_Max_Height;
		out << YAML::Key << "Window_Resizeable" << YAML::Value << m_AppSpec.Window_Resizeable;
		out << YAML::Key << "VSync"             << YAML::Value << m_AppSpec.VSync;
		out << YAML::Key << "MenuBar"           << YAML::Value << m_AppSpec.MenuBar;
		out << YAML::Key << "ColorThema"        << YAML::Value << (int)m_AppSpec.ColorThema;
		out << YAML::Key << "PlotThema"         << YAML::Value << (int)m_AppSpec.PlotThema;
		out << YAML::Key << "PlotColor_Map"     << YAML::Value << (int)m_AppSpec.PlotColor_Map;
		out << YAML::Key << "Font"              << YAML::Value << (int)m_AppSpec.Font;
		out << YAML::Key << "FontSize"          << YAML::Value << m_AppSpec.FontSize;

		out << YAML::Key << "LogToFile"    << YAML::Value << Log::LogToFile;
		out << YAML::Key << "LogToConsole" << YAML::Value << Log::LogToConsole;
		out << YAML::Key << "LuhaLogLevel" << YAML::Value << (int)Log::s_CoreLevel;
		out << YAML::Key << "AppLogLevel"  << YAML::Value << (int)Log::s_ClientLevel;

		out << YAML::EndMap;

		std::string filePath = "data/Application.Luha";
		std::filesystem::path fs_path = std::filesystem::path(filePath).parent_path();
		if (!std::filesystem::exists(fs_path)) {
			if (!std::filesystem::create_directories(fs_path)) {
				LH_CORE_ASSERT(false, "Error creating directory: {0}", (char*)fs_path.c_str());
			}
		}

		std::ofstream fout(filePath);
		if (fout.good())
		{
			fout << out.c_str();
			LH_CORE_INFO("Application data saved successfully");
		}
		else
		{
			LH_CORE_ERROR("Error saving application data");
		}
#endif
	}

	bool Application::DeserializeApplication()
	{
		try {
			ApplicationSpecification spec;

			std::ifstream stream("data/Application.Luha");
			std::stringstream strStream;
			strStream << stream.rdbuf();
			YAML::Node doc = YAML::Load(strStream.str());

			spec.Name              = doc["Name"].as<std::string>();
			spec.Window_Width      = doc["Window_Width"].as<int>();
			spec.Window_Height     = doc["Window_Height"].as<int>();
			spec.Window_Min_Width  = doc["Window_Min_Width"].as<int>();
			spec.Window_Min_Height = doc["Window_Min_Height"].as<int>();
			spec.Window_Max_Width  = doc["Window_Max_Width"].as<int>();
			spec.Window_Max_Height = doc["Window_Max_Height"].as<int>();
			spec.VSync             = doc["VSync"].as<bool>();
			spec.Window_Resizeable = doc["Window_Resizeable"].as<bool>();
			spec.MenuBar           = doc["MenuBar"].as<bool>();
			spec.ColorThema        = (AppColorTheme)doc["ColorThema"].as<int>();
			spec.PlotThema         = (PlotColorTheme)doc["PlotThema"].as<int>();
			spec.PlotColor_Map     = (PlotColorMap)doc["PlotColor_Map"].as<int>();
			spec.Font              = (AppFont)doc["Font"].as<int>();
			spec.FontSize          = doc["FontSize"].as<float>();

			bool logToFile    = doc["LogToFile"].as<bool>();
			bool logToConsole = doc["LogToConsole"].as<bool>();
			auto logCoreLevel = (spdlog::level::level_enum)doc["LuhaLogLevel"].as<int>();
			auto logAppLevel  = (spdlog::level::level_enum)doc["AppLogLevel"].as<int>();

			///		
			m_AppSpec          = spec;
			Log::LogToFile     = logToFile;
			Log::LogToConsole  = logToConsole;
			Log::s_CoreLevel   = logCoreLevel;
			Log::s_ClientLevel = logAppLevel;
		}
		catch (...)
		{
			return false;
		}
		return true;
	}

	void Application::InitImGui()
	{
		LH_PROFILE_FUNCTION();
		
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.IniFilename = "data/imgui.ini";
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		SetImGuiTheme();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

		io.Fonts->AddFontDefault();
		LoadFont();
		
		ImPlot::CreateContext();

		ImPlotContext& gp = *ImPlot::GetCurrentContext();
		gp.Style.Colormap = static_cast<ImPlotColormap>(m_AppSpec.PlotColor_Map);
		ImPlot::BustItemCache();
	}

	void Application::ShutdownImGui()
	{
		LH_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}

	void Application::BeginImGuiFrame()
	{
		LH_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Application::RenderImGuiFrame()
	{
		LH_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void Application::BeginMainWindow()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		if (m_AppSpec.MenuBar)
			window_flags |= ImGuiWindowFlags_MenuBar;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);

		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
		ImGui::PopStyleColor(); // MenuBarBg
		ImGui::PopStyleVar(2);
		ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		ImGui::DockSpace(ImGui::GetID("MyDockspace"));
		style.WindowMinSize.x = minWinSizeX;
	}

	void Application::EndMainWindow()
	{
		ImGui::End();
	}

	void Application::SetImGuiTheme()
	{
		switch (m_AppSpec.ColorThema)
		{
			case AppColorTheme::Dark:    ImGui::StyleColorsDark(); break;
			case AppColorTheme::Classic: ImGui::StyleColorsClassic(); break;
			case AppColorTheme::Light:   ImGui::StyleColorsLight(); break;
			default:                     ImGui::StyleColorsDark(); break;
		}
	}

	void Application::SetImPlotTheme()
	{
		switch (m_AppSpec.PlotThema)
		{
			case PlotColorTheme::Dark:    ImPlot::StyleColorsDark(); break;
			case PlotColorTheme::Classic: ImPlot::StyleColorsClassic(); break;
			case PlotColorTheme::Light:   ImPlot::StyleColorsLight(); break;
			case PlotColorTheme::Auto:    ImPlot::StyleColorsAuto(); break;
			default:                      ImPlot::StyleColorsAuto(); break;
		}
	}

	void Application::RenderAdditionalWindows()
	{
		if (m_ShowImGuiDebugger)
		{
			ImGui::ShowMetricsWindow(&m_ShowImGuiDebugger);

		}

		if (m_ShowImGuiStackTool)
		{
			ImGui::ShowStackToolWindow(&m_ShowImGuiStackTool);

		}

		if (m_ShowImGuiStyleTool)
		{
			ImGui::Begin("Dear ImGui Style Editor", &m_ShowImGuiStyleTool);
			ImGui::ShowStyleEditor();
			ImGui::End();

		}

		if (m_ShowImPlotDebugger)
		{
			ImPlot::ShowMetricsWindow(&m_ShowImPlotDebugger);
		}
		
		if (m_ShowImPlotStyleTool)
		{
			ImGui::Begin("ImPlot Style Editor", &m_ShowImPlotStyleTool);
			ImPlot::ShowStyleEditor();
			ImGui::End();
		}
	}

	void Application::RenderMainMenu()
	{
		if(ImGui::BeginMenu("Menu"))
		{
			// Save
			if (ImGui::MenuItem("Save##MainMenuSave", "Ctrl+S"))
			{
				Serialize();
			}

			// Vsync
			/*
			ImGui::Separator();
			bool vsync = GetWindow().IsVSync();
			if (ImGui::MenuItem("Vsync##MainMenuVsync", "", &vsync))
			{
				GetWindow().SetVSync(vsync);
				SerializeApplication();
			}
			*/
					
			// Theme
			ImGui::Separator();
			if (ImGui::BeginMenu("Theme##MainMenuTheme"))
			{
				// Application
				{
					ImGui::SeparatorText("Application##MainMenu");
					int selected = static_cast<int>(m_AppSpec.ColorThema);

					if (ImGui::Selectable("Dark  ##MainMenuAppThemeDark", selected == 1))
					{
						selected = 1;
						m_AppSpec.ColorThema = static_cast<AppColorTheme>(selected);
						SetImGuiTheme();
						SerializeApplication();
					}
					if (ImGui::Selectable("Classic  ##MainMenuAppThemeClassic", selected == 2))
					{
						selected = 2;
						m_AppSpec.ColorThema = static_cast<AppColorTheme>(selected);
						SetImGuiTheme();
						SerializeApplication();
					}
					if (ImGui::Selectable("Light  ##MainMenuAppThemeLight", selected == 3))
					{
						selected = 3;
						m_AppSpec.ColorThema = static_cast<AppColorTheme>(selected);
						SetImGuiTheme();
						SerializeApplication();
					}
				}
				// Plots
				{
					ImGui::SeparatorText("Plots");

					int selected = static_cast<int>(m_AppSpec.PlotThema);

					if (ImGui::Selectable("Auto  ##MainMenuPlotThemeAuto", selected == 1))
					{
						selected = 1;
						m_AppSpec.PlotThema = static_cast<PlotColorTheme>(selected);
						SetImPlotTheme();
						SerializeApplication();
					}

					if (ImGui::Selectable("Dark  ##MainMenuPlotThemeDark", selected == 2))
					{
						selected = 2;
						m_AppSpec.PlotThema = static_cast<PlotColorTheme>(selected);
						SetImPlotTheme();
						SerializeApplication();
					}
					if (ImGui::Selectable("Classic  ##MainMenuPlotThemeClassic", selected == 3))
					{
						selected = 3;
						m_AppSpec.PlotThema = static_cast<PlotColorTheme>(selected);
						SetImPlotTheme();
						SerializeApplication();
					}
					if (ImGui::Selectable("Light  ##MainMenuPlotThemeLight", selected == 4))
					{
						selected = 4;
						m_AppSpec.PlotThema = static_cast<PlotColorTheme>(selected);
						SetImPlotTheme();
						SerializeApplication();
					}
					ImGui::Separator();

					// Plots color map
					{
						ImPlotContext& gp = *ImPlot::GetCurrentContext();
						bool set = false;
						if (ImGui::BeginCombo(" Color map ##MainMenuColormap", gp.ColormapData.GetName(gp.Style.Colormap))) {
							for (int i = 0; i < gp.ColormapData.Count; ++i) {
								const char* name = gp.ColormapData.GetName(i);
								if (ImGui::Selectable(name, gp.Style.Colormap == i)) {
									gp.Style.Colormap = i;
									m_AppSpec.PlotColor_Map = static_cast<PlotColorMap>(i);
									ImPlot::BustItemCache();
									SerializeApplication();
									set = true;
								}
							}
							ImGui::EndCombo();
						}
					}
				}
				
				ImGui::EndMenu();
			} // Theme

			// Font
			if (ImGui::BeginMenu("Font##MainMenuFont"))
			{
				if (ImGui::DragFloat(" Font size ##MainMenuFontSize", &m_AppSpec.FontSize, 0.5f, 9.0f, 50.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					m_FontChanged = true;
					SerializeApplication();
				}

				ImGui::Separator();
				int selected = static_cast<int>(m_AppSpec.Font);
				if (ImGui::Selectable("Roboto##MainMenuFont-Roboto", selected == 1))
				{
					selected = 1;
					m_AppSpec.Font = static_cast<AppFont>(selected);
					m_FontChanged = true;
					SerializeApplication();
				}
				if (ImGui::Selectable("OpenSans##MainMenuFont-OpenSans", selected == 2))
				{
					selected = 2;
					m_AppSpec.Font = static_cast<AppFont>(selected);
					m_FontChanged = true;
					SerializeApplication();
				}
				if (ImGui::Selectable("Oswald##MainMenuFont-Oswald", selected == 3))
				{
					selected = 3;
					m_AppSpec.Font = static_cast<AppFont>(selected);
					m_FontChanged = true;
					SerializeApplication();
				}
				if (ImGui::Selectable("Montserrat##MainMenuFont-Montserrat", selected == 4))
				{
					selected = 4;
					m_AppSpec.Font = static_cast<AppFont>(selected);
					m_FontChanged = true;
					SerializeApplication();
				}
				ImGui::EndMenu();
			} // Font

			// Tools
			ImGui::Separator();
			if (ImGui::BeginMenu("Tools##MainMenuTools"))
			{
				if (ImGui::MenuItem("UI Debugger##MainMenuTools-UIDebugger", "", &m_ShowImGuiDebugger))
				{
					if(m_ShowImGuiDebugger)
						LH_CORE_TRACE("Added window: UI Debugger");
				}
				
				ImGui::Separator();
				if (ImGui::MenuItem("UI Stack tool##MainMenuTools-UIStack", "", &m_ShowImGuiStackTool)) 
				{
					if (m_ShowImGuiStackTool)
						LH_CORE_TRACE("Added window: UI Stack Tool");
				}
				
				ImGui::Separator();
				if (ImGui::MenuItem("UI Style tool##MainMenuTools-UIStyle", "", &m_ShowImGuiStyleTool))
				{
					if (m_ShowImGuiStyleTool)
						LH_CORE_TRACE("Added window: UI Style Tool");
				}
				
				ImGui::Separator();
				if (ImGui::MenuItem("Plot Debugger##MainMenuTools-PlotDebbuger", "", &m_ShowImPlotDebugger))
				{
					if (m_ShowImPlotDebugger)
						LH_CORE_TRACE("Added window: Plot Debugger");
				}
				
				ImGui::Separator();
				if (ImGui::MenuItem("Plot Style tool##MainMenuTools-PlotStyle", "", &m_ShowImPlotStyleTool))
				{
					if (m_ShowImPlotStyleTool)
						LH_CORE_TRACE("Added window: Plot Style Tool");
				}

				ImGui::EndMenu();
			}
			 
			// Logs
			if (ImGui::BeginMenu("Logs##MainMenuLogs"))
			{
				ImGui::Text("Log to:");
			
#if defined(LH_PLATFORM_WINDOWS) && defined(LH_DIST) && LH_CONSOLE_IN_DISTRIBUTION_BUILD == 0
	// No consol			
#else
				if (ImGui::Checkbox("Console", &Log::LogToConsole))
				{
					Log::UpdateLogger();
					SerializeApplication();
				}
#endif
				if (ImGui::Checkbox("File", &Log::LogToFile))
				{
					Log::UpdateLogger();
					SerializeApplication();
				}

				ImGui::SeparatorText("Log level");

				char* levels[6] = { "Trace", "", "Info", "Warn", "Error", "Critical" };

				{
					int selected = static_cast<int>(Log::s_CoreLevel);
					if (ImGui::BeginCombo("Luha", levels[selected]))
					{
						if (ImGui::Selectable("Trace##MainMenuLogsCoreTrace", selected == 0))
						{
							selected = 0;
							Log::s_CoreLevel = spdlog::level::trace;
							Log::UpdateLogger();
							SerializeApplication();
						}
						if (ImGui::Selectable("Info##MainMenuLogsCoreInfo", selected == 2))
						{
							selected = 2;
							Log::s_CoreLevel = spdlog::level::info;
							Log::UpdateLogger();
							SerializeApplication();
						}
						if (ImGui::Selectable("Warn##MainMenuLogsCoreWorn", selected == 3))
						{
							selected = 3;
							Log::s_CoreLevel = spdlog::level::warn;
							Log::UpdateLogger();
							SerializeApplication();
						}
						if (ImGui::Selectable("Error##MainMenuLogsCoreError", selected == 4))
						{
							selected = 4;
							Log::s_CoreLevel = spdlog::level::err;
							Log::UpdateLogger();
							SerializeApplication();
						}
						if (ImGui::Selectable("Critical##MainMenuLogsCoreCritical", selected == 5))
						{
							selected = 5;
							Log::s_CoreLevel = spdlog::level::critical;
							Log::UpdateLogger();
							SerializeApplication();
						}

						ImGui::EndCombo();
					}
				}
				
				{
					int selected = static_cast<int>(Log::s_ClientLevel);
					if (ImGui::BeginCombo("App", levels[selected]))
					{
						if (ImGui::Selectable("Trace##MainMenuLogsAppTrace", selected == 0))
						{
							selected = 0;
							Log::s_ClientLevel = spdlog::level::trace;
							Log::UpdateLogger();
							SerializeApplication();
						}
						if (ImGui::Selectable("Info##MainMenuLogsAppInfo", selected == 2))
						{
							selected = 2;
							Log::s_ClientLevel = spdlog::level::info;
							Log::UpdateLogger();
							SerializeApplication();
						}
						if (ImGui::Selectable("Warn##MainMenuLogsAppWorn", selected == 3))
						{
							selected = 3;
							Log::s_ClientLevel = spdlog::level::warn;
							Log::UpdateLogger();
							SerializeApplication();
						}
						if (ImGui::Selectable("Error##MainMenuLogsAppError", selected == 4))
						{
							selected = 4;
							Log::s_ClientLevel = spdlog::level::err;
							Log::UpdateLogger();
							SerializeApplication();
						}
						if (ImGui::Selectable("Critical##MainMenuLogsAppCritical", selected == 5))
						{
							selected = 5;
							Log::s_ClientLevel = spdlog::level::critical;
							Log::UpdateLogger();
							SerializeApplication();
						}

						ImGui::EndCombo();
					}
				}
			ImGui::EndMenu();
		}
			
			// Exit
			ImGui::Separator();
			if (ImGui::MenuItem("Exit##MainMenuExit", "Alt+F4"))
			{
				m_Running = false;
			}

			ImGui::EndMenu();
		}
	}

	void Application::LoadFont()
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts->Clear();

		std::string filePath = "";
		char* font;
		switch (m_AppSpec.Font)
		{
			case AppFont::Roboto:     filePath = "assets/fonts/Roboto-Regular.ttf";     font = "Roboto-Regular";     break;
			case AppFont::Montserrat: filePath = "assets/fonts/Montserrat-Regular.ttf"; font = "Montserrat-Regular"; break;
			case AppFont::Oswald:     filePath = "assets/fonts/Oswald-Regular.ttf";     font = "Oswald-Regular";     break;
			case AppFont::OpenSans:   filePath = "assets/fonts/OpenSans-Regular.ttf";   font = "OpenSans-Regular";   break;
		}

		std::filesystem::path path = std::filesystem::path(filePath);
		if (!std::filesystem::exists(path) || filePath == "")
		{
			LH_CORE_ERROR("Invalid font path");
			io.Fonts->AddFontDefault();
			m_Font = io.Fonts->Fonts[0];

			LH_CORE_ERROR("Deafult font loaded, cannot apply font size");
		}
		else
		{
			m_Font = io.Fonts->AddFontFromFileTTF(filePath.c_str(), m_AppSpec.FontSize);
			
			LH_CORE_TRACE("Font \"{0}\" loaded", font);
		}
		
		ImGui_ImplOpenGL3_CreateFontsTexture();
		m_FontChanged = false;
	}

}