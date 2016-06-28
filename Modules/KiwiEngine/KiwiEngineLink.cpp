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

#include "KiwiLink.hpp"

#include <KiwiModel/KiwiLink.hpp>
#include <KiwiModel/KiwiObject.hpp>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      LINK                                        //
        // ================================================================================ //
        
        Link::Link(model::Link const& model) :
        m_model(model)
        {
            ;
        }
        
        Link::~Link()
        {
            ;
        }
        
        std::shared_ptr<Object> Link::getSenderObject() noexcept
        {
            return m_model.getSenderObject().entity().use<std::shared_ptr<engine::Object>>();
        }
        
        std::shared_ptr<const Object> Link::getSenderObject() const noexcept
        {
            return m_model.getSenderObject().entity().use<std::shared_ptr<engine::Object>>();
        }
        
        std::shared_ptr<Object> Link::getReceiverObject() noexcept
        {
            return nullptr;
        }
 
        std::shared_ptr<const Object> Link::getReceiverObject() const noexcept
        {
            return nullptr;
        }
        
        size_t Link::getSenderIndex() const noexcept
        {
            return m_model.getSenderIndex();
        }
        
        size_t Link::getReceiverIndex() const noexcept
        {
            return m_model.getReceiverIndex();
        }
    }
}
