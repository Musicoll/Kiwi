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

#include "KiwiAttr.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                 ATTRIBUTE MANAGER								//
    // ================================================================================ //
    
    void Attr::Manager::write(Dico& dico) const
    {
        ;
    }
    
    void Attr::Manager::read(Dico const& dico)
    {
        for(auto it : dico) {setAttrValue(it.first, it.second);}
    }
    
    void Attr::Manager::addListener(sListener listener, vector<sTag> const& names)
    {
        if(listener)
        {
            if(names.empty())
            {
                lock_guard<mutex> guard(m_attrs_mutex);
                for(auto it : m_attrs)
                {
                    it.second->addListener(listener);
                }
            }
            else
            {
                for(auto name : names)
                {
                    sAttr attr = getAttr(name);
                    if(attr)
                    {
                        attr->addListener(listener);
                    }
                }
            }
        }
    }
    
    void Attr::Manager::removeListener(sListener listener, vector<sTag> const& names)
    {
        if(listener)
        {
            if(names.empty())
            {
                lock_guard<mutex> guard(m_attrs_mutex);
                for(auto it : m_attrs)
                {
                    it.second->removeListener(listener);
                }
            }
            else
            {
                for(auto name : names)
                {
                    sAttr attr = getAttr(name);
                    if(attr)
                    {
                        attr->removeListener(listener);
                    }
                }
            }
        }
    }
}
















