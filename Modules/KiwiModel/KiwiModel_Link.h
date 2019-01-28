/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include "KiwiModel_Object.h"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      LINK                                        //
        // ================================================================================ //
        
        //! @brief The Link is used to create a connection between two objects.
        //! @details The Link holds a reference to the sender Object, to the receiver Object
        //! and inlet and outlet indexes.
        class Link : public flip::Object
        {
        public: // methods
            
            //! @brief Constructs a Link.
            //! @details Constructs a Link with given origin and destination Object pointers
            //! and IO indexes.
            //! @param from     The sender Object.
            //! @param outlet   The sender outlet index.
            //! @param to       The receiver Object.
            //! @param inlet    The receiver inlet index.
            Link(model::Object const& from, const size_t outlet, model::Object const& to, const size_t inlet);
            
            //! @brief Destructor.
            ~Link() = default;
            
            //! @brief Gets the Object that sends messages.
            model::Object const& getSenderObject() const;
            
            //! @brief Gets the Object that receives messages.
            model::Object const& getReceiverObject() const;
            
            //! @brief Checks if the sender object is still in document.
            bool isSenderValid() const;
            
            //! @brief Checks if the sender object is still in document.
            bool isReceiverValid() const;
            
            //! @brief Returns the sender outlet index.
            size_t getSenderIndex() const;
            
            //! @brief Returns the receiver inlet index.
            size_t getReceiverIndex() const;
            
            //! Returns true if it is a signal link.
            bool isSignal() const;
            
            
        public: // internal methods
            
            //! @internal flip Default constructor
            Link(flip::Default&) {}
            
            //! @internal flip static declare method
            static void declare();
            
        private: // members
            
            flip::ObjectRef<model::Object>  m_sender;
            flip::ObjectRef<model::Object>  m_receiver;
            flip::Int                       m_index_outlet;
            flip::Int                       m_index_inlet;
            
        private: // deleted methods
            
            Link(Link const&) = delete;
            Link(Link&&) = delete;
            Link& operator=(Link const&) = delete;
            Link& operator=(Link&&) = delete;
        };
    }
}
