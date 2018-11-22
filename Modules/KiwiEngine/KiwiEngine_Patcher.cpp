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

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                      PATCHER                                     //
    // ================================================================================ //
    
    Patcher::Patcher(Instance& instance, model::Patcher& patcher_model) noexcept
    : m_instance(instance)
    , m_objects()
    , m_so_links(1)
    , m_chain()
    , m_patcher_model(patcher_model)
    {
        m_instance.getAudioControler().add(m_chain);
    }
    
    Patcher::~Patcher()
    {
        m_instance.getAudioControler().remove(m_chain);
    }
    
    model::Patcher& Patcher::getPatcherModel()
    {
        return m_patcher_model;
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
            
            if(m_dsp_chain_need_update)
            {
                updateChain();
                m_dsp_chain_need_update = false;
            }
        }
    }
    
    void Patcher::updateGraph(model::Patcher const& patcher_model)
    {
        std::vector<model::Link const*> added_links;
        std::vector<model::Link const*> removed_links;
        std::vector<model::Object const*> added_objects;
        std::vector<model::Object const*> removed_objects;
        
        if (patcher_model.linksChanged())
        {
            for (auto const& link : patcher_model.getLinks())
            {
                if (link.added())
                {
                    added_links.emplace_back(&link);
                }
                
                if (link.removed())
                {
                    removed_links.emplace_back(&link);
                }
            }
        }
        
        if (patcher_model.objectsChanged())
        {
            for(auto const & object : patcher_model.getObjects())
            {
                if(object.added())
                {
                    added_objects.emplace_back(&object);
                }
                else if(object.removed())
                {
                    removed_objects.emplace_back(&object);
                }
            }
        }
        
        const bool has_changed = (!added_links.empty() || !removed_links.empty()
                                  || !added_objects.empty() || !removed_objects.empty());
        
        if (has_changed)
        {
            std::unique_lock<std::mutex> lock(getScheduler().lock());
            
            for (auto link : removed_links)
            {
                linkRemoved(*link);
            }
            
            for (auto object : removed_objects)
            {
                objectRemoved(*object);
            }
            
            for (auto object : added_objects)
            {
                objectAdded(*object);
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
            for(auto const& object : patcher_model.getObjects())
            {
                if (!object.removed() && !object.added())
                {
                    std::set<std::string> changed_params = object.getChangedAttributes();
                    
                    for (std::string const& param_name : changed_params)
                    {
                        m_objects.at(object.ref())->modelAttributeChanged(param_name,
                                                                          object.getAttribute(param_name));
                    }
                }
            }
        }
    }
    
    void Patcher::objectAdded(model::Object const& object_m)
    {
        auto it = m_objects.emplace(std::make_pair(object_m.ref(),
                                                   Factory::create(*this, object_m)));
        if(it.second)
        {
            auto object = it.first->second;
            if(auto processor = std::dynamic_pointer_cast<AudioObject>(object))
            {
                m_chain.addProcessor(std::move(processor));
                m_dsp_chain_need_update = true;
            }
        }
    }
    
    void Patcher::objectRemoved(model::Object const& object_m)
    {
        auto it = m_objects.find(object_m.ref());
        
        // (for now) all model object must be reflected in the engine graph
        assert(it != m_objects.end());
        
        if(it == m_objects.end())
            return; // abort
        
        if (auto processor = std::dynamic_pointer_cast<AudioObject>(it->second))
        {
            m_chain.removeProcessor(*processor);
            m_dsp_chain_need_update = true;
        }
        
        m_objects.erase(it);
    }
    
    void Patcher::linkAdded(model::Link const& link_m)
    {
        auto from = m_objects[link_m.getSenderObject().ref()];
        auto to = m_objects[link_m.getReceiverObject().ref()];
        
        assert(from && to);
        
        if(!from || !to)
            return; // abort
        
        const auto outlet = link_m.getSenderIndex();
        const auto inlet = link_m.getReceiverIndex();
        
        if (!link_m.isSignal())
        {
            from->addOutputLink(outlet, *to, inlet);
        }
        else
        {
            auto proc_from = std::dynamic_pointer_cast<AudioObject>(from);
            auto proc_to = std::dynamic_pointer_cast<AudioObject>(to);
            
            m_chain.connect(*proc_from, outlet, *proc_to, inlet);
            m_dsp_chain_need_update = true;
        }
    }
    
    void Patcher::linkRemoved(model::Link const& link_m)
    {
        auto from = m_objects[link_m.getSenderObject().ref()];
        auto to = m_objects[link_m.getReceiverObject().ref()];
        
        assert(from && to);
        
        if(!from || !to)
            return; // abort
        
        const auto outlet = link_m.getSenderIndex();
        const auto inlet = link_m.getReceiverIndex();
        
        if (!link_m.isSignal())
        {
            from->removeOutputLink(outlet, *to, inlet);
        }
        else
        {
            auto proc_from = std::dynamic_pointer_cast<AudioObject>(from);
            auto proc_to = std::dynamic_pointer_cast<AudioObject>(to);
            
            assert(proc_from && proc_to);
            
            m_chain.disconnect(*proc_from, outlet, *proc_to, inlet);
            m_dsp_chain_need_update = true;
        }
    }
    
}}
