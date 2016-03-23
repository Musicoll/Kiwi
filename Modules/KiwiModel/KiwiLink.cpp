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
#include "KiwiObject.hpp"
#include "KiwiPatcher.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      LINK                                        //
        // ================================================================================ //
        
        Link::Link(model::Object* from, const uint8_t outlet, model::Object* to, const uint8_t inlet) :
        m_object_from(from),
        m_object_to(to),
        m_index_outlet(outlet),
        m_index_inlet(inlet)
        {
            addAttr<Attribute::RGBA>("color",    {0., 0., 0., 1.});
        }
        
        std::unique_ptr<Link> Link::create(model::Object* from, const uint8_t outlet,
                                           model::Object* to, const uint8_t inlet)
        {
            if(from != to)
            {
                //! @todo need to implement conditionnal return type
                return std::unique_ptr<Link>(new Link::Control(from, outlet, to, inlet));
            }
            
            return nullptr;
        }
    }
}
