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
        class Link : public flip::Object,
                     public Attribute::Manager
        {
        public:
            
            class Control;
            class Dsp;
            
            //! @brief Enum of Link types
            //! @remark Can't use Type because of flip::Type conflict
            enum class LinkType : uint8_t
            {
                Invalid = 0,
                Control,
                Dsp
            };
            
            //! @brief default constructor.
            Link() = default;
            
            //! @brief Constructs a Link.
            //! @details Constructs a Link with given origin and destination Object pointers
            //! and IO indexes.
            //! @param from     The origin Object pointer.
            //! @param outlet   The origin outlet index.
            //! @param to       The destination Object pointer.
            //! @param inlet    The destination inlet index.
            Link(Object* from, const uint8_t outlet, Object* to, const uint8_t inlet);
            
            //! @brief Destructor.
            virtual ~Link();
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                if(TModel::template has<Link>()) return;
                
                TModel::template declare<Link>()
                .template name("cicm.kiwi.Link")
                .template member<decltype(Link::m_object_from), &Link::m_object_from>("object_from")
                .template member<decltype(Link::m_object_to),   &Link::m_object_to>("object_to")
                .template member<decltype(Link::m_index_outlet),&Link::m_index_outlet>("outlet_index")
                .template member<decltype(Link::m_index_inlet), &Link::m_index_inlet>("inlet_index")
                .template member<decltype(Link::m_color),       &Link::m_color>("color");
            }
            
            //! @brief Get the type of the Object.
            //! @return The type of the Object.
            virtual inline LinkType getType() const noexcept { return LinkType::Invalid; };
            
            //! @brief Get the parent Patcher of the link.
            //! @return The parent Patcher of the link.
            inline Patcher* getPatcher() { return parent().ptr<Patcher>(); }
            
            //! @brief Get the origin Object of the link.
            //! @return The origin Object of the link.
            inline Object* getObjectFrom() const noexcept   { return m_object_from; }
            
            //! @brief Get the destination Object of the link.
            //! @return The destination Object of the link.
            inline Object* getObjectTo() const noexcept     { return m_object_to; }
            
            //! @brief Get the origin outlet index of the link.
            //! @return The origin outlet index of the link.
            inline int64_t getOutletIndex() const noexcept  { return m_index_outlet; }
            
            //! @brief Get the destination inlet index of the link.
            //! @return The destination inlet index of the link.
            inline int64_t getInletIndex() const noexcept   { return m_index_inlet; }
            
        private:
            flip::ObjectRef<Object>  m_object_from;
            flip::ObjectRef<Object>  m_object_to;
            flip::Int                m_index_outlet;
            flip::Int                m_index_inlet;
            
            Attribute::RGBA          m_color;           ///< The Link color
        };
        
        // ================================================================================ //
        //                                   LINK CONTROL                                   //
        // ================================================================================ //
        
        class Link::Control : public Link
        {
        public:
            
            //! @brief Get the type of the Object.
            //! @return The type of the Object.
            inline LinkType getType() const noexcept final { return LinkType::Control; };
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                if(TModel::template has<Link::Control>()) return;
                
                TModel::template declare<Link::Control>()
                .template name("cicm.kiwi.Link.Control")
                .template inherit<Link>();
            }
        };
        
        // ================================================================================ //
        //                                     LINK DSP                                     //
        // ================================================================================ //
        
        class Link::Dsp : public Link
        {
        public:
            
            //! @brief Get the type of the Object.
            //! @return The type of the Object.
            inline LinkType getType() const noexcept final { return LinkType::Dsp; };
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                if(TModel::template has<Link::Dsp>()) return;
                
                TModel::template declare<Link::Dsp>()
                .template name("cicm.kiwi.Link.Dsp")
                .template inherit<Link>();
            }
        };
    }
}


#endif // KIWI_MODEL_LINK_HPP_INCLUDED
