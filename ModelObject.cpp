#include "pch.h"
#include "ModelObject.h"

#include "TypeDefined.h"

ModelObject::ModelObject( void )
	: m_indexBuffer{ nullptr }
	, m_vertexBuffer{ nullptr }
	, m_indexCount{ 0 }
	, m_vertexCount{ 0 }
{

}

ModelObject::ModelObject( const ModelObject & rhs )
	: m_indexBuffer{ nullptr }
	, m_vertexBuffer{ nullptr }
	, m_indexCount{ 0 }
	, m_vertexCount{ 0 }
{

}

ModelObject::~ModelObject( void ) noexcept
{

}

bool ModelObject::initialize( _Out_ ID3D11Device * device ) noexcept
{
	return initializeBuffers( device );
}

void ModelObject::shutDown( void ) noexcept
{
	shutdownBuffers();
}

void ModelObject::render( _Out_ ID3D11DeviceContext * deviceContext ) noexcept
{
	renderBuffer( deviceContext );
}

int ModelObject::getIndexCount( void ) const noexcept
{
	return m_indexCount;
}

bool ModelObject::initializeBuffers( ID3D11Device * device ) noexcept
{
	m_vertexCount	= VERTEX_COUNT;
	m_indexCount	= INDEX_COUNT;

	std::unique_ptr<VertexType[]> vertices( new VertexType[ m_vertexCount ] );
	std::unique_ptr<unsigned long[]> indices( new unsigned long[ m_indexCount ] );

	vertices[0].position	= DirectX::XMFLOAT3( -1.0f, -1.0f, 0.0f );
	vertices[0].color		= DirectX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );

	vertices[1].position	= DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f );
	vertices[1].color		= DirectX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );

	vertices[2].position	= DirectX::XMFLOAT3( 1.0f, -1.0f, 0.0f );\
	vertices[2].color		= DirectX::XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f);

	indices[0] = 0;  
	indices[1] = 1;  
	indices[2] = 2;  

	D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth				= sizeof( VertexType ) * m_vertexCount;
    vertexBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags			= 0;
    vertexBufferDesc.MiscFlags				= 0;
	vertexBufferDesc.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem						= vertices.get();
	vertexData.SysMemPitch					= 0;
	vertexData.SysMemSlicePitch				= 0;

	if( FAILED( device->CreateBuffer( &vertexBufferDesc, &vertexData, &m_vertexBuffer ) ) )
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth				= sizeof( unsigned long ) * m_indexCount;
    indexBufferDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags			= 0;
    indexBufferDesc.MiscFlags				= 0;
	indexBufferDesc.StructureByteStride		= 0;

	D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem						= indices.get();
	indexData.SysMemPitch					= 0;
	indexData.SysMemSlicePitch				= 0;

	if(	FAILED( device->CreateBuffer( &indexBufferDesc, &indexData, &m_indexBuffer ) ) )
	{
		return false;
	}

	return true;
}

void ModelObject::shutdownBuffers( void ) noexcept
{
	if ( nullptr != m_vertexBuffer )
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	if ( nullptr != m_indexBuffer )
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}
}

void ModelObject::renderBuffer( _Out_ ID3D11DeviceContext * deviceContext ) noexcept
{
	const unsigned int stride = sizeof( VertexType ) * 2;
	const unsigned int offset = 0;

	deviceContext->IASetVertexBuffers( 0, 1, &m_vertexBuffer, &stride, &offset );
	deviceContext->IASetIndexBuffer( m_indexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}


