#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


class Application
{

public: virtual ~Application( void );

public: static Application*				getInstance( void ) noexcept;

private: Application( void );

};