#pragma once

#include <glm/glm.hpp>

namespace DCM {

	struct MotorSpecification
	{
		// Actual calculating data
		float Current = 240.0f;
		float MagneticField = 0.001f;
		float FrameSide_A = 1.0f;
		float FrameSide_B = 2.0f;
		float Alpha = glm::radians(90.0f); // radians
		float Mass = 1.0f;
		float AngularVelocity = 1.0f;
		int   NumberOfWires = 1;
		float DeltaTime = 0.01f;

		// Useful data to program
		float Inertia = 0.0f;
		float Constance_K = 0.0f;
		float Constance_KI = 0.0f;

		float PrevAlpha = glm::radians(90.0f);;
		float PrevAngularVelocity = 0.0f;
	};
}