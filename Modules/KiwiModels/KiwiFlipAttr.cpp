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

#include "KiwiFlipAttr.h"

namespace kiwi
{
    // ================================================================================ //
    //                                 ATTRIBUTE MANAGER								//
    // ================================================================================ //
    
    void AttributeBase::Manager::write(Dico& dico) const
    {
        ;
    }
    
    void AttributeBase::Manager::read(Dico const& dico)
    {
        for(auto it = dico.begin(); it != dico.end(); ++it)
        {
            setAttributeValue(it->first, it->second);
        }
    }
    
    void AttributeBase::Manager::addListener(sListener listener, std::vector<sTag> const& names)
    {
        if(listener)
        {
            if(names.empty())
            {
                std::lock_guard<std::mutex> guard(m_attrs_mutex);
                for(auto& it : m_attributes)
                {
                    it.addListener(listener);
                }
            }
            else
            {
                for(auto name : names)
                {
                    AttributeBase* attr = getAttributeBase(name);
                    if(attr)
                    {
                        attr->addListener(listener);
                    }
                }
            }
        }
    }
    
    void AttributeBase::Manager::removeListener(sListener listener, std::vector<sTag> const& names)
    {
        if(listener)
        {
            if(names.empty())
            {
                std::lock_guard<std::mutex> guard(m_attrs_mutex);
                for(auto& it : m_attributes)
                {
                    it.removeListener(listener);
                }
            }
            else
            {
                for(auto name : names)
                {
                    AttributeBase* attr = getAttributeBase(name);
                    if(attr)
                    {
                        attr->removeListener(listener);
                    }
                }
            }
        }
    }
}
















