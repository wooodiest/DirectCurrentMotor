#include "DirectCurrentMotor.h"

#include <imgui.h>
#include <implot.h>

#include <filesystem>
#include <yaml-cpp/yaml.h>

#include <glm/gtc/type_ptr.hpp>

namespace DCM {

	DirectCurrentMotor::DirectCurrentMotor()
	{
		LH_PROFILE_FUNCTION();
	}

	DirectCurrentMotor::~DirectCurrentMotor()
	{
		LH_PROFILE_FUNCTION();

		Serialize();
	}

	void DirectCurrentMotor::OnAttach()
	{
		LH_PROFILE_FUNCTION();

		Deserialize();

	}

	void DirectCurrentMotor::OnDetach()
	{
		LH_PROFILE_FUNCTION();
	}

	void DirectCurrentMotor::OnUpdate(Luha::Timestep ts)
	{
		LH_PROFILE_FUNCTION();

	}

	void DirectCurrentMotor::OnRender()
	{
		LH_PROFILE_FUNCTION();

		ImGui::ShowDemoWindow();
		ImPlot::ShowDemoWindow();
	}

	void DirectCurrentMotor::OnMenuRender()
	{
		LH_PROFILE_FUNCTION();

	}

	void DirectCurrentMotor::OnEvent(Luha::Event& event)
	{

	}

	void DirectCurrentMotor::Serialize()
	{
#ifdef LH_SERIALIZING

#endif
	}

	void DirectCurrentMotor::Deserialize()
	{
#ifdef LH_SERIALIZING

#endif
	}
}