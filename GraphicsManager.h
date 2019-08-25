#pragma once


class D3Object;

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

	bool _isValid;

	D3Object* _direct3D;

};