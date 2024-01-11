#pragma once

#include <Luha.h>

#include <cmath>
#include <memory>

#include "Engine.h"
#include "MotorSpecification.h"
#include "Engine.h"
#include "Camera.h"

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

		// To save and load data safely, decide when to use it. Luha only calls Serialize() when saved (Ctrl+S)
		virtual void Serialize() override;
		virtual void Deserialize() override;

	private:
		void Display_Torque();
		void Display_Inertia();
		void Display_Engine();

	private:
		Engine m_Engine;
		bool m_EngineActive = false;
		// Torque
		MotorSpecification m_TorqueSpec;
		Camera m_TorqueCamera;
		float m_TorqueTorque;

		// Inertia
		MotorSpecification m_InertiaSpec;
		Camera m_InertiaCamera;
		float m_InertiaInertia;

		// Engine
		MotorSpecification m_EngineSpec;
		Camera m_EngineCamera;
		MotorSpecification m_EngineCurrentSpec;
		float m_EngineDeltaTime        = 0.01f;
		float m_EngineCurrentDeltaTime = 0.01f;
		float m_EngineCurrentLiveTime  = 0.0f;
		float m_EngineInertia;
		float m_Engine_K, m_Engine_KI;
		float m_EnginePrevAngularVelocity, m_EnginePrevAlpha;
		bool  m_EnginePaused           = true;
		Utils::ScrollingBuffer m_EngineAlpha               { 5000 };
		Utils::ScrollingBuffer m_EngineAngularVelocity     { 5000 };
		Utils::ScrollingBuffer m_EngineAngularAcceleration { 5000 };
		Utils::ScrollingBuffer m_EngineTorque              { 5000 };

		DCM_Windows m_LastFocused = DCM_Windows::Simulation;

	};

}