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

#include "KiwiLink.h"
#include "KiwiObject.h"
#include "KiwiPatcher.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      LINK                                        //
    // ================================================================================ //
    
    Link::Link(Object* from, const ulong outlet, Object* to, const ulong inlet) noexcept :
    m_object_from(from),
    m_object_to(to),
    m_index_outlet(outlet),
    m_index_intlet(inlet)
    {
        ;
    }
    
    Link::~Link()
    {
        Object*     from    = getObjectFrom();
        Object*     to      = getObjectTo();
        if(from && to)
        {
            /*
            Object::sOutlet outlet  = from->getOutlet(m_index_outlet);
            if(outlet)
            {
                outlet->erase(to, m_index_outlet);
            }
            Object::sInlet inlet    = to->getInlet(m_index_intlet);
            if(inlet)
            {
                inlet->erase(from, m_index_intlet);
            }
            */
        }
    }
    
    void Link::write(Dico& dico) const noexcept
    {
        Object*     from    = getObjectFrom();
        Object*     to      = getObjectTo();
        
        if(from && to)
        {
            //dico[Tags::from] = {(long)from->getId(), (long)getOutletIndex()};
            //dico[Tags::to] = {(long)to->getId(), (long)getInletIndex()};
        }
        else
        {
            dico.erase(Tags::from);
            dico.erase(Tags::to);
        }
    }
}
















