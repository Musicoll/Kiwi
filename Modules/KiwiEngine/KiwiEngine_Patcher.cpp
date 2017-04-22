/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiEngine_Patcher.hpp"
#include "KiwiEngine_Object.hpp"
#include "KiwiEngine_Link.hpp"
#include "KiwiEngine_Factory.hpp"
#include "KiwiEngine_Instance.hpp"

#include <KiwiModel/KiwiModel_PatcherUser.hpp>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        Patcher::Patcher(model::Patcher const& model, Instance& instance) noexcept :
        m_model(model),
        m_instance(instance),
        m_so_links(1),
        m_chain()
        {
            m_instance.getAudioControler().add(m_chain);
        }
        
        Patcher::~Patcher()
        {
            m_instance.getAudioControler().remove(m_chain);
        }
        
        std::vector<Object const*> Patcher::getObjects() const
        {
            std::vector<Object const*> objects;
            for(auto& obj : m_model.getObjects())
            {
                if(obj.resident())
                {
                    auto object_engine = obj.entity().use<std::shared_ptr<Object>>();
                    objects.push_back(object_engine.get());
                }
            }
            
            return objects;
        }
        
        std::vector<Object*> Patcher::getObjects()
        {
            std::vector<Object*> objects;
            for(auto& obj : m_model.getObjects())
            {
                if(obj.resident())
                {
                    auto object_engine = obj.entity().use<std::shared_ptr<Object>>();
                    objects.push_back(object_engine.get());
                }
            }
            
            return objects;
        }
        
        std::vector<Link const*> Patcher::getLinks() const
        {
            std::vector<Link const*> links;
            for(auto& link : m_model.getLinks())
            {
                if(link.resident())
                {
                    Link const* link_engine = link.entity().get<Link>();
                    links.push_back(link_engine);
                }
            }
            
            return links;
        }
        
        AudioControler& Patcher::getAudioControler() const
        {
            return m_instance.getAudioControler();
        }
        
        
        void Patcher::addStackOverflow(Link const& link)
        {
            m_so_links[m_so_links.size() - 1].push(&link);
        }
        
        void Patcher::endStackOverflow()
        {
            m_so_links.push_back(SoLinks());
        }
        
        std::vector<std::queue<Link const*>> Patcher::getStackOverflow() const
        {
            return m_so_links;
        }
        
        void Patcher::clearStackOverflow()
        {
            m_so_links.clear();
            m_so_links.push_back(SoLinks());
        }
        
        void Patcher::sendLoadbang()
        {
            for(auto& obj : m_model.getObjects())
            {
                obj.entity().use<std::shared_ptr<Object>>()->loadbang();
            }
        }
        
        // ================================================================================ //
        //                                      CONSOLE                                     //
        // ================================================================================ //
        
        void Patcher::log(std::string const& text) const
        {
            m_instance.log(text);
        }
        
        void Patcher::post(std::string const& text) const
        {
            m_instance.post(text);
        }
        
        void Patcher::warning(std::string const& text) const
        {
            m_instance.warning(text);
        }
        
        void Patcher::error(std::string const& text) const
        {
            m_instance.error(text);
        }
        
        // ================================================================================ //
        //                                      BEACON                                      //
        // ================================================================================ //
        
        Beacon& Patcher::useBeacon(std::string const& name) const
        {
            return m_instance.useBeacon(name);
        }
        
        ValueBeacon& Patcher::useValueBeacon(std::string const& name) const
        {
            return m_instance.useValueBeacon(name);
        }
        
        // ================================================================================ //
        //                                    MODEL CHANGED                                 //
        // ================================================================================ //
        
        void Patcher::modelChanged()
        {
            if(m_model.changed())
            {
                const bool link_changed = m_model.linksChanged();
                
                // check links before objects
                if(link_changed)
                {
                    for(auto& link : m_model.getLinks())
                    {
                        if(link.removed())
                        {
                            linkRemoved(link);
                        }
                    }
                }

                if(m_model.objectsChanged())
                {
                    for(auto& object : m_model.getObjects())
                    {
                        if(object.added())
                        {
                            objectAdded(object);
                        }
                        else if(object.removed())
                        {
                            objectRemoved(object);
                        }
                    }
                }
                
                // check links before objects
                if(link_changed)
                {
                    for(auto& link : m_model.getLinks())
                    {
                        if(link.added())
                        {
                            linkAdded(link);
                        }
                    }
                }
                
                try
                {
                    m_chain.update();
                }
                catch (dsp::LoopError & e)
                {
                    error(e.what());
                }
            }
        }

        void Patcher::objectAdded(model::Object& object_m)
        {
            std::shared_ptr<Object> obj_sptr = Factory::create(*this, object_m);
            object_m.entity().emplace<std::shared_ptr<Object>>(obj_sptr);
            
            std::shared_ptr<dsp::Processor> processor = std::dynamic_pointer_cast<AudioObject>(obj_sptr);
            
            if (processor)
            {
                m_chain.addProcessor(processor);
            }
        }

        void Patcher::objectRemoved(model::Object& object_m)
        {   
            std::shared_ptr<dsp::Processor> processor =
            std::dynamic_pointer_cast<AudioObject>(object_m.entity().use<std::shared_ptr<Object>>());
            
            if (processor)
            {
                m_chain.removeProcessor(*processor);
            }
            
            object_m.entity().erase<std::shared_ptr<Object>>();
        }

        void Patcher::linkAdded(model::Link& link_m)
        {
            auto& sender_entity = link_m.getSenderObject().entity();
            auto& receiver_entity = link_m.getReceiverObject().entity();
            
            assert(sender_entity.has<std::shared_ptr<Object>>());
            assert(receiver_entity.has<std::shared_ptr<Object>>());
            
            auto from = sender_entity.use<std::shared_ptr<Object>>();
            auto to = receiver_entity.use<std::shared_ptr<Object>>();
            
            if(from && to)
            {
                if (!link_m.isSignal())
                {
                    auto& link_e = link_m.entity().emplace<Link>(link_m);
                    from->addOutputLink(link_e);
                }
                else
                {
                    if (link_m.isSignal())
                    {
                        std::shared_ptr<dsp::Processor> proc_from = std::dynamic_pointer_cast<AudioObject>(from);
                        std::shared_ptr<dsp::Processor> proc_to = std::dynamic_pointer_cast<AudioObject>(to);
                        
                        m_chain.connect(*proc_from, link_m.getSenderIndex(),
                                        *proc_to, link_m.getReceiverIndex());
                    }
                }
            }
        }
        
        void Patcher::linkRemoved(model::Link& link)
        {
            auto& sender_entity = link.getSenderObject().entity();
            auto& receiver_entity = link.getReceiverObject().entity();
            
            assert(sender_entity.has<std::shared_ptr<Object>>());
            assert(receiver_entity.has<std::shared_ptr<Object>>());
            
            auto from = sender_entity.use<std::shared_ptr<Object>>();
            auto to = receiver_entity.use<std::shared_ptr<Object>>();
            
            if (!link.isSignal())
            {
                auto& link_engine = link.entity().use<Link>();
                auto from = sender_entity.use<std::shared_ptr<Object>>();
                from->removeOutputLink(link_engine);
                link.entity().erase<Link>();
            }
            else
            {
                std::shared_ptr<dsp::Processor> proc_from = std::dynamic_pointer_cast<AudioObject>(from);
                std::shared_ptr<dsp::Processor> proc_to = std::dynamic_pointer_cast<AudioObject>(to);
                
                m_chain.disconnect(*proc_from, link.getSenderIndex(),
                                   *proc_to, link.getReceiverIndex());
            }
        }
    }
}
