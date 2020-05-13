#pragma once



class RenderingWindow
{

public: RenderingWindow( void );
public: virtual ~RenderingWindow( void );

public: virtual void			initialize( void ) noexcept = 0;
public: virtual void			shutdown( void ) noexcept = 0;

public: HWND					getHandle( void ) const noexcept;

protected: void					updateWindowState( void ) noexcept;

protected: HWND					_hwnd;

protected: std::wstring			_caption;
protected: int					_width;
protected: int					_height;
protected: int					_left;
protected: int					_top;

protected: DWORD				_style;
};