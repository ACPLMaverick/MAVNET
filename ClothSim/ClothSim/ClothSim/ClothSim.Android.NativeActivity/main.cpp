#include "Common.h"
#include "System.h"

/**
* This is the main entry point of a native application that is using
* android_native_app_glue.  It runs in its own thread, with its own
* event loop for receiving input events and doing other things.
*/

#ifdef PLATFORM_WINDOWS

#include <iostream>
#include <conio.h>

int main()
{
	//std::cout << "Dupa\n";
	System::GetInstance()->Initialize();

	System::GetInstance()->Run();

	System::GetInstance()->Shutdown();

	LOGI("\nProgram terminated.\n Close this window to exit.\n");

	return 0;
}

#else

void android_main(android_app* state)
{
	int error;

	error = System::GetInstance()->InitAndroid(state);

	// wait for app to init
	while (!System::GetInstance()->GetRunning());

	System::GetInstance()->Run();

	System::DestroyInstance();

	LOGI("\nProgram terminated successfully.\n");
}
#endif