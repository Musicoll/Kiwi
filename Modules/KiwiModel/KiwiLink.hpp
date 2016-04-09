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
        
        //! @brief The Link is used to create a connection between objects.
        //! @details The Link holds a reference from the origin Object and to the destination Object
        //! as well as IO indexes.
        class Link : public flip::Object
        {
        public:
            
            //! @internal flip Default constructor
            Link(flip::Default&) {}
            
            //! @brief Constructs a Link.
            //! @details Constructs a Link with given origin and destination Object pointers
            //! and IO indexes.
            //! @param from     The origin Object.
            //! @param outlet   The origin outlet index.
            //! @param to       The destination Object.
            //! @param inlet    The destination inlet index.
            Link(ObjectId const& from, const uint32_t outlet, ObjectId const& to, const uint32_t inlet);
            
            //! @brief Destructor.
            virtual ~Link() {}
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                if(TModel::template has<Link>()) return;
                
                TModel::template declare<Link>()
                .name("cicm.kiwi.Link")
                .template member<flip::ObjectRef<model::Object>, &Link::m_sender>("sender_obj")
                .template member<flip::ObjectRef<model::Object>, &Link::m_receiver>("receiver_obj")
                .template member<flip::Int, &Link::m_index_outlet>("outlet_index")
                .template member<flip::Int, &Link::m_index_inlet>("inlet_index");
            }
            
            //! @brief Returns the Link Id
            inline LinkId getId() const {return this;}
            
            //! @brief Get the source Object ID of the link.
            //! @return The source Object ID of the link.
            inline ObjectId getSenderId() const noexcept
            {
                return !removed() ? m_sender.value() : m_sender.before();
            }
            
            //! @brief Get the destination Object of the link.
            //! @return The destination Object of the link.
            inline ObjectId getReceiverId() const noexcept
            {
                return !removed() ? m_receiver.value() : m_receiver.before();
            }
            
            //! @brief Get the actual origin outlet index of the link.
            //! @return The actual origin outlet index of the link.
            inline uint32_t getSenderIndex() const noexcept
            {
                int64_t value = (!removed()) ? m_index_outlet.value() : m_index_outlet.before();
                return static_cast<uint32_t>(value);
            }
            
            //! @brief Get the actual destination inlet index of the link.
            //! @return The actual destination inlet index of the link.
            inline uint32_t getReceiverIndex() const noexcept
            {
                int64_t value = (!removed()) ? m_index_inlet.value() : m_index_inlet.before();
                return static_cast<uint32_t>(value);
            }
            
        private:
            flip::ObjectRef<model::Object>  m_sender;
            flip::ObjectRef<model::Object>  m_receiver;
            flip::Int                       m_index_outlet;
            flip::Int                       m_index_inlet;
        };
    }
}


#endif // KIWI_MODEL_LINK_HPP_INCLUDED
