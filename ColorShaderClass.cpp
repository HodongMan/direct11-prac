#include "pch.h"

#include "ColorShaderClass.h"

ColorShaderClass::ColorShaderClass( void )
	: m_vertexShader( nullptr )
	, m_pixelShader( nullptr )
	, m_layout( nullptr )
	, m_matrixBuffer( nullptr )
{
}

ColorShaderClass::ColorShaderClass( const ColorShaderClass & rhs )
	: m_vertexShader( nullptr )
	, m_pixelShader( nullptr )
	, m_layout( nullptr )
	, m_matrixBuffer( nullptr )
{

}

ColorShaderClass::~ColorShaderClass( void )
{

}

bool ColorShaderClass::initialize( const ID3D11Device * device, HWND hWnd ) noexcept
{
	return initializeShader( device, hWnd, L"Color.vs", L"Color.ps" );
}

void ColorShaderClass::shutdown( void ) noexcept
{
	shutdownShader();
}

bool ColorShaderClass::render( const ID3D11DeviceContext * deviceContext, const int indexCount, const DirectX::XMMATRIX worldMatrix, const DirectX::XMMATRIX viewMatrix, const DirectX::XMMATRIX projectionMatrix ) noexcept
{
	if ( false == setShaderParameters( deviceContext, worldMatrix, viewMatrix, projectionMatrix ) )
	{
		return false;
	}

	renderShader( deviceContext, indexCount );

	return true;
}

bool ColorShaderClass::initializeShader( const ID3D11Device * device, HWND hWnd, const WCHAR * vsFileName, const WCHAR * psFileName ) noexcept
{
	ID3D10Blob* errorMessage = nullptr;

	ID3D10Blob* vertexShaderBuffer = nullptr;

	if ( FAILED( D3DCompileFromFile( vsFileName, nullptr, nullptr, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage ) ) )
	{
		if ( nullptr != errorMessage )
		{
			outputShaderErrorMessage( errorMessage, hWnd, vsFileName );
		}
		else
		{
			MessageBox( hWnd, vsFileName, L"Missing Shader File", MB_OK );
		}

		return false;
	}

	ID3D10Blob* pixelShaderBuffer = nullptr;

	if ( FAILED( D3DCompileFromFile( psFileName, nullptr, nullptr, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage ) ) )
	{
		if ( nullptr != errorMessage )
		{
			outputShaderErrorMessage( errorMessage, hWnd, psFileName );
		}
		else
		{
			MessageBox( hWnd, psFileName, L"Missing Shader File", MB_OK );
		}

		return false;
	}


}

void ColorShaderClass::shutdownShader(void) noexcept
{
}

void ColorShaderClass::outputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hWnd, const WCHAR * shaderFileName) const noexcept
{
}

bool ColorShaderClass::setShaderParameters(const ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX worldMatrix, const DirectX::XMMATRIX viewMatrix, const DirectX::XMMATRIX projectionMatrix) noexcept
{
	return false;
}

void ColorShaderClass::renderShader(const ID3D11DeviceContext * deviceContext, const int indexCount) noexcept
{
}
