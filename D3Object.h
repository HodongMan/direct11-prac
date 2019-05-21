#pragma once

#include "pch.h"

class D3Object
{
public:
	D3Object( void );
	D3Object( const D3Object& rhs );
	virtual ~D3Object( void );

	bool initialize( int, int, bool, HWND, bool, float, float ) noexcept;
	bool shutDown( void ) noexcept;

	void beginScene( float, float, float, float ) noexcept;
	void endScene( void ) noexcept;

	ID3D11Device* getDevice( void ) const noexcept;
	ID3D11DeviceContext* getDeviceContext( void ) const noexcept;

	void getProjectMatrix( DirectX::XMMATRIX& ) noexcept;
	void getWorldMatrix( DirectX::XMMATRIX& ) noexcept;
	void getOrthoMatrix( DirectX::XMMATRIX& ) noexcept;

	void getVideoCardInfo( char*, int ) noexcept;

private:
	bool				_vsyncEnable;
	int					_videoCardMemory;
	char				_videoCardDescription[128];
	IDXGISwapChain* 





};