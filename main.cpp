#include "pch.h"

#include "RenderingWindowManager.h"

int APIENTRY wWinMain( _In_ HINSTANCE hInstance, 
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPWSTR lpCmdLine, 
					   _In_ int mCmdShow )
{
	RenderingWindowManager manager;

	manager.initialize();

	MSG msg;

	while( true )
	{
		while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			if ( msg.message == WM_QUIT )
			{
				return true;
			}

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	manager.shutdown();

	return 0;
}