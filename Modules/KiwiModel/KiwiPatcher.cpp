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
        
        model::Object& Patcher::addPlus()
        {
            assert(ObjectFactory::has("plus"));
            
            std::vector<Atom> args {42};
            return *m_objects.insert(m_objects.end(), ObjectFactory::create("plus", args));
            
            //return *m_objects.insert(m_objects.end(), std::unique_ptr<model::ObjectPlus>(new model::ObjectPlus()));
        }
        
        model::Object& Patcher::addPrint()
        {
            return *m_objects.insert(m_objects.end(), std::unique_ptr<model::ObjectPrint>(new model::ObjectPrint()));
        }
        
        bool Patcher::canConnect(model::Object const& from, const uint32_t outlet,
                                 model::Object const& to, const uint32_t inlet) const
        {
            // check source object
            const auto from_it = findObject(from);
            const bool from_valid = (from_it != m_objects.cend() && from_it->getNumberOfOutlets() > outlet);
            
            // check destination object
            const auto to_it = findObject(to);
            const bool to_valid = (to_it != m_objects.cend() && to_it->getNumberOfInlets() > inlet);
            
            
            if(from_valid && to_valid)
            {
                // Check if link does not exists.
                const auto find_link = [&from, &outlet, &to, &inlet](model::Link const& link_model)
                {
                    return (link_model.getSenderObject().ref()      == from.ref() &&
                            link_model.getReceiverObject().ref()    == to.ref() &&
                            link_model.getSenderIndex()         == outlet &&
                            link_model.getReceiverIndex()       == inlet);
                };
                
                return (std::find_if(m_links.begin(), m_links.end(), find_link) == m_links.cend());
            }
            
            return false;
        }
        
        void Patcher::addLink(model::Object const& from, const uint32_t outlet, model::Object const& to, const uint32_t inlet)
        {
            if(canConnect(from, outlet, to, inlet))
            {
                m_links.insert(m_links.end(), std::unique_ptr<model::Link>(new model::Link(from, outlet, to, inlet)));
            }
        }
        
        void Patcher::removeObject(model::Object const& object)
        {
            auto obj_it = findObject(object);
            if(obj_it != m_objects.end())
            {
                // first remove links connected to this object
                for(auto link_it = m_links.begin(); link_it != m_links.end();)
                {
                    if(link_it->getSenderObject().ref() == object.ref()
                       || link_it->getReceiverObject().ref() == object.ref())
                    {
                        link_it = m_links.erase(link_it);
                    }
                    else
                    {
                        ++link_it;
                    }
                }
                
                m_objects.erase(obj_it);
            }
        }
        
        void Patcher::removeLink(model::Link const& link)
        {
            const auto link_it = findLink(link);
            if(link_it != m_links.end())
            {
                m_links.erase(link_it);
            }
        }
        
        flip::Array<model::Object>::const_iterator Patcher::findObject(model::Object const& object) const
        {
            const auto find_it = [&object](model::Object const& object_model)
            {
                return (object.ref() == object_model.ref());
            };
            
            return std::find_if(m_objects.begin(), m_objects.end(), find_it);
        }
        
        flip::Array<model::Link>::const_iterator Patcher::findLink(model::Link const& link) const
        {
            const auto find_it = [&link](model::Link const& link_model)
            {
                return (link.ref() == link_model.ref());
            };
            
            return std::find_if(m_links.begin(), m_links.end(), find_it);
        }
        
        flip::Array<model::Object>::iterator Patcher::findObject(model::Object const& object)
        {
            const auto find_it = [&object](model::Object const& object_model)
            {
                return (object.ref() == object_model.ref());
            };
            
            return std::find_if(m_objects.begin(), m_objects.end(), find_it);
        }
        
        flip::Array<model::Link>::iterator Patcher::findLink(model::Link const& link)
        {
            const auto find_it = [&link](model::Link const& link_model)
            {
                return (link.ref() == link_model.ref());
            };
            
            return std::find_if(m_links.begin(), m_links.end(), find_it);
        }
    }
}
