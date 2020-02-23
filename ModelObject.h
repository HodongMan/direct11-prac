#pragma once

class ModelObject : public AlignedAllocationPolicy<16>
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

public:
	ModelObject( void );
	ModelObject( const ModelObject& rhs );
	~ModelObject( void ) noexcept;

	bool				initialize( _Out_ ID3D11Device* device ) noexcept;
	void				shutDown( void ) noexcept;
	void				render( _Out_ ID3D11DeviceContext* deviceContext ) noexcept;

	int					getIndexCount( void ) const noexcept;

private:

	bool				initializeBuffers( _Out_ ID3D11Device* device ) noexcept;
	void				shutdownBuffers( void ) noexcept;
	void				renderBuffer( _Out_ ID3D11DeviceContext* deviceContext ) noexcept;

private:

	ID3D11Buffer*		m_vertexBuffer;
	ID3D11Buffer*		m_indexBuffer;
	int					m_vertexCount;
	int					m_indexCount;
};