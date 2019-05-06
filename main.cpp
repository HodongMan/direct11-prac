#include "SystemManager.h"

int APIENTRY wWinMain( _In_ HINSTANCE hInstance, 
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPWSTR lpCmdLine, 
					   _In_ int mCmdShow )
{
	SystemManager* systemManager = new SystemManager();
	if ( nullptr == systemManager )
	{
		return -1;
	}

	if ( true == systemManager->initialize() )
	{
		systemManager->run();
	}

	systemManager->shutDown( true );
	delete systemManager;
	systemManager = nullptr;

	return 0;
}