#include "D3Object.h"

D3Object::D3Object( void )
	: _vsyncEnable( false )
	, _videoCardMemory( 0 )
	, _videoCardDescription{0,}
	, _swapChain( nullptr )
	, _device( nullptr )
	, _deviceContext( nullptr )
	, _renderTargetView( nullptr )
	, _depthStencilBuffer( nullptr )
	, _depthStencilState( nullptr )
	, _depthStencilView( nullptr )
	, _rasterState( nullptr )
{

}

D3Object::D3Object( const D3Object& rhs )
{

}

D3Object::~D3Object( void )
{

}

bool D3Object::initialize( int screenWidth, int screenHeight, bool vsync, HWND hWnd, bool fullScreen, float screenDepth, float screenNear ) noexcept
{
	_vsyncEnable = vsync;

	IDXGIFactory* factory = nullptr;
	if ( FAILED( CreateDXGIFactory( __uuidof( IDXGIFactory ), (void**)( &factory ) ) ) )
	{
		return false;
	}

	IDXGIAdapter* adapter = nullptr;
	if ( FAILED( factory->EnumAdapters( 0, &adapter ) ) )
	{
		return false;
	}

	IDXGIOutput* adapterOutput = nullptr;
	if ( FAILED( adapter->EnumOutputs( 0, &adapterOutput ) ) )
	{
		return false;
	}
	
	unsigned int numModes = 0;
	if ( FAILED( adapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr ) ) )
	{
		return false;
	}

	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	if ( nullptr == displayModeList )
	{
		return false;
	}

	if ( FAILED( adapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList ) ) )
	{
		return false;
	}

	unsigned int numerator		= 0;
	unsigned int denominator	= 0;
	for( unsigned int i = 0; i < numModes; ++i )
	{
		if ( displayModeList[i].Width == static_cast<unsigned int>( screenWidth ) )
		{
			if ( displayModeList[i].Height == static_cast<unsigned int>( screenHeight ) )
			{
				numerator	= displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	if ( FAILED( adapter->GetDesc( &adapterDesc ) ) )
	{
		return false;
	}

	_videoCardMemory = static_cast<int>( adapterDesc.DedicatedVideoMemory / 1024 / 1024 );

	size_t stringLength = 0;
	if ( 0 != wcstombs_s( &stringLength, _videoCardDescription, 128, adapterDesc.Description, 128 ) )
	{
		return false;
	}

	delete[] displayModeList;
	displayModeList = nullptr;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width	= screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if ( true == _vsyncEnable )
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator		= numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator	= denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator		= numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator	= denominator;	
	}

	swapChainDesc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow	= hWnd;

	swapChainDesc.SampleDesc.Count		= 1;
	swapChainDesc.SampleDesc.Quality	= 0;

	if ( true == fullScreen )
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.BufferDesc.ScanlineOrdering	= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling			= DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	UINT createDeviceFlags = 0;	
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	if ( FAILED( D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1,
		 D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, nullptr, &_deviceContext ) ) )
	{
		return false;
	}

	ID3D11Texture2D* backBufferPtr = nullptr;
	if ( FAILED( _swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ) , (LPVOID*)&backBufferPtr ) ) )
	{
		return false;
	}

	if ( FAILED( _device->CreateRenderTargetView( backBufferPtr, nullptr, &_renderTargetView ) ) )
	{
		return false;
	}

	backBufferPtr->Release();
	backBufferPtr = nullptr;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory( &depthBufferDesc, sizeof( depthBufferDesc ) );

	depthBufferDesc.Width		= screenWidth;
	depthBufferDesc.Height		= screenHeight;
	depthBufferDesc.MipLevels	= 1;
	depthBufferDesc.ArraySize	= 1;
	depthBufferDesc.Format		= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count	= 1;
	depthBufferDesc.SampleDesc.Quality	= 0;
	depthBufferDesc.Usage		= D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags	= D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags		= 0;
	depthBufferDesc.MiscFlags	= 0;

	if ( FAILED( _device->CreateTexture2D( &depthBufferDesc, nullptr, &_depthStencilBuffer ) ) )
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );

	depthStencilDesc.DepthEnable	= true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc		= D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable	= true;
	depthStencilDesc.StencilReadMask	= 0xFF;
	depthStencilDesc.StencilWriteMask	= 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	if ( FAILED( _device->CreateDepthStencilState( &depthStencilDesc, &_depthStencilState ) ) )
	{
		return false;
	}

	_deviceContext->OMSetRenderTargets( 1, &_renderTargetView, _depthStencilView );

	D3D11_RASTERIZER_DESC rasterDesc;
	
	rasterDesc.AntialiasedLineEnable	= false;
	rasterDesc.CullMode					= D3D11_CULL_BACK;
	rasterDesc.DepthBias				= 0;
	rasterDesc.DepthBiasClamp			= 0.0f;
	rasterDesc.DepthClipEnable			= true;
	rasterDesc.FillMode					= D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise	= false;
	rasterDesc.MultisampleEnable		= false;
	rasterDesc.ScissorEnable			= false;
	rasterDesc.SlopeScaledDepthBias		= 0.0f;

	if ( FAILED( _device->CreateRasterizerState( &rasterDesc, &_rasterState ) ) )
	{
		return false;
	}

	_deviceContext->RSSetState( _rasterState );

	D3D11_VIEWPORT viewPort;

	viewPort.Width		= static_cast<float>( screenWidth );
	viewPort.Height		= static_cast<float>( screenHeight );
	viewPort.MinDepth	= 0.0f;
	viewPort.MinDepth	= 0.0f;
	viewPort.TopLeftX	= 0.0f;
	viewPort.TopLeftY	= 0.0f;

	_deviceContext->RSSetViewports( 1, &viewPort );

	float fieldOfView	= 3.141592654f / 4.0f;
	float screenAspect	= static_cast<float>( screenWidth ) / static_cast<float>( screenHeight );

	_projectionMatrix	= DirectX::XMMatrixPerspectiveFovLH( fieldOfView, screenAspect, screenNear, screenDepth );
	_worldMatrix		= DirectX::XMMatrixIdentity();

	_orthoMatrix		= DirectX::XMMatrixOrthographicLH( static_cast<float>( screenWidth ), static_cast<float>( screenHeight ), screenNear, screenDepth );

	return true;
}

bool D3Object::shutDown( void ) noexcept
{
	if ( nullptr != _swapChain )
	{
		_swapChain->SetFullscreenState( false , nullptr );
	}

	if ( nullptr != _rasterState )
	{
		_rasterState->Release();
		_rasterState = nullptr;
	}

	if ( nullptr != _depthStencilView )
	{
		_depthStencilView->Release();
		_depthStencilView = nullptr;
	}

	if ( nullptr != _depthStencilState )
	{
		_depthStencilState->Release();
		_depthStencilState = nullptr;
	}

	if ( nullptr != _depthStencilBuffer )
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = nullptr;
	}

	if ( nullptr != _renderTargetView )
	{
		_renderTargetView->Release();
		_renderTargetView = nullptr;
	}

	if ( nullptr != _deviceContext )
	{
		_deviceContext->Release();
		_deviceContext = nullptr;
	}

	if ( nullptr != _device )
	{
		_device->Release();
		_device = nullptr;
	}

	if ( nullptr != _swapChain )
	{
		_swapChain->Release();
		_swapChain = nullptr;
	}

	return true;
}

void D3Object::beginScene( const float red, const float green, const float blue, const float alpha ) noexcept
{
	float color[4] = { red, green, blue, alpha };

	_deviceContext->ClearRenderTargetView( _renderTargetView, color );
	_deviceContext->ClearDepthStencilView( _depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

void D3Object::endScene( void ) noexcept
{
	if ( true == _vsyncEnable ) 
	{
		_swapChain->Present( 1, 0 );
	}
	else
	{
		_swapChain->Present( 0, 0 );
	}
}

const ID3D11Device* D3Object::getDevice( void ) const noexcept
{
	return _device;
}

const ID3D11DeviceContext * D3Object::getDeviceContext(void) const noexcept
{
	return _deviceContext;
}

void D3Object::getProjectionMatrix( DirectX::XMMATRIX & projectionMatrix ) noexcept
{
	projectionMatrix = _projectionMatrix;
}

void D3Object::getWorldMatrix( DirectX::XMMATRIX & worldMatrix ) noexcept
{
	worldMatrix = _worldMatrix;
}

void D3Object::getOrthoMatrix( DirectX::XMMATRIX & orthoMatrix ) noexcept
{
	orthoMatrix = _orthoMatrix;
}

void D3Object::getVideoCardInfo( const char * cardName, int & memory ) noexcept
{
	//strcpy_s( cardName, static_cast<size_t>( 128 ), _videoCardDescription );
	memory = _videoCardMemory;
}


