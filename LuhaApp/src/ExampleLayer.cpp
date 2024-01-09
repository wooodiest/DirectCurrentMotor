#include "ExampleLayer.h"

#include <imgui.h>
#include <implot.h>

#include <filesystem>
#include <yaml-cpp/yaml.h>

#include <glm/gtc/type_ptr.hpp>

namespace ExampleApp {

	ExampleLayer::ExampleLayer()
	{
		LH_PROFILE_FUNCTION();
	}

	ExampleLayer::~ExampleLayer()
	{
		LH_PROFILE_FUNCTION();

		Serialize();
	}

	void ExampleLayer::OnAttach()
	{
		LH_PROFILE_FUNCTION();

		Deserialize();

		m_Image.LoadData("assets/img/opengl_logo.png", Luha::ImageFilter::Linear);
	}

	void ExampleLayer::OnDetach()
	{
		LH_PROFILE_FUNCTION();
	}

	void ExampleLayer::OnUpdate(Luha::Timestep ts)
	{
		LH_PROFILE_FUNCTION();

		if (Luha::Input::IsKeyPressed(Luha::KeyCode::A))
			LH_INFO("Key A is pressed, random number: {0}", Luha::Random::Int());

		if (Luha::Input::IsMouseButtonPressed(Luha::MouseCode::ButtonRight))
			LH_INFO("Left mouse button is clicked: ({0}, {1})", Luha::Input::GetMousePositionX(), Luha::Input::GetMousePositionY());
	
		static auto clock = Luha::RepeatClock(Luha::Timestep(3.0f), false, []() { LH_INFO("Hi"); });
		clock.OnUpdate();

	}

	void ExampleLayer::OnRender()
	{
		LH_PROFILE_FUNCTION();

		ImGui::ShowDemoWindow();
		ImPlot::ShowDemoWindow();

		ImGui::Begin("Example Window");
		ImGui::TextColored(Utils::GlmVec4_To_ImVec4(m_Color), "Ala ma kota");
		ImGui::Text("App is live for %.1fs", Luha::Application::Get().GetTime());
		ImGui::Text("Delta time: %.1fms", Luha::Application::Get().GetDeltaTime().GetMilliseconds());
		ImGui::Text("Total frames: %d", Luha::Application::Get().GetWindow().GetFrameCount());
		ImGui::InputText("Charcter", &m_Character);
		ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Example Image");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImGui::Image(m_Image.GetData(), viewportPanelSize);
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

	}

	void ExampleLayer::OnMenuRender()
	{
		LH_PROFILE_FUNCTION();

		if (ImGui::BeginMenu("Example"))
		{
			ImGui::Text("JD2000");

			ImGui::EndMenu();
		}
	}

	void ExampleLayer::OnEvent(Luha::Event& event)
	{
		if (event.GetEventType() == Luha::EventType::WindowResize)
		{
			Luha::ScopedTimer timer("Window resized", Luha::TimeUnit::ms);

			auto ev = (Luha::WindowResizeEvent&)event;
			LH_INFO("Window Resized: ({0}, {1})", ev.GetWidth(), ev.GetHeight());
		}
	}

	void ExampleLayer::Serialize()
	{
#ifdef LH_SERIALIZING

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Character" << YAML::Value << m_Character;
		out << YAML::Key << "Color" << YAML::Value << m_Color;
		out << YAML::EndMap;

		std::string filePath = "data/Example.Luha";
		std::filesystem::path fs_path = std::filesystem::path(filePath).parent_path();
		if (!std::filesystem::exists(fs_path)) {
			if (!std::filesystem::create_directories(fs_path)) {
				LH_ASSERT(false, "Error creating directory: {0}", (char*)fs_path.c_str());
			}
		}

		std::ofstream fout(filePath);
		if (fout.good())
		{
			fout << out.c_str();
			LH_INFO("Example data saved successfully");
		}
		else
		{
			LH_ERROR("Error saving example data");
		}
#endif
	}

	void ExampleLayer::Deserialize()
	{
#ifdef LH_SERIALIZING

		try {
			std::ifstream stream("data/Example.Luha");
			std::stringstream strStream;
			strStream << stream.rdbuf();
			YAML::Node doc = YAML::Load(strStream.str());

			std::string name = doc["Character"].as<std::string>();
			glm::vec4 color = doc["Color"].as<glm::vec4>();
			
			LH_INFO("Example data loaded successfully");
			m_Character = name;
			m_Color = color;
		}
		catch (...)
		{
			LH_ERROR("Cannot load example data");
		}

#endif
	}
}