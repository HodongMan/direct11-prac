#pragma once

#include "TypeDefined.h"

class InputManager
{
public:
	InputManager( void );
	InputManager( const InputManager& rhs );
	virtual ~InputManager( void );

	void initialize( void ) noexcept;
	bool isValid( void ) const noexcept;

	void keyDown( const unsigned int input ) noexcept;
	void keyUp( const unsigned int input ) noexcept;

	bool isKeyDown( const unsigned int input ) const noexcept;

private:
	bool _keys[KeyTypeLength];
	bool _isValid;
};