#include "pch.h"


#include "RenderingWindowManager.h"

LRESULT InternalWindowProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	switch ( msg )
	{

	case WM_CREATE:
		{
			return 0;
		}
		break;
	case WM_PAINT:
		{

		}
		break;
	case WM_CLOSE:
		{

		}
		break;
	case WM_DESTROY:
		{
			::PostQuitMessage( 0 );
		
			return 0;
		} 
		break;
	default:
		{
			return DefWindowProc( hwnd, msg, wparam, lparam );
		}
		break;
	}

	return DefWindowProc( hwnd, msg, wparam, lparam );
}

RenderingWindowManager::RenderingWindowManager( void )
	: RenderingWindow()
{

}

RenderingWindowManager::~RenderingWindowManager( void )
{
	shutdown();
}

void RenderingWindowManager::initialize( void ) noexcept
{
	WNDCLASSEX wc;

	ZeroMemory( &wc, sizeof( wc ) );
	wc.cbSize						= sizeof( WNDCLASSEX );
	wc.style						= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc					= InternalWindowProc;
	wc.cbClsExtra					= 0;
	wc.cbWndExtra					= 0;
	wc.hInstance					= 0;
	wc.hIcon						= LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor						= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground				= ( HBRUSH )GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName					= NULL;
	wc.lpszClassName				= L"HodongDX11";
	wc.hIconSm						= LoadIcon( NULL, IDI_APPLICATION );

	::RegisterClassEx( &wc );

	RECT rect;
	rect.top						= 0;
	rect.left						= 0;
	rect.right						= _width;
	rect.bottom						= _height;

	::AdjustWindowRectEx( &rect, _style, false, 0 );

	const long lwidth				= rect.right - rect.left;
	const long lheight				= rect.bottom - rect.top;

	const long lleft				= static_cast<long>( _left );
	const long ltop					= static_cast<long>( _top );


	_hwnd = ::CreateWindowEx( NULL,
							  wc.lpszClassName,
							  _caption.c_str(),
							  _style,
							  lleft,
							  ltop,
							  lwidth,
							  lheight,
							  NULL,
							  NULL,
							  NULL,
							  NULL );

	if ( _hwnd )
	{
		ShowWindow( _hwnd, SW_SHOWNORMAL );
		UpdateWindow( _hwnd );
	}
}

void RenderingWindowManager::shutdown( void ) noexcept
{
	if ( _hwnd )
	{
		::DestroyWindow( _hwnd );
	}
}
