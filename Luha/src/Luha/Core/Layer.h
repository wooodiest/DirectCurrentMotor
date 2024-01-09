#pragma once

#include "Luha/Events/Event.h"

#include "Luha/Utils/Timestep.h"

namespace Luha {

	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(Timestep ts) {}
		virtual void OnRender() {}
		virtual void OnMenuRender() {}
		virtual void OnEvent(Event& event) {}

		// To save and load data safely, decide when to use it. Luha only calls Serialize() when saved (Ctrl+S)
		virtual void Serialize() {}
		virtual void Deserialize() {}

	};

}
