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
#include "KiwiEngine_Scheduler.h"

#include <KiwiModel/KiwiModel_PatcherUser.hpp>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        Patcher::Patcher(Instance& instance) noexcept :
        m_instance(instance),
        m_objects(),
        m_callbacks(),
        m_mutex(),
        m_so_links(1),
        m_chain()
        {
            m_instance.getAudioControler().add(m_chain);
        }
        
        Patcher::~Patcher()
        {
            m_instance.getAudioControler().remove(m_chain);
        }
        
        void Patcher::addObject(uint64_t object_id, std::shared_ptr<Object> object)
        {
            m_objects[object_id] = object;
            
            std::shared_ptr<dsp::Processor> processor = std::dynamic_pointer_cast<AudioObject>(object);
            
            if (processor)
            {
                m_chain.addProcessor(processor);
            }
        }
        
        void Patcher::removeObject(uint64_t object_id)
        {
            std::shared_ptr<dsp::Processor> processor =
                std::dynamic_pointer_cast<AudioObject>(m_objects[object_id]);
            
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
        
        void Patcher::disableCommands()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            
            for(auto & callback : m_callbacks)
            {
                callback->disable();
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
        
        Beacon& Patcher::getBeacon(std::string const& name) const
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
                const bool link_changed = model.linksChanged();
                
                // check links before objects
                if(link_changed)
                {
                    for(auto& link : model.getLinks())
                    {
                        if(link.removed())
                        {
                            linkRemoved(link);
                        }
                    }
                }

                if(model.objectsChanged())
                {
                    for(auto& object : model.getObjects())
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
                    for(auto& link : model.getLinks())
                    {
                        if(link.added())
                        {
                            linkAdded(link);
                        }
                    }
                }
                
                Scheduler<>::getInstance().schedule(new Patcher::CallBack(*this,
                                                                          std::bind(&Patcher::updateChain, this)));
            }
        }

        void Patcher::objectAdded(model::Object& object_m)
        {
            std::shared_ptr<Object> object = Factory::create(*this, object_m);
            
            Scheduler<>::CallBack* callback =
                new Patcher::CallBack(*this, std::bind(&Patcher::addObject, this, object_m.ref().obj(), object));
            
            Scheduler<>::getInstance().schedule(callback);
        }

        void Patcher::objectRemoved(model::Object& object_m)
        {
            Scheduler<>::CallBack* callback =
                new Patcher::CallBack(*this, std::bind(&Patcher::removeObject, this, object_m.ref().obj()));
            
            Scheduler<>::getInstance().schedule(callback);
        }

        void Patcher::linkAdded(model::Link& link_m)
        {
            Scheduler<>::CallBack* callback =
                new Patcher::CallBack(*this, std::bind(&Patcher::addLink, this,
                                                       link_m.getSenderObject().ref().obj(),
                                                       link_m.getSenderIndex(),
                                                       link_m.getReceiverObject().ref().obj(),
                                                       link_m.getReceiverIndex(),
                                                       link_m.isSignal()));
            
            Scheduler<>::getInstance().schedule(callback);
        }
        
        void Patcher::linkRemoved(model::Link& link_m)
        {
            Scheduler<>::CallBack* callback =
                new Patcher::CallBack(*this, std::bind(&Patcher::removeLink, this,
                                                       link_m.getSenderObject().ref().obj(),
                                                       link_m.getSenderIndex(),
                                                       link_m.getReceiverObject().ref().obj(),
                                                       link_m.getReceiverIndex(),
                                                       link_m.isSignal()));
            
            Scheduler<>::getInstance().schedule(callback);
        }
        
        // ================================================================================ //
        //                                PATCHER CALLBACK                                  //
        // ================================================================================ //
        
        Patcher::CallBack::CallBack(Patcher& patcher, std::function<void(void)> callback):
        Scheduler<>::CallBack(Thread::Gui, Thread::Engine, callback),
        m_patcher(patcher)
        {
            std::lock_guard<std::mutex> lock(m_patcher.m_mutex);
            m_patcher.m_callbacks.insert(this);
        }
        
        Patcher::CallBack::~CallBack()
        {
            std::lock_guard<std::mutex> lock(m_patcher.m_mutex);
            m_patcher.m_callbacks.erase(this);
        }
    }
}
