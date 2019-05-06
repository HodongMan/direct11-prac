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

	void keyDown(_In_ const unsigned int input ) noexcept;
	void keyUp(_In_ const unsigned int input ) noexcept;

	bool isKeyDown(_In_ const unsigned int input ) const noexcept;

private:
	bool _keys[KeyTypeLength];
	bool _isValid;
};