#include "pch.h"
#include "SystemManager.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "TypeDefined.h"

SystemManager::SystemManager( void )
{
	_input		= nullptr;
	_graphics	= nullptr;
	_isValid	= false;
}

SystemManager::SystemManager( const SystemManager& rhs )
{
	_input		= nullptr;
	_graphics	= nullptr;
	_isValid	= false;
}

SystemManager::~SystemManager( void )
{
	shutDown( true );
}

bool SystemManager::initialize( void ) noexcept
{
	int screenWidth		= 0;
	int screenHeight	= 0;
	
	initializeWindows( screenWidth, screenHeight );

	_input		= new InputManager();
	if ( nullptr == _input )
	{
		return false;
	}

	_input->initialize();

	_graphics	= new GraphicsManager();
	if ( nullptr == _graphics )
	{
		shutDown( false );
		
		return false;
	}
	
	if ( false ==  _graphics->initialize( screenWidth, screenHeight, _hwnd ) )
	{
		shutDown( false );

		return false;
	}

	_isValid = true;

	return true;
}


bool SystemManager::shutDown( const bool isWindowShutdown ) noexcept
{
	if ( nullptr != _graphics )
	{
		_graphics->shutDown();
		delete _graphics;
		_graphics = nullptr;

	}

	if ( nullptr != _input )
	{
		delete _input;
		_input = nullptr;
	}

	if ( true == isWindowShutdown )
	{
		shutDownWindows();
	}

	return true;
}

void SystemManager::run( void ) noexcept
{
	MSG message;
	ZeroMemory( &message, sizeof( message ) );

	while( true )
	{
		if ( PeekMessage( &message, nullptr, 0, 0, PM_REMOVE ) )
		{
			if ( WM_QUIT == message.message )
			{
				break;
			}

			TranslateMessage( &message );
			DispatchMessage( &message );
		}
		else
		{
			if ( false == frame() )
			{
				break;
			}
		}
	}
}

bool SystemManager::isValid( void ) const noexcept
{
	return ( ( true == _isValid ) && ( nullptr != _input ) && ( nullptr != _graphics ) );
}

bool SystemManager::frame( void ) noexcept
{
	if ( true == _input->isKeyDown( VK_ESCAPE ) )
	{
		return false;
	}

	return _graphics->frame();
}

LRESULT CALLBACK SystemManager::MessageHandler( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool isHandled ) noexcept
{
	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			_input->keyDown( static_cast<unsigned int>( wParam ) );

			return 0;
		}
		break;
	case WM_KEYUP:
		{
			_input->keyUp( static_cast<unsigned int>( wParam ) );
		}
		break;
	default:
		{
			return DefWindowProc( hwnd, uMsg, wParam, lParam );
		}
		break;
	}

	return 0;
}

void SystemManager::initializeWindows( int& screenWidth, int& screenHeight ) noexcept
{
	applicationHandle = this;

	_hInstance = GetModuleHandle( nullptr );

	_applicationName = L"Hodong Dx11";

	WNDCLASSEX wc;

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= _hInstance;
	wc.hIcon			= LoadIcon( nullptr, IDI_WINLOGO );
	wc.hIconSm			= wc.hIcon;
	wc.hCursor			= LoadCursor( nullptr, IDC_ARROW );
	wc.hbrBackground	= static_cast<HBRUSH>( GetStockObject( BLACK_BRUSH ) );
	wc.lpszMenuName		= nullptr;
	wc.lpszClassName	= nullptr;
	wc.cbSize			= sizeof( WNDCLASSEX );

	RegisterClassEx( &wc );

	screenWidth			= GetSystemMetrics( SM_CXSCREEN );
	screenHeight		= GetSystemMetrics( SM_CYSCREEN );

	int positionX		= 0;
	int positionY		= 0;

	if ( true == FULL_SCREEN )
	{
		DEVMODE	dmScreenSettings;
		memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );

		dmScreenSettings.dmSize			= sizeof( dmScreenSettings );
		dmScreenSettings.dmPelsWidth	= static_cast<unsigned long>( screenWidth );
		dmScreenSettings.dmPelsHeight	= static_cast<unsigned long>( screenHeight );
		dmScreenSettings.dmBitsPerPel	= 32;
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN );
	}
	else
	{
		screenWidth		= 1024;
		screenHeight	= 768;

		positionX		= ( GetSystemMetrics( SM_CXSCREEN ) - screenWidth ) / 2;
		positionY		= ( GetSystemMetrics( SM_CYSCREEN ) - screenHeight ) / 2;
	}

	_hwnd = CreateWindowEx( WS_EX_APPWINDOW, _applicationName, _applicationName,
							WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							positionX, positionY, screenWidth, screenHeight, nullptr, nullptr, _hInstance, nullptr);

	ShowWindow( _hwnd, SW_SHOW );
	SetForegroundWindow( _hwnd );
	SetFocus( _hwnd );
}

void SystemManager::shutDownWindows( void ) noexcept
{
	if ( true == FULL_SCREEN )
	{
		ChangeDisplaySettings( nullptr, 0 );
	}

	DestroyWindow( _hwnd );
	_hwnd = nullptr;

	UnregisterClass( _applicationName, _hInstance );
	_hInstance = nullptr;

	applicationHandle = nullptr;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;
		}
		break;
	case WM_CLOSE:
		{
			PostQuitMessage( 0 );
			return 0;
		}
		break;
	default:
		{
			if ( nullptr == applicationHandle )
			{
				return 0;
			}

			return applicationHandle->MessageHandler( hwnd, uMsg, wParam, lParam , true );
		}
		break;
	}
}