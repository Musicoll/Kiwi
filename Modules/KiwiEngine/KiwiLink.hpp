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

#ifndef KIWI_ENGINE_LINK_HPP_INCLUDED
#define KIWI_ENGINE_LINK_HPP_INCLUDED

#include "KiwiObject.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      LINK                                        //
        // ================================================================================ //
        
        //! @brief The link represents a connection between two objects.
        class Link
        {
        public:
            
            //! @brief Constructor.
            Link(model::Link& model, Object& sender_object, Object& receiver_object);
            
            //! @brief Destructor.
            ~Link();
            
            //! @brief Get the object that send messages.
            inline Object& getSenderObject() const noexcept     { return m_sender; }
            
            //! @brief Get the object that receive messages.
            inline Object& getReceiverObject() const noexcept   { return m_receiver; }
            
            //! @brief Retrieve the index of the outlet of the link.
            inline uint32_t getSenderIndex() const noexcept     { return m_model.getSenderIndex(); }
            
            //! @brief Retrieve the index of the inlet of the link.
            inline uint32_t getReceiverIndex() const noexcept   { return m_model.getReceiverIndex(); }
            
        private:
            
            //! @internal Model change notification.
            void modelChanged(model::Link& link_m);
            
        private:
            
            model::Link const&  m_model;
            Object&             m_sender;
            Object&             m_receiver;
            
            friend class Patcher;
        };
    }
}

#endif // KIWI_ENGINE_LINK_HPP_INCLUDED
