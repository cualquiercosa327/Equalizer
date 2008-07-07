
/* Copyright (c) 2007-2008, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#include "commandQueue.h"

#ifdef GLX
#  include "glXMessagePump.h"
#endif
#ifdef WGL
#  include "wglMessagePump.h"
#endif
#ifdef AGL
#  include "aglMessagePump.h"
#endif

using namespace std;

namespace eq
{
CommandQueue::CommandQueue()
        : _messagePump( 0 )
        , _windowSystem( WINDOW_SYSTEM_NONE )
{
}

CommandQueue::~CommandQueue()
{
    delete _messagePump;
    _messagePump = 0;
}

void CommandQueue::setWindowSystem( const WindowSystem windowSystem )
{
    if( _windowSystem == windowSystem )
        return;

    EQASSERTINFO( _windowSystem == WINDOW_SYSTEM_NONE, 
                  "Can't switch window system from " << _windowSystem << " to "
                  << windowSystem );
    EQASSERT( !_windowSystem );

    _windowSystem = windowSystem;

    switch( windowSystem )
    {
#ifdef GLX
        case WINDOW_SYSTEM_GLX:
            _messagePump = new GLXMessagePump();
            break;
#endif

#ifdef WGL
        case WINDOW_SYSTEM_WGL:
            _messagePump = new WGLMessagePump();
            break;
#endif

#ifdef AGL
        case WINDOW_SYSTEM_AGL:
            _messagePump = new AGLMessagePump();
            break;
#endif

        default:
            EQUNREACHABLE;
    }
}

void CommandQueue::push(eq::net::Command& inCommand)
{
    eq::net::CommandQueue::push(inCommand);
    if( _messagePump )
        _messagePump->postWakeup();
}

void CommandQueue::pushFront(eq::net::Command& inCommand)
{
    eq::net::CommandQueue::pushFront(inCommand);
    if( _messagePump )
        _messagePump->postWakeup();
}

void CommandQueue::wakeup()
{
    eq::net::CommandQueue::wakeup();
    if( _messagePump )
        _messagePump->postWakeup();
}

eq::net::Command* CommandQueue::pop()
{
    while( true )
    {
        if( _messagePump )
            _messagePump->dispatchAll(); // non-blocking

        // Poll for a command
        if( !empty( ))
            return eq::net::CommandQueue::pop();

        if( _messagePump )
            _messagePump->dispatchOne(); // blocking - push will send wakeup
        else
            return eq::net::CommandQueue::pop();
    }
}

eq::net::Command* CommandQueue::tryPop()
{
    if( _messagePump )
        _messagePump->dispatchAll(); // non-blocking

    return eq::net::CommandQueue::tryPop();
}
}
