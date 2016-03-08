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

#ifndef __DEF_KIWI_MODELS_LINK__
#define __DEF_KIWI_MODELS_LINK__

#include "KiwiObject.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      LINK                                        //
    // ================================================================================ //
    
    //! The link is used to create a connection between objects.
    /**
     The link is a combination of two sockets used to create the connection between objects in a patcher.
     */
    class Link : public flip::Object,
                 public Attribute::Manager
    {
    private:
        flip::ObjectRef<Object>  m_object_from;
        flip::ObjectRef<Object>  m_object_to;
        flip::Int                m_index_outlet;
        flip::Int                m_index_intlet;
        
    public:
        
        Link() = default;
        
        //! The constructor.
        /** You should never use this method.
         */
        Link(Object* from, const uint8_t outlet, Object* to, const uint8_t inlet) noexcept;
        
        //! The destructor.
        /** You should never use this method.
         */
        virtual ~Link();
        
        //! flip declare method
        static void declare()
        {
            Model::declare<Link>()
            .name("cicm.kiwi.Link")
            .member<flip::ObjectRef<Object>, &Link::m_object_from>("object_from")
            .member<flip::ObjectRef<Object>, &Link::m_object_to>("object_to")
            .member<flip::Int, &Link::m_index_outlet>("outlet_index")
            .member<flip::Int, &Link::m_index_intlet>("inlet_index");
        }
        
        
        //! Retrieve the patcher of the link.
        /** The function retrieves the patcher of the link.
         @return The patcher of the link.
         */
        inline Patcher* getPatcher()
        {
            Patcher* patcher = parent().ptr<Patcher>();
            return patcher;
        }
        
        //! Retrieve the output object.
        /** The function retrieves the output object of the link.
         @return The output object.
         */
        inline Object* getObjectFrom() const noexcept
        {
            Object* objptr = m_object_from;
            return objptr;
        }
        
        //! Retrieve the input object.
        /** The function retrieves the input object of the link.
         @return The input object.
         */
        inline Object* getObjectTo() const noexcept
        {
            Object* objptr = m_object_to;
            return objptr;
        }
        
        //! Retrieve the index of the outlet of the link.
        /** The function retrieves the index of the outlet of the link.
         @return The index of the outlet of the link.
         */
        inline int64_t getOutletIndex() const noexcept
        {
            return m_index_outlet;
        }
        
        //! Retrieve the index of the inlet of the link.
        /** The function retrieves the index of the inlet of the link.
         @return The index of the inlet of the link.
         */
        inline int64_t getInletIndex() const noexcept
        {
            return m_index_intlet;
        }
    };

}


#endif


