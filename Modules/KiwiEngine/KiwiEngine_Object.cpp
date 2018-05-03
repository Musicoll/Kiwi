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
        
        Object::Object(model::Object const& model, Patcher& patcher) noexcept :
        m_patcher(patcher),
        m_ref(model.ref()),
        m_inlets(model.getNumberOfInlets()),
        m_outlets(model.getNumberOfOutlets()),
        m_stack_count(0ul),
        m_master(this, [](engine::Object*){})
        {
            getObjectModel().addListener(*this);
        }
        
        Object::~Object() noexcept
        {
            getObjectModel().removeListener(*this);
        }
        
        void Object::addOutputLink(size_t outlet_index, Object & receiver, size_t inlet_index)
        {
            m_outlets[outlet_index].insert(Link(receiver, inlet_index));
        }
        
        void Object::removeOutputLink(size_t outlet_index, Object & receiver, size_t inlet_index)
        {
            m_outlets[outlet_index].erase(Link(receiver, inlet_index));
        }
        
        // ================================================================================ //
        //                                      PARMETERS                                   //
        // ================================================================================ //
        
        void Object::modelParameterChanged(std::string const& name, tool::Parameter const& parameter)
        {
            defer([this, name, parameter]()
            {
                parameterChanged(name, parameter);
            });
        }
        
        void Object::modelAttributeChanged(std::string const& name, tool::Parameter const& parameter)
        {
            defer([this, name, parameter]()
            {
                attributeChanged(name, parameter);
            });
        }
        
        model::Object & Object::getObjectModel()
        {
            flip::Array<model::Object> & objects = m_patcher.getPatcherModel().getObjects();
    
            return *std::find_if(objects.begin(), objects.end(), [this](model::Object const & object_model)
            {
                return object_model.ref() == m_ref;
            });
        }
        
        void Object::parameterChanged(std::string const& param_name, tool::Parameter const& param)
        {
        }
        
        void Object::attributeChanged(std::string const& name, tool::Parameter const& attribute)
        {
        }
        
        void Object::setAttribute(std::string const& name, tool::Parameter const& param)
        {
            deferMain([this, name, param]()
            {
                getObjectModel().setAttribute(name, param);
                
                model::DocumentManager::commit(getObjectModel());
            });
        }
        
        void Object::setParameter(std::string const& name, tool::Parameter const& param)
        {
            deferMain([this, name, param]
            {
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
            
            getScheduler().defer([object, cb = std::move(call_back)]()
            {
                if (!object.expired())
                {
                    cb();
                }
            });
        }
        
        void Object::deferMain(std::function<void()> call_back)
        {
            std::weak_ptr<engine::Object> object(m_master);
            
            getMainScheduler().defer([object, cb = std::move(call_back)]()
            {
                if (!object.expired())
                {
                    cb();
                }
            });
        }
        
        void Object::schedule(std::function<void()> call_back, tool::Scheduler<>::duration_t delay)
        {
            std::weak_ptr<engine::Object> object(m_master);
            
            getScheduler().schedule([object, cb = std::move(call_back)]()
            {
                if (!object.expired())
                {
                    cb();
                }
            }, delay);
        }
        
        void Object::scheduleMain(std::function<void()> call_back, tool::Scheduler<>::duration_t delay)
        {
            std::weak_ptr<engine::Object> object(m_master);
            
            getMainScheduler().schedule([object, cb = std::move(call_back)]()
            {
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
        
#define KIWI_ENGINE_STACKOVERFLOW_MAX 128
        
        void Object::send(const size_t index, std::vector<tool::Atom> const& args)
        {
            assert(getScheduler().isThisConsumerThread());
            
            const auto idx = static_cast<std::vector<Outlet>::size_type>(index);
            
            if(idx < m_outlets.size())
            {
                for(Link const& link : m_outlets[idx])
                {
                    Object& receiver = link.getReceiver();
                    if(++(receiver.m_stack_count) < KIWI_ENGINE_STACKOVERFLOW_MAX)
                    {
                        receiver.receive(link.getReceiverIndex(), args);
                    }
                    else if(++(receiver.m_stack_count) == KIWI_ENGINE_STACKOVERFLOW_MAX)
                    {
                        m_patcher.addStackOverflow(link);
                        receiver.receive(link.getReceiverIndex(), args);
                    }
                    else
                    {
                        m_patcher.endStackOverflow();
                        error("message loop detected");
                    }
                    --(receiver.m_stack_count);
                }
            
            }
        }

#undef KIWI_ENGINE_STACKOVERFLOW_MAX
        
        // ================================================================================ //
        //                                    AUDIOOBJECT                                   //
        // ================================================================================ //
        
        AudioObject::AudioObject(model::Object const& model, Patcher& patcher) noexcept:
        Object(model, patcher),
        dsp::Processor(model.getNumberOfInlets(), model.getNumberOfOutlets())
        {
        }
        
    }
}


