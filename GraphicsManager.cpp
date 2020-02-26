#include "pch.h"

#include "TypeDefined.h"
#include "GraphicsManager.h"
#include "CameraManager.h"
#include "D3Object.h"
#include "ColorShaderManager.h"
#include "ModelObject.h"


GraphicsManager::GraphicsManager( void )
	: m_direct3D{ nullptr }
	, m_modelObject{ nullptr }
	, m_cameraManager{ nullptr }
	, m_colorShaderManager{ nullptr }
{
	m_isValid = false;
}

GraphicsManager::GraphicsManager( const GraphicsManager& rhs )
	: m_direct3D{ nullptr }
	, m_modelObject{ nullptr }
	, m_cameraManager{ nullptr }
	, m_colorShaderManager{ nullptr }
{
	m_isValid = false;
}

GraphicsManager::~GraphicsManager( void )
{
		
}

bool GraphicsManager::initialize( const int screenWidth, const int screenHeight, HWND hwnd ) noexcept
{
	m_direct3D = (D3Object*)_aligned_malloc( sizeof(D3Object), 16 );
	if ( nullptr == m_direct3D )
	{
		return false;
	}

	if ( false == m_direct3D->initialize( screenWidth, screenHeight, VSYNC_ENABLE, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR ) )
	{
		MessageBox( hwnd, L"Driect3D 실행에 실패했습니다.", L"Error", MB_OK );
		return false;
	}

	m_cameraManager = new CameraManager;
	if ( nullptr == m_cameraManager )
	{
		return false;
	}

	m_modelObject = new ModelObject;
	if ( nullptr == m_modelObject )
	{
		return false;
	}

	if ( false == m_modelObject->initialize( m_direct3D->getDevice() ) )
	{
		return false;
	}

	m_colorShaderManager = new ColorShaderManager;
	if ( nullptr == m_colorShaderManager )
	{
		return false;
	}

	if ( false == m_colorShaderManager->initialize( m_direct3D->getDevice(), hwnd ) )
	{
		return false;
	}

	return true;
}

bool GraphicsManager::isValid( void ) const noexcept
{
	return m_isValid;
}

void GraphicsManager::shutDown( void ) noexcept
{
	if ( nullptr != m_direct3D )
	{
		m_direct3D->shutDown();
		_aligned_free( m_direct3D );
		m_direct3D = nullptr;
	}

	if ( nullptr != m_modelObject )
	{
		m_modelObject->shutDown();
		_aligned_free( m_modelObject );
		m_modelObject = nullptr;
	}

	if ( nullptr != m_cameraManager )
	{
		_aligned_free( m_cameraManager );
		m_cameraManager = nullptr;
	}

	if ( nullptr != m_colorShaderManager )
	{
		_aligned_free( m_colorShaderManager );
		m_colorShaderManager = nullptr;
	}
}

bool GraphicsManager::frame( void ) noexcept
{
	return render();
}

bool GraphicsManager::render( void ) noexcept
{
	m_direct3D->beginScene( 0.5f, 0.1f, 0.1f, 1.0f );

	m_cameraManager->render();

	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;

	m_direct3D->getWorldMatrix( worldMatrix );
	m_cameraManager->getViewMatrix( viewMatrix );
	m_direct3D->getProjectionMatrix( projectionMatrix );

	m_modelObject->render( m_direct3D->getDeviceContext() );

	if ( false == m_colorShaderManager->render( m_direct3D->getDeviceContext(), m_modelObject->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix ) )
	{
		return false;
	}

	m_direct3D->endScene();

	return true;
}