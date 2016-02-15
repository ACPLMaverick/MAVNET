#include "Common.h"
#include "System.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <conio.h>

int main()
{
	int error;
	
	error = System::GetInstance()->Initialize();

	if (error != CS_ERR_NONE)
	{
#ifdef _DEBUG
		printf("\nSystem.Initialize exited with error code %d\n", error);
		getch();
#endif
		return error;
	}

	error = System::GetInstance()->Run();

	if (error != CS_ERR_NONE)
	{
#ifdef _DEBUG
		printf("\nSystem.Run exited with error code %d\n", error);
		getch();
#endif
		return error;
	}

	error = System::GetInstance()->Shutdown();

	if (error != CS_ERR_NONE)
	{
#ifdef _DEBUG
		printf("\nSystem.Shutdown exited with error code %d\n", error);
		getch();
#endif
		return error;
	}

	System::DestroyInstance();

	printf("\nProgram terminated successfully.\n");
	getch();
	return CS_ERR_NONE;
}

