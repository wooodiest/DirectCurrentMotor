#pragma once

#include <Luha.h>

#include <glm/glm.hpp>

namespace DCM {

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

	};

}