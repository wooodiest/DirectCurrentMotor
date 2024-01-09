#include "lhpch.h"

#include "Luha/Utils/Random.h"

namespace Luha {

	std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

}