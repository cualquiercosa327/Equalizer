
/* Copyright (c) 2005, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#ifndef EQNET_CONNECTION_NETWORK_H
#define EQNET_CONNECTION_NETWORK_H

#include "networkPriv.h"
#include "connectionSet.h"

#include <iostream> 

namespace eqNet
{
    namespace priv
    {
        class Connection;

        /** A Network base class for connection-based networks. */
        class ConnectionNetwork : public Network
        {
        public:
            ConnectionNetwork( const uint id, Session* session ) 
                    : Network( id, session ) {}
            ~ConnectionNetwork(){ exit(); }

            virtual bool init();
            virtual void exit();

            /** 
             * Forces a node into started mode, used during server launch
             * 
             * @param nodeID the node identifier.
             * @param connection the open connection to the node.
             */
            void setStarted( const uint nodeID, Connection* connection );

        protected:
            /** The set of active connections. */
            ConnectionSet _connectionSet;
        };
    }
}

#endif //EQNET_CONNECTION_NETWORK_H
