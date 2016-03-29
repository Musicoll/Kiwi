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
            //! @param from     The origin Object pointer.
            //! @param outlet   The origin outlet index.
            //! @param to       The destination Object pointer.
            //! @param inlet    The destination inlet index.
            Link(model::Object& from, const uint32_t outlet, model::Object& to, const uint32_t inlet);
            
            //! @brief Destructor.
            virtual ~Link() {}
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                if(TModel::template has<Link>()) return;
                
                TModel::template declare<Link>()
                .name("cicm.kiwi.Link")
                .template member<flip::ObjectRef<model::Object>, &Link::m_object_from>("object_from")
                .template member<flip::ObjectRef<model::Object>, &Link::m_object_to>("object_to")
                .template member<flip::Int, &Link::m_index_outlet>("outlet_index")
                .template member<flip::Int, &Link::m_index_inlet>("inlet_index");
            }
            
            //! @brief Get the origin Object of the link.
            //! @return The origin Object of the link.
            inline model::Object& getObjectFrom() const noexcept            { return *m_object_from; }
            
            //! @brief Get the destination Object of the link.
            //! @return The destination Object of the link.
            inline model::Object& getObjectTo() const noexcept              { return *m_object_to; }
            
            //! @brief Get the origin outlet index of the link.
            //! @return The origin outlet index of the link.
            inline flip::Int::internal_type getOutletIndex() const noexcept { return m_index_outlet; }
            
            //! @brief Get the destination inlet index of the link.
            //! @return The destination inlet index of the link.
            inline flip::Int::internal_type getInletIndex() const noexcept  { return m_index_inlet; }
            
        private:
            flip::ObjectRef<model::Object>  m_object_from;
            flip::ObjectRef<model::Object>  m_object_to;
            flip::Int                       m_index_outlet;
            flip::Int                       m_index_inlet;
        };
    }
}


#endif // KIWI_MODEL_LINK_HPP_INCLUDED
