#include "DirectCurrentMotor.h"

#include <imgui.h>
#include <implot.h>

namespace DCM {

	static float  drag_speed          = 1.0f;
	static double plot_visible_time   = 5.0;
	static ImVec4 output_color        = { 1.0f, 0.0f, 1.0f, 1.0f };

	static float min_current          =  0.000001f;
	static float min_magnetic_field   =  0.000001f;
	static float min_alpha            = -180.0f;
	static float min_angular_velocity =  0.000001f;
	static float min_mass             =  0.000001f;
	static float min_frame_side       =  0.000001f;
	static int   min_number_of_wires  =  1;
	static float min_delta_time       =  0.001f;

	static float max_current          =  1000000.0f;
	static float max_magnetic_field   =  1000000.0f;
	static float max_alpha            =  180.0f; 
	static float max_angular_velocity =  1000000.0f;
	static float max_mass             =  1000000.0f;
	static float max_frame_side       =  1000000.0f;
	static int   max_number_of_wires  =  1000;
	static float max_delta_time       =  0.016f;

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

		UpdateEngineFirstFrame();
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

		// Main calculation windows
		ImGui::Begin("   Torque   ",            nullptr);
		if (ImGui::IsWindowFocused()) m_LastFocused = DCM_Windows::Torque;
		Display_Torque();
		ImGui::End();

		ImGui::Begin("   Moment of inertia   ", nullptr);
		if (ImGui::IsWindowFocused()) m_LastFocused = DCM_Windows::Inertia; 
		Display_Inertia();
		ImGui::End();

		ImGui::Begin("   Engine simulation   ", nullptr);
		if (ImGui::IsWindowFocused()) m_LastFocused = DCM_Windows::Engine;
		Display_Engine();
		ImGui::End();

		// Simulation window
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("   Simulation   ");
		switch (m_LastFocused)
		{	
			case DCM::DCM_Windows::Torque:  m_Simulation.SetData(m_TorqueSpec,        m_TorqueCamera,  m_TorqueSpec.Alpha);       break;
			case DCM::DCM_Windows::Inertia: m_Simulation.SetData(m_InertiaSpec,       m_InertiaCamera, m_InertiaSpec.Alpha);      break;
			case DCM::DCM_Windows::Engine:  m_Simulation.SetData(m_EngineCurrentSpec, m_EngineCamera,  m_EngineSimulationAlpha);  break;
		}
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		m_Simulation.   SetViewportSize(viewportSize.x, viewportSize.y);
		m_TorqueCamera. SetViewportSize(viewportSize.x, viewportSize.y);
		m_InertiaCamera.SetViewportSize(viewportSize.x, viewportSize.y);
		m_EngineCamera. SetViewportSize(viewportSize.x, viewportSize.y);

		m_SimulationFocused = ImGui::IsWindowFocused();
		m_SimulationHovered = ImGui::IsWindowHovered();

		m_Simulation.GetCamera()->OnUpdate(Luha::Application::Get().GetDeltaTime(), m_SimulationFocused);
		m_Simulation.             OnUpdate(Luha::Application::Get().GetDeltaTime());

		ImGui::Image((void*)m_Simulation.GetData(), viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		
		ImGui::End();
		ImGui::PopStyleVar();

		// Other
		ImGui::Begin("   Options   ");
		m_Simulation.RenderOptions();
		ImGui::End();
	}

	void DirectCurrentMotor::OnMenuRender()
	{
		LH_PROFILE_FUNCTION();

	}

	void DirectCurrentMotor::OnEvent(Luha::Event& event)
	{
		if(m_SimulationHovered)
			m_Simulation.OnEvent(event);
	}

	void DirectCurrentMotor::Display_Torque()
	{
		// Get data
		ImGui::SeparatorText("Data:");
		ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp; 
		ImGui::DragFloat("Current##Torque",        &m_TorqueSpec.Current,       drag_speed, min_current,         max_current,         "%.6f", flags);
		ImGui::DragFloat("Magnetic field##Torque", &m_TorqueSpec.MagneticField, drag_speed, min_magnetic_field,  max_magnetic_field,  "%.6f", flags);
		ImGui::DragFloat("Frame side A##Torque",   &m_TorqueSpec.FrameSide_A,   drag_speed, min_frame_side,      max_frame_side,      "%.6f", flags);
		ImGui::DragFloat("Frame side B##Torque",   &m_TorqueSpec.FrameSide_B,   drag_speed, min_frame_side,      max_frame_side,      "%.6f", flags);
		float alphaInDegrees = Utils::RadToDeg(m_TorqueSpec.Alpha);
		ImGui::DragFloat("Alpha##Torque",          &alphaInDegrees,             drag_speed, min_alpha,           max_alpha,           "%.6f", flags);
		m_TorqueSpec.Alpha = Utils::DegToRad(alphaInDegrees); 
		ImGui::DragInt("Number of wires",          &m_TorqueSpec.NumberOfWires, drag_speed, min_number_of_wires, max_number_of_wires, "%d",   flags);

		// Calculate
		m_TorqueTorque = 
			m_TorqueSpec.Current     * m_TorqueSpec.MagneticField * m_TorqueSpec.FrameSide_A *
			m_TorqueSpec.FrameSide_B * m_TorqueSpec.NumberOfWires * std::sin(m_TorqueSpec.Alpha);

		// Output
		ImGui::SeparatorText("Output:");
		ImGui::TextColored(output_color, "Torque = %.6f", m_TorqueTorque);

	}

	void DirectCurrentMotor::Display_Inertia()
	{
		// Get data
		ImGui::SeparatorText("Data:");
		ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;
		ImGui::DragFloat("Frame side A##Inertia", &m_InertiaSpec.FrameSide_A,   drag_speed, min_frame_side, max_frame_side, "%.6f", flags);
		ImGui::DragFloat("Frame side B##Inertia", &m_InertiaSpec.FrameSide_B,   drag_speed, min_frame_side, max_frame_side, "%.6f", flags);
		ImGui::DragFloat("Mass##Inertia",         &m_InertiaSpec.Mass,          drag_speed, min_mass,       max_mass,       "%.6f", flags);
		// Calculate
		m_InertiaInertia = 
			(m_InertiaSpec.FrameSide_A * m_InertiaSpec.FrameSide_A * m_InertiaSpec.Mass) /
			(m_InertiaSpec.FrameSide_A + m_InertiaSpec.FrameSide_B) * 
			(m_InertiaSpec.FrameSide_B / 4.0f + 2.0f / 3.0f * m_InertiaSpec.FrameSide_A);

		// Output
		ImGui::SeparatorText("Output:");
		ImGui::TextColored(output_color, "Moment of inertia = %.6f", m_InertiaInertia);
	}

	void DirectCurrentMotor::Display_Engine()
	{
		// Get data
		ImGui::SeparatorText("Data:");
		ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp; 
		ImGui::DragFloat("Current##Engine",           &m_EngineSpec.Current,         drag_speed, min_current,          max_current,          "%.6f", flags);
		ImGui::DragFloat("Magnetic field##Engine",    &m_EngineSpec.MagneticField,   drag_speed, min_magnetic_field,   max_magnetic_field,   "%.6f", flags);
		ImGui::DragFloat("Frame side A##Engine",      &m_EngineSpec.FrameSide_A,     drag_speed, min_frame_side,       max_frame_side,       "%.6f", flags);
		ImGui::DragFloat("Frame side B##Engine",      &m_EngineSpec.FrameSide_B,     drag_speed, min_frame_side,       max_frame_side,       "%.6f", flags);
		ImGui::DragFloat("Mass##Engine",              &m_EngineSpec.Mass,            drag_speed, min_mass,             max_mass,             "%.6f", flags);
		float alphaInDegrees = Utils::RadToDeg(m_EngineSpec.Alpha);
		ImGui::DragFloat("Alpha##Engine",             &alphaInDegrees,               drag_speed, min_alpha,            max_alpha,            "%.6f", flags);
		ImGui::DragFloat("Angular velocity##Engine",  &m_EngineSpec.AngularVelocity, drag_speed, min_angular_velocity, max_angular_velocity, "%.6f", flags);
		m_EngineSpec.Alpha = Utils::DegToRad(alphaInDegrees);
		ImGui::DragInt("Number of wires##Engine",     &m_EngineSpec.NumberOfWires,   drag_speed, min_number_of_wires,  max_number_of_wires,  "%d",   flags);
		ImGui::DragFloat("Delta time##Engine",        &m_EngineDeltaTime,            drag_speed / 5000.0f, min_delta_time, max_delta_time,       "%.6f", flags);

		// Display menu
		ImGui::SeparatorText("Simulation");
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (m_EnginePaused)
		{
			const float buttonSpacing = 5.0f;
			ImVec2 buttonSize = { (viewportSize.x - buttonSpacing) / 2, 0.0f };
			
			const char* buttonName = m_EngineCurrentLiveTime == 0.0f ? "   Start   " : "   Resume   ";
			if (ImGui::Button(buttonName, buttonSize))
				m_EnginePaused = false;
			
			ImGui::SameLine(0.0f, buttonSpacing);
			if (ImGui::Button("  Reset  ", buttonSize))
				m_EngineCurrentLiveTime = 0.0f;
		}
		else
		{
			if (ImGui::Button("  Stop  ", { viewportSize.x, 0.0f }))
				m_EnginePaused = true;		
		}

		if (m_EngineCurrentLiveTime == 0.0f)
		{
			m_EngineAlpha.Erase();
			m_EngineAngularVelocity.Erase();
			m_EngineAngularAcceleration.Erase();
			m_EngineTorque.Erase();

			UpdateEngineFirstFrame();
		}

		// Calculate
		if (!m_EnginePaused)
		{
			int n = (int)(ImGui::GetIO().DeltaTime / m_EngineCurrentDeltaTime); // number of elements that will be calculated in single frame
			if (n <= 0) n = 1;

			for (int i = 0; i < n; i++)
			{
				m_EngineCurrentLiveTime += m_EngineCurrentDeltaTime;

				float angularVelocity = m_EnginePrevAngularVelocity + m_EngineCurrentDeltaTime * m_Engine_KI * std::sin(m_EnginePrevAlpha);
				m_EngineAngularVelocity.AddPoint(m_EngineCurrentLiveTime, m_EnginePrevAlpha);
				m_EnginePrevAngularVelocity = angularVelocity;

				float alpha = m_EnginePrevAlpha + m_EngineCurrentDeltaTime * angularVelocity;
				m_EngineSimulationAlpha = alpha;
				m_EngineAlpha.AddPoint(m_EngineCurrentLiveTime, alpha);
				m_EnginePrevAlpha = alpha;

				float torque = m_Engine_K * std::sin(alpha);
				m_EngineTorque.AddPoint(m_EngineCurrentLiveTime, torque);

				m_EngineAngularAcceleration.AddPoint(m_EngineCurrentLiveTime, torque / m_EngineInertia);			
			}
		}
		// Display output
		ImGui::SeparatorText("Plots");

		if (ImPlot::BeginAlignedPlots("Plots:Live"))
		{
			ImPlotFlags plotFlags = ImPlotFlags_NoLegend;
			ImPlotAxisFlags axisFlags = ImPlotAxisFlags_AutoFit;
			ImPlot::PushStyleColor(ImPlotCol_Line, output_color);

			if (ImPlot::BeginPlot("##Plot:1", "Time", "Alpha", {-1.0f, 0.0f}, plotFlags))
			{
				ImPlot::SetupAxes("Time", "Alpha", 0, axisFlags);
				ImPlot::SetupAxisLimits(ImAxis_X1, (double)m_EngineCurrentLiveTime - plot_visible_time, m_EngineCurrentLiveTime, m_EnginePaused ? ImGuiCond_Once : ImGuiCond_Always);
				if (m_EngineAlpha.Data.size() > 0)
					ImPlot::PlotLine("Alpha", &m_EngineAlpha.Data[0].x, &m_EngineAlpha.Data[0].y, m_EngineAlpha.Data.size(), 0, m_EngineAlpha.Offset, 2 * sizeof(float));

				ImPlot::EndPlot();
			}

			if (ImPlot::BeginPlot("##Plot:2", "Time", "Angular velocity", {-1.0f, 0.0f}, plotFlags))
			{
				ImPlot::SetupAxes("Time", "Angular velocity", 0, axisFlags);
				ImPlot::SetupAxisLimits(ImAxis_X1, (double)m_EngineCurrentLiveTime - plot_visible_time, m_EngineCurrentLiveTime, m_EnginePaused ? ImGuiCond_Once : ImGuiCond_Always);
				if (m_EngineAlpha.Data.size() > 0)
					ImPlot::PlotLine("Angular velocity", &m_EngineAngularVelocity.Data[0].x, &m_EngineAngularVelocity.Data[0].y, m_EngineAngularVelocity.Data.size(), 0, m_EngineAngularVelocity.Offset, 2 * sizeof(float));

				ImPlot::EndPlot();
			}

			if (ImPlot::BeginPlot("##Plot:3", "Time", "Angular acceleration", { -1.0f, 0.0f }, plotFlags))
			{
				ImPlot::SetupAxes("Time", "Angular acceleration", 0, axisFlags);
				ImPlot::SetupAxisLimits(ImAxis_X1, (double)m_EngineCurrentLiveTime - plot_visible_time, m_EngineCurrentLiveTime, m_EnginePaused ? ImGuiCond_Once : ImGuiCond_Always);
				if (m_EngineAlpha.Data.size() > 0)
					ImPlot::PlotLine("Angular acceleration", &m_EngineAngularAcceleration.Data[0].x, &m_EngineAngularAcceleration.Data[0].y, m_EngineAngularAcceleration.Data.size(), 0, m_EngineAngularAcceleration.Offset, 2 * sizeof(float));

				ImPlot::EndPlot();
			}

			if (ImPlot::BeginPlot("##Plot:4", "Time", "Torque", { -1.0f, 0.0f }, plotFlags))
			{
				ImPlot::SetupAxes("Time", "Torque", 0, axisFlags);
				ImPlot::SetupAxisLimits(ImAxis_X1, (double)m_EngineCurrentLiveTime - plot_visible_time, m_EngineCurrentLiveTime, m_EnginePaused ? ImGuiCond_Once : ImGuiCond_Always);
				if (m_EngineAlpha.Data.size() > 0)
					ImPlot::PlotLine("Torque", &m_EngineTorque.Data[0].x, &m_EngineTorque.Data[0].y, m_EngineTorque.Data.size(), 0, m_EngineTorque.Offset, 2 * sizeof(float));

				ImPlot::EndPlot();
			}

			ImPlot::PopStyleColor();
			ImPlot::EndAlignedPlots();
		}
	}

	void DirectCurrentMotor::UpdateEngineFirstFrame()
	{
		m_EngineCurrentSpec = m_EngineSpec;
		m_EngineCurrentDeltaTime = m_EngineDeltaTime;

		m_EngineInertia =
			(m_EngineCurrentSpec.FrameSide_A * m_EngineCurrentSpec.FrameSide_A * m_EngineCurrentSpec.Mass) /
			(m_EngineCurrentSpec.FrameSide_A + m_EngineCurrentSpec.FrameSide_B) *
			(m_EngineCurrentSpec.FrameSide_B / 4.0f + 2.0f / 3.0f * m_EngineCurrentSpec.FrameSide_A);

		m_Engine_K = m_EngineCurrentSpec.Current * m_EngineCurrentSpec.MagneticField * m_EngineCurrentSpec.FrameSide_A
			* m_EngineCurrentSpec.FrameSide_B * m_EngineCurrentSpec.NumberOfWires;

		m_Engine_KI = m_Engine_K / m_EngineInertia;

		m_EngineAngularVelocity.AddPoint(m_EngineCurrentLiveTime, m_EngineCurrentSpec.AngularVelocity);
		m_EnginePrevAngularVelocity = m_EngineCurrentSpec.AngularVelocity;

		m_EngineAlpha.AddPoint(m_EngineCurrentLiveTime, m_EngineCurrentSpec.Alpha);
		m_EngineSimulationAlpha = m_EngineCurrentSpec.Alpha;
		m_EnginePrevAlpha = m_EngineCurrentSpec.Alpha;

		float torque = m_Engine_K * std::sin(m_EnginePrevAlpha);
		m_EngineTorque.AddPoint(m_EngineCurrentLiveTime, torque);

		m_EngineAngularAcceleration.AddPoint(m_EngineCurrentLiveTime, torque / m_EngineInertia);
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