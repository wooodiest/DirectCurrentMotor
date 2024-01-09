#pragma once

#ifdef LH_DEBUG
	#define LH_ENABLE_ASSERTS
#endif

#ifdef LH_ENABLE_ASSERTS
	#define LH_ASSERT(x, ...) { if(!(x)) { LH_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define LH_CORE_ASSERT(x, ...) { if(!(x)) { LH_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define LH_ASSERT(x, ...)
	#define LH_CORE_ASSERT(x, ...)
#endif