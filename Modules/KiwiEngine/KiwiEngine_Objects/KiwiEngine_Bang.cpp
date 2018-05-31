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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Bang.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Bang.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT BANG                                     //
    // ================================================================================ //
    
    void Bang::declare()
    {
        Factory::add<Bang>("bang", &Bang::create);
    }
    
    std::unique_ptr<Object> Bang::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Bang>(model, patcher);
    }
    
    Bang::Bang(model::Object const& model, Patcher & patcher)
    : Object(model, patcher)
    , m_trigger_signal(model.getSignal<>(model::Bang::Signal::TriggerBang))
    , m_flash_signal(model.getSignal<>(model::Bang::Signal::FlashBang))
    , m_connection(m_trigger_signal.connect(std::bind(&Bang::signalTriggered, this)))
    {
    }
    
    Bang::~Bang()
    {
    }
    
    void Bang::signalTriggered()
    {
        defer([this]()
        {
            send(0, {"bang"});
        });
    }
    
    void Bang::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0 && !args.empty())
        {
            if(args[0].getString() == "set")
            {
                m_flash_signal();
            }
            else
            {
                m_trigger_signal();
            }
        }
    }
    
}}

