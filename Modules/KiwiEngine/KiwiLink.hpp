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

#include <KiwiModel/KiwiLink.hpp>

#include "KiwiObject.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      LINK                                        //
        // ================================================================================ //
        
        //! @brief The link represents the connection between the outlet of an object to the inlet of another.
        class Link
        {
        public: // methods
            
            //! @brief Constructor.
            Link(model::Link& model, Object& sender_object, Object& receiver_object);
            
            //! @brief Destructor.
            ~Link();
            
            //! @brief Get the object that send messages.
            Object& getSenderObject() noexcept;
            
            //! @brief Get the object that send messages.
            Object const& getSenderObject() const noexcept;
            
            //! @brief Get the object that receive messages.
            Object& getReceiverObject() noexcept;
            
            //! @brief Get the object that receive messages.
            Object const& getReceiverObject() const noexcept;
            
            //! @brief Retrieve the index of the outlet of the link.
            size_t getSenderIndex() const noexcept;
            
            //! @brief Retrieve the index of the inlet of the link.
            size_t getReceiverIndex() const noexcept;
            
        private: // members
            
            model::Link const&  m_model;
            Object&             m_sender;
            Object&             m_receiver;
            
        private: // deleted methods
            
            Link(Link const&) = delete;
            Link(Link&&) = delete;
            Link& operator=(Link const&) = delete;
            Link& operator=(Link&&) = delete;
        };
    }
}

#endif // KIWI_ENGINE_LINK_HPP_INCLUDED
