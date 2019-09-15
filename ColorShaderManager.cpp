#include "pch.h"

#include "ColorShaderManager.h"

ColorShaderManager::ColorShaderManager( void )
	: m_vertexShader( nullptr )
	, m_pixelShader( nullptr )
	, m_layout( nullptr )
	, m_matrixBuffer( nullptr )
{
}

ColorShaderManager::ColorShaderManager( const ColorShaderManager & rhs )
	: m_vertexShader( nullptr )
	, m_pixelShader( nullptr )
	, m_layout( nullptr )
	, m_matrixBuffer( nullptr )
{

}

ColorShaderManager::~ColorShaderManager( void )
{

}

bool ColorShaderManager::initialize( ID3D11Device * device, HWND hWnd ) noexcept
{
	return initializeShader( device, hWnd, L"Color.vs", L"Color.ps" );
}

void ColorShaderManager::shutdown( void ) noexcept
{
	shutdownShader();
}

bool ColorShaderManager::render( const ID3D11DeviceContext * deviceContext, const int indexCount, const DirectX::XMMATRIX worldMatrix, const DirectX::XMMATRIX viewMatrix, const DirectX::XMMATRIX projectionMatrix ) noexcept
{
	if ( false == setShaderParameters( deviceContext, worldMatrix, viewMatrix, projectionMatrix ) )
	{
		return false;
	}

	renderShader( deviceContext, indexCount );

	return true;
}

bool ColorShaderManager::initializeShader( ID3D11Device * device, HWND hWnd, const WCHAR * vsFileName, const WCHAR * psFileName ) noexcept
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

	if ( FAILED( device->CreateVertexShader( vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &m_vertexShader ) ) )
	{
		return false;
	}

	if ( FAILED( device->CreatePixelShader( pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &m_pixelShader ) ) )
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	polygonLayout[0].SemanticName	= "POSITION";
	polygonLayout[0].SemanticIndex	= 0;
	polygonLayout[0].Format			= DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot		= 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName	= "COLOR";
	polygonLayout[1].SemanticIndex	= 0;
	polygonLayout[1].Format			= DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot		= 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	const unsigned int numElements = sizeof( polygonLayout ) / sizeof( polygonLayout[0] );

	if ( FAILED( device->CreateInputLayout( polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout ) ) )
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	D3D11_BUFFER_DESC matrixBufferDesc;

	matrixBufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth		= sizeof( MatrixBufferType );
	matrixBufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags		= 0;
	matrixBufferDesc.StructureByteStride = 0;

	if ( FAILED( device->CreateBuffer( &matrixBufferDesc, nullptr, &m_matrixBuffer ) ) )
	{
		return false;
	}

	return true;
}

void ColorShaderManager::shutdownShader( void ) noexcept
{
	if ( nullptr != m_matrixBuffer )
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}

	if ( nullptr != m_layout )
	{
		m_layout->Release();
		m_layout = nullptr;
	}

	if ( nullptr != m_pixelShader )
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	if ( nullptr != m_vertexShader )
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
}

void ColorShaderManager::outputShaderErrorMessage( ID3D10Blob * errorMessage, HWND hWnd, const WCHAR * shaderFileName ) const noexcept
{
	OutputDebugStringA( reinterpret_cast<const char*>( errorMessage->GetBufferPointer() ) );

	errorMessage->Release();
	errorMessage = nullptr;

	MessageBox( hWnd, L"Error compiling shader.", shaderFileName, MB_OK );
}

bool ColorShaderManager::setShaderParameters( ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix ) noexcept
{
	worldMatrix			= DirectX::XMMatrixTranspose( worldMatrix );
	viewMatrix			= DirectX::XMMatrixTranspose( viewMatrix );
	projectionMatrix	= DirectX::XMMatrixTranspose( projectionMatrix );

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if ( FAILED( deviceContext->Map( m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource ) ) )
	{
		return false;
	}

	MatrixBufferType* dataPtr = static_cast<MatrixBufferType*>( mappedResource.pData );

	dataPtr->world		= worldMatrix;
	dataPtr->view		= viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap( m_matrixBuffer, 0 );

	unsigned bufferNumber = 0;

	deviceContext->VSSetConstantBuffers( bufferNumber, 1, &m_matrixBuffer );

	return true;
}

void ColorShaderManager::renderShader( ID3D11DeviceContext * deviceContext, const int indexCount ) const noexcept
{
	deviceContext->IASetInputLayout( m_layout );

	deviceContext->VSSetShader( m_vertexShader, nullptr, 0 );
	deviceContext->PSSetShader( m_pixelShader, nullptr, 0 );

	deviceContext->DrawIndexed( indexCount, 0, 0 );
}
