/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_MODEL_LINK_HPP_INCLUDED
#define KIWI_MODEL_LINK_HPP_INCLUDED

#include "KiwiModelObject.hpp"

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
            virtual ~Link() = default;
            
            //! @brief Gets the Object that sends messages.
            model::Object const& getSenderObject() const;
            
            //! @brief Gets the Object that receives messages.
            model::Object const& getReceiverObject() const;
            
            //! @brief Returns the sender outlet index.
            size_t getSenderIndex() const;
            
            //! @brief Returns the receiver inlet index.
            size_t getReceiverIndex() const;
            
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


#endif // KIWI_MODEL_LINK_HPP_INCLUDED
