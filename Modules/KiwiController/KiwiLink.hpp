/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#ifndef KIWI_CONTROLLER_LINK_HPP_INCLUDED
#define KIWI_CONTROLLER_LINK_HPP_INCLUDED

#include "KiwiObject.hpp"

namespace kiwi
{
    namespace controller
    {
        // ================================================================================ //
        //                                      LINK                                        //
        // ================================================================================ //
        
        //! The link is used to create a connection between objects.
        /**
         The link is a combination of two sockets used to create the connection between objects in a patcher.
         */
        class Link
        {
        public:
            
            //! The constructor.
            /** You should never use this method.
             */
            Link(model::Link model, Object* from, Object* to);
            
            //! The destructor.
            /** You should never use this method.
             */
            virtual ~Link();
            
            //! Retrieve the output object.
            /** The function retrieves the output object of the link.
             @return The output object.
             */
            inline Object* getObjectFrom() const noexcept   { return m_object_from; }
            
            //! Retrieve the input object.
            /** The function retrieves the input object of the link.
             @return The input object.
             */
            inline Object* getObjectTo() const noexcept     { return m_object_to; }
            
            //! Retrieve the index of the outlet of the link.
            /** The function retrieves the index of the outlet of the link.
             @return The index of the outlet of the link.
             */
            inline uint32_t getOutletIndex() const noexcept { return m_model.getOutletIndex(); }
            
            //! Retrieve the index of the inlet of the link.
            /** The function retrieves the index of the inlet of the link.
             @return The index of the inlet of the link.
             */
            inline uint32_t getInletIndex() const noexcept  { return m_model.getInletIndex(); }
            
        private:
            model::Link&    m_model;
            Object*         m_object_from;
            Object*         m_object_to;
        };
    }
}

#endif // KIWI_CONTROLLER_LINK_HPP_INCLUDED
