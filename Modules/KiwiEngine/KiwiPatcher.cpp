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
        
        std::vector<engine::Object const*> Patcher::getObjects() const
        {
            std::vector<engine::Object const*> objects;
            for(auto& obj : m_model->getObjects())
            {
                if(obj.resident())
                {
                    auto object_engine = obj.entity().use<sObject>();
                    objects.push_back(object_engine.get());
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
                    auto object_engine = obj.entity().use<sObject>();
                    objects.push_back(object_engine.get());
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
                            linkChanged(link);
                            
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
                            
                            objectChanged(object);
                            
                            if(object.removed())
                            {
                                objectWillBeRemoved(object);
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
                            
                            if(link.resident())
                            {
                                linkHasBeenAdded(link);
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

        void Patcher::objectHasBeenAdded(model::Object& object_m)
        {
            std::vector<Atom> args{42};
            
            sObject obj_sptr = object_m.entity().emplace<sObject>(model::ObjectFactory::createEngine<engine::Object>(object_m.getName(), args));
        }

        void Patcher::objectChanged(model::Object& object_m)
        {
            sObject object_e = object_m.entity().use<sObject>();
            object_e->modelChanged(object_m);
        }

        void Patcher::objectWillBeRemoved(model::Object& object)
        {
            object.entity().erase<sObject>();
        }

        void Patcher::linkHasBeenAdded(model::Link& link)
        {
            auto& sender_entity = link.getSenderObject().entity();
            auto& receiver_entity = link.getReceiverObject().entity();
            
            assert(sender_entity.has<sObject>());
            assert(receiver_entity.has<sObject>());
            
            auto from = sender_entity.use<sObject>();
            auto to = receiver_entity.use<sObject>();
            
            if(from && to)
            {
                auto& link_engine = link.entity().emplace<Link>(link, *from, *to);
                from->addOutputLink(&link_engine);
            }
        }
        
        void Patcher::linkChanged(model::Link& link_m)
        {
            auto& link_e = link_m.entity().use<engine::Link>();
            link_e.modelChanged(link_m);
        }
        
        void Patcher::linkWillBeRemoved(model::Link& link)
        {
            auto& sender_entity = link.getSenderObject().entity();
            
            if(sender_entity.has<sObject>())
            {
                auto& link_engine = link.entity().use<Link>();
                auto from = sender_entity.use<sObject>();
                from->removeOutputLink(&link_engine);
            }
            
            link.entity().erase<Link>();
        }
    }
}
