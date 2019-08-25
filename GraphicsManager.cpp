#include "pch.h"
#include "GraphicsManager.h"
#include "TypeDefined.h"
#include "D3Object.h"


GraphicsManager::GraphicsManager( void )
{
	_isValid = false;
}

GraphicsManager::GraphicsManager( const GraphicsManager& rhs )
{
	_isValid = false;
}

GraphicsManager::~GraphicsManager( void )
{
		
}

bool GraphicsManager::initialize( const int screenWidth, const int screenHeight, HWND hwnd ) noexcept
{
	_direct3D = (D3Object*)_aligned_malloc( sizeof(D3Object), 16 );
	if ( nullptr == _direct3D )
	{
		return false;
	}

	if ( false == _direct3D->initialize( screenWidth, screenHeight, VSYNC_ENABLE, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR ) )
	{
		MessageBox( hwnd, L"Driect3D 실행에 실패했습니다.", L"Error", MB_OK );
		return false;
	}

	return true;
}

void GraphicsManager::shutDown( void ) noexcept
{
	if ( nullptr != _direct3D )
	{
		_direct3D->shutDown();
		_aligned_free( _direct3D );
		_direct3D = nullptr;
	}
}

bool GraphicsManager::frame( void ) noexcept
{
	return render();
}

bool GraphicsManager::render( void ) noexcept
{
	_direct3D->beginScene( 0.5f, 0.1f, 0.1f, 1.0f );

	_direct3D->endScene();

	return true;
}