#include "pch.h"

#include "RenderingWindow.h"


RenderingWindow::RenderingWindow( void )
	: _hwnd{ 0 }
	, _caption{ L"" }
	, _width{ 640 }
	, _height{ 480 }
	, _left{ 100 }
	, _top{ 100 }
	, _style{ WS_OVERLAPPEDWINDOW | WS_VISIBLE }
{

}

RenderingWindow::~RenderingWindow( void )
{
}

HWND RenderingWindow::getHandle( void ) const noexcept
{
	return _hwnd;
}

void RenderingWindow::updateWindowState( void ) noexcept
{
	if ( 0 != _hwnd )
	{
		RECT ClientRect;
		ClientRect.left = 0;
		ClientRect.top = 0;
		ClientRect.right = _width;
		ClientRect.bottom = _height;

		RECT WindowRect = ClientRect;
		AdjustWindowRect( &WindowRect, _style, FALSE );

		int deltaX = ( WindowRect.right - ClientRect.right ) / 2;
		int deltaY = ( WindowRect.bottom - ClientRect.bottom ) / 2;

		MoveWindow( _hwnd, _left - deltaX, _top - deltaY, _width + deltaX * 2, _height + deltaY * 2, true );
	}
}
