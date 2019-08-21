#pragma once

#include "pch.h"

class D3Object
{
public:
	D3Object( void );
	D3Object( const D3Object& rhs );
	virtual ~D3Object( void );

	bool initialize( int screenWidth, int screenHeight, bool vsync, HWND hWnd, float screenDepth, float screenNear ) noexcept;
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