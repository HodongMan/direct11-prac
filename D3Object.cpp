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

bool D3Object::initialize( int screenWidth, int screenHeight, bool vsync, HWND hWnd, float screenDepth, float screenNear ) noexcept
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

	return true;
}
