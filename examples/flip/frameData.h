
/* Copyright (c) 2006, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#ifndef EQ_FLIP_FRAMEDATA_H
#define EQ_FLIP_FRAMEDATA_H

#include "flip.h"

#include <eq/eq.h>

class FrameData : public eqNet::VersionedObject
{
public:
    FrameData() : VersionedObject( OBJECT_FRAMEDATA ), spin(0.) {}

    FrameData( const void* data, const uint64_t size ) 
            : VersionedObject( OBJECT_FRAMEDATA )
        {
            EQASSERT( size == sizeof( spin ));
            spin = *(float*)data;
        }

    float spin;

protected:
    const void* getInstanceData( uint64_t* size )
        { return pack( size ); }

    const void* pack( uint64_t* size )
        {
            *size = sizeof( spin );
            return &spin;
        }

    void unpack( const void* data, const uint64_t size )
        {
            EQASSERT( size == sizeof( spin ));
            spin = *(float*)data;
        }
};



#endif // EQ_FLIP_FRAMEDATA_H

