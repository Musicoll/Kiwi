/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiEngine_Patcher.h"
#include "KiwiEngine_Object.h"
#include "KiwiEngine_Link.h"
#include "KiwiEngine_Factory.h"
#include "KiwiEngine_Instance.h"
#include <KiwiTool/KiwiTool_Scheduler.h>

#include <KiwiModel/KiwiModel_PatcherUser.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        Patcher::Patcher(Instance& instance, model::Patcher & patcher_model) noexcept :
        m_instance(instance),
        m_objects(),
        m_so_links(1),
        m_chain(),
        m_patcher_model(patcher_model)
        {
            m_instance.getAudioControler().add(m_chain);
        }
        
        Patcher::~Patcher()
        {
            m_instance.getAudioControler().remove(m_chain);
        }
        
        model::Patcher & Patcher::getPatcherModel()
        {
            return m_patcher_model;
        }
        
        void Patcher::addObject(uint64_t object_id, std::shared_ptr<Object> object)
        {
            m_objects[object_id] = object;
            
            std::shared_ptr<dsp::Processor> processor = std::dynamic_pointer_cast<AudioObject>(object);
            
            if(processor)
            {
                m_chain.addProcessor(std::move(processor));
            }
        }
        
        void Patcher::removeObject(uint64_t object_id)
        {
            auto processor = std::dynamic_pointer_cast<AudioObject>(m_objects[object_id]);
            
            if (processor)
            {
                m_chain.removeProcessor(*processor);
            }
            
            m_objects.erase(object_id);
        }
        
        void Patcher::addLink(uint64_t from_id, size_t outlet, uint64_t to_id, size_t inlet, bool is_signal)
        {
            std::shared_ptr<Object> from = m_objects[from_id];
            std::shared_ptr<Object> to = m_objects[to_id];
            
            if(from && to)
            {
                if (!is_signal)
                {
                    from->addOutputLink(outlet, *to, inlet);
                }
                else
                {
                    std::shared_ptr<dsp::Processor> proc_from = std::dynamic_pointer_cast<AudioObject>(from);
                    std::shared_ptr<dsp::Processor> proc_to = std::dynamic_pointer_cast<AudioObject>(to);
                    
                    m_chain.connect(*proc_from, outlet,
                                    *proc_to, inlet);
                }
            }
        }
        
        void Patcher::removeLink(uint64_t from_id, size_t outlet, uint64_t to_id, size_t inlet, bool is_signal)
        {
            std::shared_ptr<Object> from = m_objects[from_id];
            std::shared_ptr<Object> to = m_objects[to_id];
            
            if (!is_signal)
            {
                from->removeOutputLink(outlet, *to, inlet);
            }
            else
            {
                std::shared_ptr<dsp::Processor> proc_from = std::dynamic_pointer_cast<AudioObject>(from);
                std::shared_ptr<dsp::Processor> proc_to = std::dynamic_pointer_cast<AudioObject>(to);
                
                m_chain.disconnect(*proc_from, outlet,
                                   *proc_to, inlet);
            }
        }
        
        void Patcher::updateChain()
        {
            try
            {
                m_chain.update();
            }
            catch (dsp::LoopError & e)
            {
                error(e.what());
            }
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
            for(auto& object : m_objects)
            {
                object.second->loadbang();
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
        //                                      SCHEDULER                                   //
        // ================================================================================ //
        
        tool::Scheduler<> & Patcher::getScheduler() const
        {
            return m_instance.getScheduler();
        }
        
        tool::Scheduler<> & Patcher::getMainScheduler() const
        {
            return m_instance.getMainScheduler();
        }
        
        // ================================================================================ //
        //                                      BEACON                                      //
        // ================================================================================ //
        
        tool::Beacon& Patcher::getBeacon(std::string const& name) const
        {
            return m_instance.getBeacon(name);
        }
        
        // ================================================================================ //
        //                                    MODEL CHANGED                                 //
        // ================================================================================ //
        
        void Patcher::modelChanged(model::Patcher const& model)
        {
            if(model.changed())
            {
                updateGraph(model);
                
                updateAttributes(model);
                
                updateChain();
            }
        }
        
        void Patcher::updateGraph(model::Patcher const& patcher_model)
        {
            std::set<model::Link const*> added_links;
            std::set<model::Link const*> removed_links;
            std::set<model::Object const*> added_objects;
            std::set<model::Object const*> removed_objects;
            
            if (patcher_model.linksChanged())
            {
                for (auto const& link : patcher_model.getLinks())
                {
                    if (link.added())
                    {
                        added_links.insert(&link);
                    }
                    if (link.removed())
                    {
                        removed_links.insert(&link);
                    }
                }
            }
            
            if (patcher_model.objectsChanged())
            {
                for(auto const & object : patcher_model.getObjects())
                {
                    if(object.added())
                    {
                        added_objects.insert(&object);
                    }
                    else if(object.removed())
                    {
                        removed_objects.insert(&object);
                    }
                }
            }
            
            if (!added_links.empty() || !removed_links.empty() || !added_objects.empty() || !removed_objects.empty())
            {
                std::unique_lock<std::mutex> lock(getScheduler().lock());
                
                for (auto link : removed_links)
                {
                    linkRemoved(*link);
                }
                
                for (auto object : added_objects)
                {
                    objectAdded(*object);
                }
                
                for (auto object : removed_objects)
                {
                    objectRemoved(*object);
                }
                
                for (auto link : added_links)
                {
                    linkAdded(*link);
                }
            }
        }
        
        void Patcher::updateAttributes(model::Patcher const& patcher_model)
        {
            if (patcher_model.objectsChanged())
            {
                for(auto const & object : patcher_model.getObjects())
                {
                    if (!object.removed() && !object.added())
                    {
                        std::set<std::string> changed_params = object.getChangedAttributes();
                        
                        for (std::string const& param_name : changed_params)
                        {
                            m_objects.at(object.ref().obj())->modelAttributeChanged(param_name,
                                                                                    object.getAttribute(param_name));
                        }
                    }
                }
            }
        }

        void Patcher::objectAdded(model::Object const& object_m)
        {
            addObject(object_m.ref().obj(), Factory::create(*this, object_m));
        }

        void Patcher::objectRemoved(model::Object const& object_m)
        {
            removeObject(object_m.ref().obj());
        }

        void Patcher::linkAdded(model::Link const& link_m)
        {
            addLink(link_m.getSenderObject().ref().obj(),
                    link_m.getSenderIndex(),
                    link_m.getReceiverObject().ref().obj(),
                    link_m.getReceiverIndex(),
                    link_m.isSignal());
        }
        
        void Patcher::linkRemoved(model::Link const& link_m)
        {
            removeLink(link_m.getSenderObject().ref().obj(),
                       link_m.getSenderIndex(),
                       link_m.getReceiverObject().ref().obj(),
                       link_m.getReceiverIndex(),
                       link_m.isSignal());
        }
    }
}
