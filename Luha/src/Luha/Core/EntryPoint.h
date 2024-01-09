#pragma once

extern Luha::Application* Luha::CreateApplication(int argc, char** argv);

namespace Luha {

	int Main(int argc, char** argv)
	{
		LH_PROFILE_BEGIN_SESSION("Startup", "Data/Profile/Luha-Profile-Startup.json");
			Luha::Application* app = Luha::CreateApplication(argc, argv);
		LH_PROFILE_END_SESSION();

		LH_PROFILE_BEGIN_SESSION("Runtime", "Data/Profile/Luha-Profile-Runtime.json");
			app->Run();
		LH_PROFILE_END_SESSION();

		LH_PROFILE_BEGIN_SESSION("Shutdown", "Data/Profile/Luha-Profile-Shutdown.json");
			delete app;
		LH_PROFILE_END_SESSION();

		return 0;
	}

}

// Application definition
#ifdef LH_PLATFORM_WINDOWS

	#if defined(LH_DIST) && LH_CONSOLE_IN_DISTRIBUTION_BUILD == 0
		// To open without console:
		// In Visual studio 2022 go to Project:Properties -> Linker -> System -> change SubSystem to Windows (/SUBSYSTEM:WINDOWS)
		// To undo changes change SubSystem to Console (/SUBSYSTEM:CONSOLE)
		
		#include <Windows.h>
		int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
		{
			return Luha::Main(__argc, __argv);
		}
		
	#else
		int main(int argc, char** argv)
		{
			return Luha::Main(argc, argv);
		}
	#endif


#endif // LH_PLATFORM_WINDOWS

