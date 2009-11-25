
/* Copyright (c) 2007-2009, Stefan Eilemann <eile@equalizergraphics.com> 
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *  
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "staticSlaveCM.h"

#include "command.h"
#include "commands.h"
#include "log.h"
#include "object.h"
#include "objectInstanceDataIStream.h"
#include "session.h"

#include <eq/base/scopedMutex.h>

namespace eq
{
namespace net
{
typedef CommandFunc< StaticSlaveCM > CmdFunc;

StaticSlaveCM::StaticSlaveCM( Object* object )
        : _object( object )
        , _currentIStream( new ObjectInstanceDataIStream )
{
    registerCommand( CMD_OBJECT_INSTANCE,
                     CmdFunc( this, &StaticSlaveCM::_cmdInstance ), 0 );
}

StaticSlaveCM::~StaticSlaveCM()
{
    delete _currentIStream;
    _currentIStream = 0;
}

//---------------------------------------------------------------------------
// command handlers
//---------------------------------------------------------------------------
CommandResult StaticSlaveCM::_cmdInstance( Command& command )
{
    EQASSERT( _currentIStream );
    _currentIStream->addDataPacket( command );

    if( _currentIStream->isReady( ))
        EQLOG( LOG_OBJECTS ) << "id " << _object->getID() << "." 
                             << _object->getInstanceID() << " ready" 
                             << std::endl;

    return eq::net::COMMAND_HANDLED;
}

void StaticSlaveCM::applyMapData()
{
    EQASSERT( _currentIStream );
    _currentIStream->waitReady();

    _object->applyInstanceData( *_currentIStream );

    delete _currentIStream;
    _currentIStream = 0;

    EQLOG( LOG_OBJECTS ) << "Mapped initial data for " << _object->getID()
                         << "." << _object->getInstanceID() << " ready" 
                         << std::endl;
}

void StaticSlaveCM::addInstanceDatas( const InstanceDataDeque& cache, 
                                      const uint32_t startVersion )
{
    EQASSERT( _currentIStream );
    EQASSERT( _currentIStream->getDataSize() == 0 );
    EQASSERT( cache.size() == 1 );
    if( cache.empty( ))
        return;

    ObjectInstanceDataIStream* stream = cache.front();
    EQASSERT( stream );
    EQASSERT( stream->isReady( ));
    EQASSERT( stream->getVersion() == VERSION_NONE );

    if( !stream->isReady() || stream->getVersion() != VERSION_NONE )
        return;

    EQLOG( LOG_OBJECTS ) << "Adding cached instance data" << std::endl;
    delete _currentIStream;
    _currentIStream = new ObjectInstanceDataIStream( *stream );
}

}
}
