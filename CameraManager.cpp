#include "pch.h"
#include "CameraManager.h"

CameraManager::CameraManager( void )
	: m_position{ DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) }
	, m_rotation{ DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f ) }
{

}

CameraManager::CameraManager( const CameraManager & rhs )
{

}

CameraManager::~CameraManager( void ) noexcept
{

}

void CameraManager::setPosition( const float x, const float y, const float z ) noexcept
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void CameraManager::setRotation( const float x, const float y, const float z ) noexcept
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

DirectX::XMFLOAT3 CameraManager::getPosition( void ) const noexcept
{
	return m_position;
}

DirectX::XMFLOAT3 CameraManager::getRotation( void ) const noexcept
{
	return m_rotation;
}

void CameraManager::render(void) noexcept
{
	DirectX::XMFLOAT3 up				= DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f );
	DirectX::XMVECTOR upVector			= DirectX::XMLoadFloat3( &up );
	DirectX::XMFLOAT3 position			= m_position;
	DirectX::XMVECTOR positionVector	= DirectX::XMLoadFloat3( &position );
	DirectX::XMFLOAT3 lookAt			= DirectX::XMFLOAT3( 0.0f, 0.0f, 1.0f );
	DirectX::XMVECTOR lookAtVector		= DirectX::XMLoadFloat3( &lookAt );

	float pitch							= m_rotation.x * 0.0174532925f;
	float yaw							= m_rotation.y * 0.0174532925f;
	float roll							= m_rotation.z * 0.0174532925f;

	DirectX::XMMATRIX rotationMatrix	= DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll );
	lookAtVector						= DirectX::XMVector3TransformCoord( lookAtVector, rotationMatrix );
	upVector							= DirectX::XMVector3TransformCoord( upVector, rotationMatrix );

	lookAtVector						= DirectX::XMVectorAdd( positionVector, lookAtVector );
	m_viewMatrix						= DirectX::XMMatrixLookAtLH( positionVector, lookAtVector, upVector );
}

void CameraManager::getViewMatrix( _Out_ DirectX::XMMATRIX& viewMatrix ) const noexcept
{
	viewMatrix = m_viewMatrix;
}
