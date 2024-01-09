#pragma once

#include <Luha.h>

#include <glm/glm.hpp>

namespace ExampleApp {

	class ExampleLayer : public Luha::Layer
	{
	public:
		ExampleLayer();
		virtual ~ExampleLayer();

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
		std::string m_Character = "";
		glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Luha::Image m_Image;

	};

}