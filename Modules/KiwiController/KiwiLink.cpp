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
    namespace controller
    {
        // ================================================================================ //
        //                                      LINK                                        //
        // ================================================================================ //
        
        Link::Link(model::Link& model, Object* from, Object* to) :
        m_model(model),
        m_object_from(from),
        m_object_to(to)
        {
            const auto  from_idx = m_model.getOutletIndex();
            const auto  to_idx   = m_model.getInletIndex();
            auto* const outlet   = m_object_from->getOutlet(from_idx);
            auto* const inlet    = m_object_to->getInlet(to_idx);

            if(outlet && inlet)
            {
                outlet->append(m_object_to, to_idx);
                inlet->append(m_object_from, from_idx);
            }
        }
        
        Link::~Link()
        {
            if(m_object_from && m_object_to)
            {
                const auto  from_idx = m_model.getOutletIndex();
                const auto  to_idx   = m_model.getInletIndex();
                auto* const outlet   = m_object_from->getOutlet(from_idx);
                auto* const inlet    = m_object_to->getInlet(to_idx);
                
                if(inlet)
                {
                    inlet->erase(m_object_from, from_idx);
                }
                
                if(outlet)
                {
                    outlet->erase(m_object_to, to_idx);
                }
            }
        }
    }
}
