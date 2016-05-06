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

#ifndef KIWI_MODEL_LINK_HPP_INCLUDED
#define KIWI_MODEL_LINK_HPP_INCLUDED

#include "KiwiObject.hpp"

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
        public:
            
            //! @brief Constructs a Link.
            //! @details Constructs a Link with given origin and destination Object pointers
            //! and IO indexes.
            //! @param from     The sender Object.
            //! @param outlet   The sender outlet index.
            //! @param to       The receiver Object.
            //! @param inlet    The receiver inlet index.
            Link(model::Object const& from, const uint32_t outlet, model::Object const& to, const uint32_t inlet);
            
            //! @brief Destructor.
            virtual ~Link() = default;
            
            //! @brief Get the source Object ID of the link.
            //! @return The source Object ID of the link.
            model::Object const& getSenderObject() const noexcept
            {
                return !removed() ? *m_sender.value() : *m_sender.before();
            }
            
            //! @brief Get the destination Object of the link.
            //! @return The destination Object of the link.
            model::Object const& getReceiverObject() const noexcept
            {
                return !removed() ? *m_receiver.value() : *m_receiver.before();
            }
            
            //! @brief Get the actual origin outlet index of the link.
            //! @return The actual origin outlet index of the link.
            uint32_t getSenderIndex() const noexcept
            {
                int64_t value = !removed() ? m_index_outlet.value() : m_index_outlet.before();
                return static_cast<uint32_t>(value);
            }
            
            //! @brief Get the actual destination inlet index of the link.
            //! @return The actual destination inlet index of the link.
            uint32_t getReceiverIndex() const noexcept
            {
                int64_t value = !removed() ? m_index_inlet.value() : m_index_inlet.before();
                return static_cast<uint32_t>(value);
            }
            
        public:
            
            //! @internal flip Default constructor
            Link(flip::Default&) {}
            
            //! @internal flip static declare method
            template<class TModel> static void declare();
            
        private:
            flip::ObjectRef<model::Object>  m_sender;
            flip::ObjectRef<model::Object>  m_receiver;
            flip::Int                       m_index_outlet;
            flip::Int                       m_index_inlet;
        };
        
        // ================================================================================ //
        //                                  LINK::declare                                   //
        // ================================================================================ //
        
        template<class TModel>
        void Link::declare()
        {
            if(TModel::template has<Link>()) return;
            
            TModel::template declare<Link>()
            .name("cicm.kiwi.Link")
            .template member<flip::ObjectRef<model::Object>, &Link::m_sender>("sender_obj")
            .template member<flip::ObjectRef<model::Object>, &Link::m_receiver>("receiver_obj")
            .template member<flip::Int, &Link::m_index_outlet>("outlet_index")
            .template member<flip::Int, &Link::m_index_inlet>("inlet_index");
        }
    }
}


#endif // KIWI_MODEL_LINK_HPP_INCLUDED
