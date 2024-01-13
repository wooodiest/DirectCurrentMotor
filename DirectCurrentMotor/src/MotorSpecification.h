#pragma once

namespace DCM {

	struct MotorSpecification
	{
		float Current = 240.0f;
		float MagneticField = 0.001f;
		float FrameSide_A = 1.0f;
		float FrameSide_B = 2.0f;
		float Alpha = 1.5707963268; // radians
		float Mass = 1.0f;
		float AngularVelocity = 1.0f;
		int   NumberOfWires = 1;
	};
}