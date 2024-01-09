#pragma once

#include "Luha/Events/Event.h"
#include "Luha/Events/ApplicationEvent.h"
#include "Luha/Events/KeyEvent.h"
#include "Luha/Events/MouseEvent.h"

#include "Luha/Core/LayerStack.h"
#include "Luha/Core/Window.h"

#include "Luha/Utils/Utils.h"

#include "glm/glm.hpp"
#include "imgui.h"
#include "implot.h"

namespace Luha {

	// Struct that describes application
	struct ApplicationSpecification
	{
		std::string Name              = "Luha App";
		uint32_t    Window_Width      = 1600;
		uint32_t    Window_Height     = 900;
		uint32_t    Window_Min_Width  = 0;
		uint32_t    Window_Min_Height = 0;
		uint32_t    Window_Max_Width  = 0;    // no limit
		uint32_t    Window_Max_Height = 0;    // no limit
		bool        Window_Resizeable = true;
		bool        VSync             = true; // TODO: Custom frame lock
		bool        MenuBar           = true;
		AppColorTheme  ColorThema     = AppColorTheme::Dark;
		PlotColorTheme PlotThema      = PlotColorTheme::Auto;
		PlotColorMap   PlotColor_Map  = PlotColorMap::Deep; // Deafult colors for plots
		AppFont        Font           = AppFont::Roboto;
		float          FontSize       = 20.0f;
	};								  
	
	class Application
	{
	public:
		friend class Window;

		Application(const ApplicationSpecification& spec = ApplicationSpecification());
		virtual ~Application();

		// Runloop
		void Run();
		void OnEvent(Event& e);

		// Management
		void Init(const ApplicationSpecification& spec);
		void Close();
		void PushLayer(Layer* layer);

		// Getters
		inline static Application& Get()       { return *s_Instance; }
		inline Window&             GetWindow() { return *m_Window; }
		ApplicationSpecification   GetSpec()      const { return m_AppSpec; }
		Timestep                   GetDeltaTime() const { return m_DeltaTime;};
		float                      GetTime()      const;

	private:
		// On Event
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

		// Serialize 
		void Serialize();
		void SerializeApplication();
		bool DeserializeApplication();

	private:
		// Application data
		ApplicationSpecification m_AppSpec;
		Scope<Window> m_Window;
		LayerStack    m_LayerStack;

		glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		bool m_Running   = true;
		bool m_Minimized = false;
		Timestep m_LastFrameTime = 0.0f;
		Timestep m_DeltaTime     = 0.0f;

		ImFont* m_Font = nullptr;
		bool    m_FontChanged = false;

	private:	
		// ImGui
		void InitImGui();
		void ShutdownImGui();

		void BeginImGuiFrame();
		void RenderImGuiFrame();

		void BeginMainWindow();
		void EndMainWindow();
		void RenderMainMenu();
		void RenderAdditionalWindows();

		void SetImGuiTheme();
		void SetImPlotTheme();
		void LoadFont();

		bool m_ShowImGuiDebugger = false;
		bool m_ShowImGuiStackTool = false;
		bool m_ShowImGuiStyleTool = false;
		bool m_ShowImPlotDebugger = false;
		bool m_ShowImPlotStyleTool = false;

	private:
		static Application* s_Instance;
	};

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);
}