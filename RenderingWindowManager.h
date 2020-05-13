#pragma once

#include "RenderingWindow.h"


class RenderingWindowManager : public RenderingWindow
{

public: RenderingWindowManager( void );
public: virtual ~RenderingWindowManager( void );

public: virtual void			initialize( void ) noexcept override final;
public: virtual void			shutdown( void ) noexcept override final;

};