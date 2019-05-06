#include "pch.h"
#include "InputManager.h"

InputManager::InputManager( void )
{
	_isValid = false;
}

InputManager::InputManager( const InputManager& rhs )
{
	_isValid = false;
}

InputManager::~InputManager( void )
{

}

void InputManager::initialize( void ) noexcept
{
	for( int i = 0; i < KeyTypeLength; ++i )
	{
		_keys[i] = false;
	}

	_isValid = true;
}

void InputManager::keyDown( const unsigned int input ) noexcept
{
	_keys[input] = true;
}

void InputManager::keyUp( const unsigned int input ) noexcept
{
	_keys[input] = false;
}

bool InputManager::isKeyDown( const unsigned int input ) const noexcept
{
	return true == _keys[input];
}