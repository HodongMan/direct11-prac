#pragma once


class D3Object;
class CameraManager;
class ModelObject;
class ColorShaderManager;

class GraphicsManager
{
public:
	GraphicsManager( void );
	GraphicsManager( const GraphicsManager& rhs );
	virtual ~GraphicsManager( void );

	bool initialize( _In_ const int screenWidth, _In_ const int screenHeight, _In_ HWND hwnd) noexcept;
	bool isValid( void ) const noexcept;

	void shutDown( void ) noexcept;
	bool frame( void ) noexcept;

private:

	bool render( void ) noexcept;

private:

	bool				m_isValid;
	D3Object*			m_direct3D;
	CameraManager*		m_cameraManager;
	ModelObject*		m_modelObject;
	ColorShaderManager*	m_colorShaderManager;

};