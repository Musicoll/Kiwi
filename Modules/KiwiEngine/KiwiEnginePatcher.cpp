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

#include "KiwiEnginePatcher.hpp"
#include "KiwiEngineObject.hpp"
#include "KiwiEngineLink.hpp"
#include <KiwiModel/KiwiPatcher.hpp>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        Patcher::Patcher(model::Patcher const& model) noexcept :
        m_model(model)
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
            for(auto& obj : m_model.getObjects())
            {
                if(obj.resident())
                {
                    auto object_engine = obj.entity().use<std::shared_ptr<engine::Object>>();
                    objects.push_back(object_engine.get());
                }
            }
            
            return objects;
        }
        
        std::vector<engine::Object*> Patcher::getObjects()
        {
            std::vector<engine::Object*> objects;
            for(auto& obj : m_model.getObjects())
            {
                if(obj.resident())
                {
                    auto object_engine = obj.entity().use<std::shared_ptr<engine::Object>>();
                    objects.push_back(object_engine.get());
                }
            }
            
            return objects;
        }
        
        std::vector<engine::Link const*> Patcher::getLinks() const
        {
            std::vector<engine::Link const*> links;
            for(auto& link : m_model.getLinks())
            {
                if(link.resident())
                {
                    engine::Link const* link_engine = link.entity().get<Link>();
                    links.push_back(link_engine);
                }
            }
            
            return links;
        }
        
        // ================================================================================ //
        //                               DOCUMENT OBSERVER                                  //
        // ================================================================================ //
        
        void Patcher::document_changed(model::Patcher& patcher)
        {
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
                            
                            linkRemoved(link);
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
                                objectAdded(object);
                            }
                            
                            objectChanged(object);
                            
                            if(object.removed())
                            {
                                objectRemoved(object);
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
                                linkAdded(link);
                            }
                            
                            if(link.resident())
                            {
                                linkChanged(link);
                            }
                        }
                    }
                }
            }
        }

        void Patcher::objectAdded(model::Object& object_m)
        {
            std::shared_ptr<engine::Object> obj_sptr = ObjectFactory::createEngine<engine::Object>(object_m);
            object_m.entity().emplace<std::shared_ptr<engine::Object>>(obj_sptr);
        }

        void Patcher::objectChanged(model::Object& object_m)
        {
        }

        void Patcher::objectRemoved(model::Object& object_m)
        {
            object_m.entity().erase<std::shared_ptr<engine::Object>>();
        }

        void Patcher::linkAdded(model::Link& link_m)
        {
            auto& sender_entity = link_m.getSenderObject().entity();
            auto& receiver_entity = link_m.getReceiverObject().entity();
            
            assert(sender_entity.has<std::shared_ptr<engine::Object>>());
            assert(receiver_entity.has<std::shared_ptr<engine::Object>>());
            
            auto from = sender_entity.use<std::shared_ptr<engine::Object>>();
            auto to = receiver_entity.use<std::shared_ptr<engine::Object>>();
            
            if(from && to)
            {
                auto& link_e = link_m.entity().emplace<engine::Link>(link_m);
                from->addOutputLink(&link_e);
            }
        }
        
        void Patcher::linkChanged(model::Link& link_m)
        {
            ;
        }
        
        void Patcher::linkRemoved(model::Link& link)
        {
            auto& sender_entity = link.getSenderObject().entity();
            
            if(sender_entity.has<std::shared_ptr<engine::Object>>())
            {
                auto& link_engine = link.entity().use<engine::Link>();
                auto from = sender_entity.use<std::shared_ptr<engine::Object>>();
                from->removeOutputLink(&link_engine);
            }
            
            link.entity().erase<Link>();
        }
    }
}
