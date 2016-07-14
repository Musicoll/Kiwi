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

#include "KiwiModelDataModel.hpp"

#include "KiwiModelObject.hpp"
#include "KiwiModelLink.hpp"
#include "KiwiModelPatcherUser.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                  LINK::declare                                   //
        // ================================================================================ //
        
        void Link::declare()
        {
            if(DataModel::has<Link>()) return;
            
            DataModel::declare<Link>()
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
        
        model::Object const& Link::getSenderObject() const
        {
            return !removed() ? *m_sender.value() : *m_sender.before();
        }
        
        model::Object const& Link::getReceiverObject() const
        {
            return !removed() ? *m_receiver.value() : *m_receiver.before();
        }
        
        size_t Link::getSenderIndex() const
        {
            int64_t value = !removed() ? m_index_outlet.value() : m_index_outlet.before();
            return static_cast<size_t>(value);
        }

        size_t Link::getReceiverIndex() const
        {
            int64_t value = !removed() ? m_index_inlet.value() : m_index_inlet.before();
            return static_cast<size_t>(value);
        }
    }
}
