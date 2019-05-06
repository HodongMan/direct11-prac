#pragma once

#include "pch.h"

class InputManager;
class GraphicsManager;


class SystemManager
{
public:
	SystemManager( void );
	SystemManager( const SystemManager& rhs );
	virtual ~SystemManager( void );

	bool				initialize( void ) noexcept;
	bool				shutDown( const bool isWindowShutdown  ) noexcept;
	void				run( void ) noexcept;
	bool				isValid( void ) const noexcept;

	LRESULT CALLBACK	MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool isHandled ) noexcept;

private:

	bool				frame( void ) noexcept;
	void				initializeWindows( _Out_ int& screenWidth, _Out_ int& screenHeight ) noexcept;
	void				shutDownWindows( void ) noexcept;

	LPCWSTR				_applicationName;
	HINSTANCE			_hInstance;
	HWND				_hwnd;

	InputManager*		_input;
	GraphicsManager*	_graphics;
	bool				_isValid;
};

static LRESULT CALLBACK	WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
static SystemManager*	applicationHandle = nullptr;