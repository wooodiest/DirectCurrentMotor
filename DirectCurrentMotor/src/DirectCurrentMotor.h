#pragma once

#include <Luha.h>

#include <cmath>
#include <memory>

#include "Simulation/Simulation.h"
#include "Simulation/Camera.h"
#include "MotorSpecification.h"

namespace DCM {

	enum class DCM_Windows
	{
		Simulation = 0,
		Torque, Inertia, Engine
	};

	class DirectCurrentMotor : public Luha::Layer
	{
	public:
		DirectCurrentMotor();
		virtual ~DirectCurrentMotor();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Luha::Timestep ts) override;
		virtual void OnRender() override;
		virtual void OnMenuRender() override;
		virtual void OnEvent(Luha::Event& event) override;

		virtual void Serialize() override;
		virtual void Deserialize() override;

	private:
		void Display_Torque();
		void Display_Inertia();
		void Display_Engine();

	private:
		Simulation m_Simulation;
		bool m_SimulationFocused = false;
		bool m_SimulationHovered = false;
		DCM_Windows m_LastFocused = DCM_Windows::Simulation;

		// Torque
		MotorSpecification m_TorqueSpec;
		Camera m_TorqueCamera;
		float m_TorqueTorque;

		// Inertia
		MotorSpecification m_InertiaSpec;
		Camera m_InertiaCamera;
		float m_InertiaInertia;

		// Engine
		void CalculateEngineFirstElement();
		MotorSpecification m_EngineSpec;
		MotorSpecification m_EngineCurrentSpec;
		Camera m_EngineCamera;
		float m_EngineCurrentLiveTime      = 0.0f;
		float m_EngineFixedDeltaTimeHelper = 0.0f;
		bool  m_EnginePaused               = true;
		Utils::ScrollingBuffer m_EngineAlpha               { 5000 };
		Utils::ScrollingBuffer m_EngineAngularVelocity     { 5000 };
		Utils::ScrollingBuffer m_EngineAngularAcceleration { 5000 };
		Utils::ScrollingBuffer m_EngineTorque              { 5000 };

	};

}