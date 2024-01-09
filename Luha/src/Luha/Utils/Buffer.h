#pragma once

#include <imgui.h>

namespace Utils {

	struct ScrollingBuffer
	{
		int MaxSize;
		int Offset;
		ImVector<ImVec2> Data;

		ScrollingBuffer(int max_size = 3000);
		void AddPoint(float x, float y);
		void Erase();
	};

}