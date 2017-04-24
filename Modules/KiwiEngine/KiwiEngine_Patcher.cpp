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
        
        Patcher::Patcher(Instance& instance) noexcept :
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
            std::shared_ptr<Object> object = std::move(Factory::create(*this, object_m));
            
            m_objects[object_m.ref().obj()] = object;
            
            std::shared_ptr<dsp::Processor> processor = std::dynamic_pointer_cast<AudioObject>(object);
            
            if (processor)
            {
                m_chain.addProcessor(processor);
            }
        }

        void Patcher::objectRemoved(model::Object& object_m)
        {   
            std::shared_ptr<dsp::Processor> processor =
                std::dynamic_pointer_cast<AudioObject>(m_objects[object_m.ref().obj()]);
            
            if (processor)
            {
                m_chain.removeProcessor(*processor);
            }
            
            m_objects.erase(object_m.ref().obj());
        }

        void Patcher::linkAdded(model::Link& link_m)
        {
            std::shared_ptr<Object> from = m_objects[link_m.getSenderObject().ref().obj()];
            std::shared_ptr<Object> to = m_objects[link_m.getReceiverObject().ref().obj()];
            
            if(from && to)
            {
                if (!link_m.isSignal())
                {
                    from->addOutputLink(link_m.getSenderIndex(), *to, link_m.getReceiverIndex());
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
            std::shared_ptr<Object> from = m_objects[link.getSenderObject().ref().obj()];
            std::shared_ptr<Object> to = m_objects[link.getReceiverObject().ref().obj()];
            
            if (!link.isSignal())
            {
                from->removeOutputLink(link.getSenderIndex(), *to, link.getReceiverIndex());
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
