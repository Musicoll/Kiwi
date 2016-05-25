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

#include "KiwiObject.hpp"
#include "KiwiLink.hpp"
#include "KiwiPatcher.hpp"

#include "KiwiPatcherModel.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                  LINK::declare                                   //
        // ================================================================================ //
        
        void Link::declare()
        {
            if(PatcherModel::has<Link>()) return;
            
            PatcherModel::declare<Link>()
            .name("cicm.kiwi.Link")
            .member<flip::ObjectRef<model::Object>, &Link::m_sender>("sender_obj")
            .member<flip::ObjectRef<model::Object>, &Link::m_receiver>("receiver_obj")
            .member<flip::Int, &Link::m_index_outlet>("outlet_index")
            .member<flip::Int, &Link::m_index_inlet>("inlet_index");
        }
        
        // ================================================================================ //
        //                                      LINK                                        //
        // ================================================================================ //
        
        Link::Link(model::Object const& from, const size_t outlet, model::Object const& to, const size_t inlet) :
        m_sender(from.ref()),
        m_receiver(to.ref()),
        m_index_outlet(outlet),
        m_index_inlet(inlet)
        {
            ;
        }
    }
}
