#pragma once


class CameraManager : public AlignedAllocationPolicy<16>
{
public:
	CameraManager( void );
	CameraManager( const CameraManager& rhs );
	~CameraManager( void ) noexcept;

	CameraManager& operator=( const CameraManager& rhs ) = delete;

	void setPosition( const float x, const float y, const float z ) noexcept;
	void setRotation( const float x, const float y, const float z ) noexcept;

	DirectX::XMFLOAT3 getPosition( void ) const noexcept;
	DirectX::XMFLOAT3 getRotation( void ) const noexcept;

	void render( void ) noexcept;
	void getViewMatrix( _Out_ DirectX::XMMATRIX& viewMatrix ) const noexcept;

private:
	DirectX::XMFLOAT3	m_position;
	DirectX::XMFLOAT3	m_rotation;
	DirectX::XMMATRIX	m_viewMatrix;

};