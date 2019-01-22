/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiEngine_Object.h"
#include "KiwiEngine_Link.h"
#include "KiwiEngine_Patcher.h"

#include <KiwiModel/KiwiModel_DocumentManager.h>
#include <KiwiModel/KiwiModel_Object.h>

namespace kiwi
{
    namespace engine
    {
        
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        Object::Object(model::Object const& model, Patcher& patcher) noexcept
        : m_patcher(patcher)
        , m_ref(model.ref())
        , m_inlets(model.getNumberOfInlets())
        , m_outlets(model.getNumberOfOutlets())
        , m_master(this, [](engine::Object*){})
        {
            getObjectModel().addListener(*this);
        }
        
        Object::~Object() noexcept
        {
            getObjectModel().removeListener(*this);
        }
        
        void Object::addOutputLink(flip::Ref link_ref, size_t outlet_index, Object & receiver, size_t inlet_index)
        {
            m_outlets[outlet_index].emplace(std::make_pair(link_ref, Link {receiver, inlet_index}));
        }
        
        void Object::removeOutputLink(flip::Ref link_ref, size_t outlet_index)
        {
            m_outlets[outlet_index].erase(link_ref);
        }
        
        // ================================================================================ //
        //                                      PARMETERS                                   //
        // ================================================================================ //
        
        void Object::modelParameterChanged(std::string const& name, tool::Parameter const& parameter)
        {
            defer([this, name, parameter]() {
                parameterChanged(name, parameter);
            });
        }
        
        void Object::modelAttributeChanged(std::string const& name, tool::Parameter const& parameter)
        {
            defer([this, name, parameter]() {
                attributeChanged(name, parameter);
            });
        }
        
        model::Object& Object::getObjectModel()
        {
            return m_patcher.getPatcherModel().document().object<model::Object>(m_ref);
        }
        
        void Object::parameterChanged(std::string const& param_name, tool::Parameter const& param)
        {}
        
        void Object::attributeChanged(std::string const& name, tool::Parameter const& attribute)
        {}
        
        void Object::setAttribute(std::string const& name, tool::Parameter && param)
        {
            deferMain([this, name, param = std::move(param)]() {
                
                auto& object_model = getObjectModel();
                object_model.setAttribute(name, param);                
                model::DocumentManager::commit(object_model);
                
            });
        }
        
        void Object::setParameter(std::string const& name, tool::Parameter && param)
        {
            deferMain([this, name, param = std::move(param)]() {
                getObjectModel().setParameter(name, param);
            });
        }
        
        // ================================================================================ //
        //                                      CONSOLE                                     //
        // ================================================================================ //
        
        void Object::log(std::string const& text) const
        {
            m_patcher.log(text);
        }
        
        void Object::post(std::string const& text) const
        {
            m_patcher.post(text);
        }
        
        void Object::warning(std::string const& text) const
        {
            m_patcher.warning(text);
        }
        
        void Object::error(std::string const& text) const
        {
            m_patcher.error(text);
        }
        
        // ================================================================================ //
        //                                      SCHEDULER                                   //
        // ================================================================================ //
        
        tool::Scheduler<> & Object::getScheduler() const
        {
            return m_patcher.getScheduler();
        }
        
        tool::Scheduler<> & Object::getMainScheduler() const
        {
            return m_patcher.getMainScheduler();
        }
        
        void Object::defer(std::function<void()> call_back)
        {
            std::weak_ptr<engine::Object> object(m_master);
            
            getScheduler().defer([object, cb = std::move(call_back)]() {
                if (!object.expired())
                {
                    cb();
                }
            });
        }
        
        void Object::deferMain(std::function<void()> call_back)
        {
            std::weak_ptr<engine::Object> object(m_master);
            
            getMainScheduler().defer([object, cb = std::move(call_back)]() {
                if (!object.expired())
                {
                    cb();
                }
            });
        }
        
        void Object::schedule(std::function<void()> call_back, tool::Scheduler<>::duration_t delay)
        {
            std::weak_ptr<engine::Object> object(m_master);
            
            getScheduler().schedule([object, cb = std::move(call_back)]() {
                if (!object.expired())
                {
                    cb();
                }
            }, delay);
        }
        
        void Object::scheduleMain(std::function<void()> call_back, tool::Scheduler<>::duration_t delay)
        {
            std::weak_ptr<engine::Object> object(m_master);
            
            getMainScheduler().schedule([object, cb = std::move(call_back)]() {
                if (!object.expired())
                {
                    cb();
                }
            }, delay);
        }
        
        // ================================================================================ //
        //                                      BEACON                                      //
        // ================================================================================ //
        
        tool::Beacon& Object::getBeacon(std::string const& name) const
        {
            return m_patcher.getBeacon(name);
        }
        
        void Object::send(const size_t index, std::vector<tool::Atom> const& args)
        {
            assert(getScheduler().isThisConsumerThread());
            
            // all message are disabled when the patcher has a stack overflow
            if(m_patcher.stackOverflowDetected())
                return;
            
            if(index >= m_outlets.size())
                return;
            
            for(auto& link_it : m_outlets[index])
            {
                auto& link = link_it.second;
                
                ++link.useStackCount();
                
                if(link.useStackCount() >= m_stack_overflow_max)
                {
                    auto link_ref = link_it.first;
                    m_patcher.signalStackOverflow(link_ref);
                    
                    error("infinite message loop detected, messages are disabled until you fix it...");
                    return;
                }
                
                Object& receiver = link.getReceiver();
                receiver.receive(link.getReceiverIndex(), args);
                
                --link.useStackCount();
            }
        }
        
        void Object::clearStackOverflow()
        {
            for(auto& outlet : m_outlets)
            {
                for(auto& link_it : outlet)
                {
                    auto& link = link_it.second;
                    link.useStackCount() = 0;
                }
            }
        }
        
        // ================================================================================ //
        //                                    AUDIOOBJECT                                   //
        // ================================================================================ //
        
        AudioObject::AudioObject(model::Object const& model, Patcher& patcher) noexcept
        : Object(model, patcher)
        , dsp::Processor(model.getNumberOfInlets(), model.getNumberOfOutlets())
        {}
        
    }
}


