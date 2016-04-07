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

#include "KiwiPatcher.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                   PATCHER MODEL                                  //
        // ================================================================================ //
        
        Patcher::Patcher()
        {
            ;
        }
        
        Patcher::~Patcher()
        {
            m_links.clear();
            m_objects.clear();
        }
        
        ID Patcher::addObject(std::unique_ptr<model::Object> object)
        {
            if(object)
            {
                const auto it = m_objects.insert(m_objects.end(), std::move(object));
                return it->ref();
            }
            
            return flip::Ref::null;
        }
        
        ID Patcher::addLink(std::unique_ptr<model::Link> link)
        {
            if(link)
            {
                const auto it = m_links.insert(m_links.end(), std::move(link));
                return it->ref();
            }
            
            return flip::Ref::null;
        }
        
        bool Patcher::removeObject(ID const& object_id)
        {
            auto obj_it = m_objects.find_if([&object_id](model::Object const& obj)
            {
                return obj.ref() == object_id;
            });
            
            if(obj_it != m_objects.end())
            {
                // first remove links connected to this object
                for(auto link_it = m_links.begin(); link_it != m_links.end();)
                {
                    if(link_it->getSourceId() == object_id
                       || link_it->getDestinationId() == object_id)
                    {
                        link_it = m_links.erase(link_it);
                    }
                    else
                    {
                        ++link_it;
                    }
                }
                
                m_objects.erase(obj_it);
                return true;
            }
            
            return false;
        }
        
        bool Patcher::removeLink(ID const& link_id)
        {
            auto link_it = m_links.find_if([&link_id](model::Link const& link)
            {
                return link.ref() == link_id;
            });

            if(link_it != m_links.end())
            {
                m_links.erase(link_it);
                return true;
            }
            
            return false;
        }
    }
}
