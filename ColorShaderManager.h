#pragma once

class ColorShaderManager : public AlignedAllocationPolicy<16>
{
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	ColorShaderManager( void );
	ColorShaderManager( const ColorShaderManager& rhs );
	~ColorShaderManager( void );

	bool					initialize( _Out_ ID3D11Device* device, HWND hWnd ) noexcept;
	void					shutdown( void ) noexcept;
	bool					render( const ID3D11DeviceContext* deviceContext, 
									const int indexCount,
									const DirectX::XMMATRIX worldMatrix, 
									const DirectX::XMMATRIX viewMatrix,
									const DirectX::XMMATRIX projectionMatrix ) noexcept;

private:
	bool					initializeShader( _Out_ ID3D11Device* device, HWND hWnd, const WCHAR* vsFileName, const WCHAR* psFileName ) noexcept;
	void					shutdownShader( void ) noexcept;
	void					outputShaderErrorMessage( _Out_ ID3D10Blob* errorMessage, HWND hWnd, const WCHAR* shaderFileName ) const noexcept;
	
	bool					setShaderParameters( _Inout_ ID3D11DeviceContext* deviceContext,
												 _Inout_ DirectX::XMMATRIX worldMatrix, 
												 _Inout_ DirectX::XMMATRIX viewMatrix,
												 _Inout_ DirectX::XMMATRIX projectionMatrix ) noexcept;
	void					renderShader( _Inout_ ID3D11DeviceContext* deviceContext, const int indexCount ) const noexcept;

	ID3D11VertexShader*		m_vertexShader;
	ID3D11PixelShader*		m_pixelShader;
	ID3D11InputLayout*		m_layout;
	ID3D11Buffer*			m_matrixBuffer;
}; 