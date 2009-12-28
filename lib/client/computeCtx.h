
/* Copyright (c) 2009, Philippe Robert <philippe.robert@gmail.com> 
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

#ifndef EQ_COMPUTE_CONTEXT_H
#define EQ_COMPUTE_CONTEXT_H

#include <eq/base/base.h>

namespace eq
{
    class Pipe;

    /**
     * The interface definition for API-specific GPGPU handling.
     *
     * The ComputeCtx abstracts all GPGPU API-system specific code for handling 
     * a GPU for computing purposes. Each Pipe uses one ComputeCtx, which is 
     * initialized in Pipe::configInit. 
     */
    class EQ_EXPORT ComputeCtx
    {
    public:
        /** Create a new ComputeCtx for the given accelerator (here eq::Pipe).*/
        ComputeCtx( Pipe* parent );

        /** Destroy the ComputeCtx. */
        virtual ~ComputeCtx( );

        /** @name Methods forwarded from eq::Pipe */
        //@{
        /** Initialize the ComputeCtx. */
        virtual bool configInit( ) = 0;

        /** De-initialize the ComputeCtx. */
        virtual void configExit( ) = 0;
        //@}

        /** @return the reason of the last failed operation. */
        const std::string & getErrorMessage() const { return _error; }

    protected:
        /** @name Error information. */
        //@{
        /** 
         * Set a message why the last operation failed.
         * 
         * The message will be transmitted to the originator of the request, for
         * example to Config::init when set from within the configInit method.
         *
         * @param message the error message.
         */
        void setErrorMessage( const std::string& message ) { _error = message; }
        //@}

        /** The eq::Pipe used by the context. */
        Pipe* const _pipe;

        /** The reason for the last error. */
        std::string _error;

    private:
        union // placeholder for binary-compatible changes
        {
            char dummy[64];
        };

    };		
}

#endif // EQ_COMPUTE_CONTEXT_H