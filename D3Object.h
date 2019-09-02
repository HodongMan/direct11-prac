#pragma once

#include "pch.h"

class D3Object
{
public:
	D3Object( void );
	D3Object( const D3Object& rhs );
	virtual ~D3Object( void );

	bool initialize( int screenWidth, int screenHeight, bool vsync, HWND hWnd, bool fullScreen, float screenDepth, float screenNear ) noexcept;
	bool shutDown( void ) noexcept;

	void beginScene( const float red, const float green, const float blue, const float alpha ) noexcept;
	void endScene( void ) noexcept;

	const ID3D11Device* getDevice( void ) const noexcept;
	const ID3D11DeviceContext* getDeviceContext( void ) const noexcept;

	void getProjectionMatrix( _Out_ DirectX::XMMATRIX& projectionMatrix ) noexcept;
	void getWorldMatrix( _Out_ DirectX::XMMATRIX& worldMatrix ) noexcept;
	void getOrthoMatrix( _Out_ DirectX::XMMATRIX& orthoMatrix ) noexcept;

	void getVideoCardInfo( char* cardName, _Out_ int& memory ) noexcept;

private:
	bool						_vsyncEnable;
	int							_videoCardMemory;
	char						_videoCardDescription[128];
	IDXGISwapChain*				_swapChain;
	ID3D11Device*				_device;
	ID3D11DeviceContext*		_deviceContext;
	ID3D11RenderTargetView*		_renderTargetView;
	ID3D11Texture2D*			_depthStencilBuffer;
	ID3D11DepthStencilState*	_depthStencilState;
	ID3D11DepthStencilView*		_depthStencilView;
	ID3D11RasterizerState*		_rasterState;
	DirectX::XMMATRIX			_projectionMatrix;
	DirectX::XMMATRIX			_worldMatrix;
	DirectX::XMMATRIX			_orthoMatrix;
};