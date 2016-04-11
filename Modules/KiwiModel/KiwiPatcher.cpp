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
        
        ObjectId Patcher::addPlus()
        {
            return addObject(std::unique_ptr<model::ObjectPlus>(new model::ObjectPlus()));
        }
        
        ObjectId Patcher::addPrint()
        {
            return addObject(std::unique_ptr<model::ObjectPrint>(new model::ObjectPrint()));
        }
        
        ObjectId Patcher::addObject(std::unique_ptr<model::Object> object)
        {
            if(object)
            {
                const auto it = m_objects.insert(m_objects.end(), std::move(object));
                return it->getId();
            }
            
            return nullptr;
        }
        
        bool Patcher::canConnect(ObjectId const& from, const uint32_t outlet,
                                 ObjectId const& to, const uint32_t inlet) const
        {
            // check source object
            const auto p_from = [&from](model::Object const& object_model)
            {
                return (from == object_model.getId());
            };
            
            const auto from_it = std::find_if(m_objects.begin(), m_objects.end(), p_from);
            
            const bool from_valid = (from_it != m_objects.cend() && from_it->getNumberOfOutlets() > outlet);
            

            // check destination object
            const auto p_to = [&to](model::Object const& object_model)
            {
                return (to == object_model.getId());
            };
            
            const auto to_it = std::find_if(m_objects.begin(), m_objects.end(), p_to);
            
            const bool to_valid = (to_it != m_objects.cend() && to_it->getNumberOfInlets() > inlet);
            
            
            if(from_valid && to_valid)
            {
                // Check if link does not exists.
                const auto find_link = [&from, &outlet, &to, &inlet](model::Link const& link_model)
                {
                    return (link_model.getSenderId()      == from &&
                            link_model.getReceiverId()    == to &&
                            link_model.getSenderIndex()   == outlet &&
                            link_model.getReceiverIndex() == inlet);
                };
                
                return (std::find_if(m_links.begin(), m_links.end(), find_link) == m_links.cend());
            }
            
            return false;
        }
        
        LinkId Patcher::addLink(ObjectId const& from, const uint32_t outlet, ObjectId const& to, const uint32_t inlet)
        {
            if(canConnect(from, outlet, to, inlet))
            {
                return addLink(std::unique_ptr<model::Link>(new model::Link(from, outlet, to, inlet)));
            }
            
            return nullptr;
        }
        
        LinkId Patcher::addLink(std::unique_ptr<model::Link> link)
        {
            if(link)
            {
                const auto it = m_links.insert(m_links.end(), std::move(link));
                return it->getId();
            }
            
            return nullptr;
        }
        
        bool Patcher::removeObject(ObjectId const& object_id)
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
                    if(link_it->getSenderId() == object_id
                       || link_it->getReceiverId() == object_id)
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
        
        bool Patcher::removeLink(LinkId const& link_id)
        {
            auto link_it = m_links.find_if([&link_id](model::Link const& link)
            {
                return link_id == link.ref();
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
