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
#include "KiwiInstance.hpp"
#include "Objects/KiwiObjects.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        Patcher::Patcher(Instance& instance) noexcept : m_instance(instance)
        {
            ;
        }
        
        Patcher::~Patcher()
        {
            ;
        }
        
        void Patcher::addPlus()
        {
            m_model->addPlus();
        }
        
        void Patcher::addPrint()
        {
            m_model->addPrint();
        }
        
        void Patcher::addLink(Object const& from, const uint32_t outlet, Object const& to, const uint32_t inlet)
        {
            m_model->addLink(from.m_model, outlet, to.m_model, inlet);
        }
        
        void Patcher::removeObject(Object const& object)
        {
            m_model->removeObject(object.m_model);
        }
        
        void Patcher::removeLink(Link const& link)
        {
            m_model->removeLink(link.m_model);
        }
        
        std::vector<engine::Object const*> Patcher::getObjects() const
        {
            std::vector<engine::Object const*> objects;
            for(auto& obj : m_model->getObjects())
            {
                if(obj.resident())
                {
                    engine::Object const* object_engine = obj.entity().use<engine::Object*>();
                    objects.push_back(object_engine);
                }
            }
            
            return objects;
        }
        
        std::vector<engine::Object*> Patcher::getObjects()
        {
            std::vector<engine::Object*> objects;
            for(auto& obj : m_model->getObjects())
            {
                if(obj.resident())
                {
                    engine::Object* object_engine = obj.entity().use<engine::Object*>();
                    objects.push_back(object_engine);
                }
            }
            
            return objects;
        }
        
        std::vector<engine::Link const*> Patcher::getLinks() const
        {
            std::vector<engine::Link const*> links;
            for(auto& link : m_model->getLinks())
            {
                if(link.resident())
                {
                    engine::Link const* link_engine = link.entity().get<Link>();
                    links.push_back(link_engine);
                }
            }
            
            return links;
        }
        
        void Patcher::sendToObject(Object& object, const uint32_t inlet, std::vector<Atom> args)
        {
            object.receive(inlet, args);
        }
        
        // ================================================================================ //
        //                               DOCUMENT OBSERVER                                  //
        // ================================================================================ //
        
        void Patcher::document_changed(model::Patcher& patcher)
        {
            if(patcher.added())
            {
                m_model = &patcher;
            }
            
            if(patcher.changed())
            {
                const bool link_changed = patcher.linksChanged();
                
                // check links before objects
                if(link_changed)
                {
                    for(auto& link : patcher.getLinks())
                    {
                        if(link.changed() && link.removed())
                        {
                            linkWillBeRemoved(link);
                        }
                    }
                }

                if(patcher.objectsChanged())
                {
                    for(auto& object : patcher.getObjects())
                    {
                        if(object.changed())
                        {
                            if(object.added())
                            {
                                objectHasBeenAdded(object);
                            }
                            else if(object.removed())
                            {
                                objectWillBeRemoved(object);
                            }
                            else // resident
                            {
                                objectChanged(object);
                            }
                        }
                    }
                }
                
                // check links before objects
                if(link_changed)
                {
                    for(auto& link : patcher.getLinks())
                    {
                        if(link.changed())
                        {
                            if(link.added())
                            {
                                linkHasBeenAdded(link);
                            }
                            else if(link.resident())
                            {
                                linkChanged(link);
                            }
                        }
                    }
                }
            }
            
            if(patcher.removed())
            {
                m_model = nullptr;
            }
        }

        void Patcher::objectHasBeenAdded(model::Object& object)
        {
            const auto name = object.getName();
            
            engine::Object* object_engine_ptr = nullptr;
            
            if(name == "plus")
            {
                object_engine_ptr = &object.entity().emplace<engine::ObjectPlus>(static_cast<model::ObjectPlus&>(object));
            }
            else if(name == "print")
            {
                object_engine_ptr = &object.entity().emplace<engine::ObjectPrint>(static_cast<model::ObjectPrint&>(object));
            }
            
            if(object_engine_ptr)
            {
                object.entity().emplace<engine::Object*>(object_engine_ptr);
            }
            else
            {
                assert(false && "Object engine creation fail");
            }
        }

        void Patcher::objectChanged(model::Object& /*object*/)
        {
            
        }

        void Patcher::objectWillBeRemoved(model::Object& object)
        {
            object.entity().erase<engine::Object*>();
            
            const auto name = object.getName();
            
            if(name == "plus")
            {
                object.entity().erase<engine::ObjectPlus>();
            }
            else if(name == "print")
            {
                object.entity().erase<engine::ObjectPrint>();
            }
            else
            {
                assert(false && "Object engine destruction fail");
                return;
            }
        }

        void Patcher::linkHasBeenAdded(model::Link& link)
        {
            engine::Object* from = link.getSenderObject().entity().use<engine::Object*>();
            engine::Object* to = link.getReceiverObject().entity().use<engine::Object*>();
            
            if(from && to)
            {
                auto& link_engine = link.entity().emplace<Link>(link, *from, *to);
                from->addOutputLink(&link_engine);
            }
        }
        
        void Patcher::linkChanged(model::Link& /*link*/)
        {
            ;
        }
        
        void Patcher::linkWillBeRemoved(model::Link& link)
        {
            auto& link_engine = link.entity().use<Link>();
            engine::Object* from = link.getSenderObject().entity().use<engine::Object*>();
            from->removeOutputLink(&link_engine);
            link.entity().erase<Link>();
        }
    }
}
