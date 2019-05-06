#include "pch.h"
#include "GraphicsManager.h"

GraphicsManager::GraphicsManager( void )
{
	_isValid = false;
}

GraphicsManager::GraphicsManager( const GraphicsManager& rhs )
{
	_isValid = false;
}

GraphicsManager::~GraphicsManager( void )
{
	
}

bool GraphicsManager::initialize( const int screenWidth, const int screenHeight, HWND hwnd ) noexcept
{
	return true;
}

void GraphicsManager::shutDown( void ) noexcept
{

}

bool GraphicsManager::frame( void ) noexcept
{
	return true;
}

bool GraphicsManager::render( void ) noexcept
{
	return true;
}