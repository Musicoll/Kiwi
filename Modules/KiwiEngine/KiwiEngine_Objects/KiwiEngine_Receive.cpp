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

#include <KiwiEngine/KiwiEngine_Factory.h>

#include <KiwiTool/KiwiTool_Scheduler.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Receive.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT RECEIVE                                  //
    // ================================================================================ //
    
    void Receive::declare()
    {
        Factory::add<Receive>("receive", &Receive::create);
    }
    
    std::unique_ptr<Object> Receive::create(model::Object const& model, Patcher& patcher)
    {
        return std::make_unique<Receive>(model, patcher);
    }
    
    Receive::Receive(model::Object const& model, Patcher& patcher):
    Object(model, patcher),
    m_name()
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        m_name = !args.empty() ? args[0].getString() : "";
        
        if(!m_name.empty())
        {
            tool::Beacon& beacon = getBeacon(m_name);
            beacon.bind(*this);
        }
    }
    
    Receive::~Receive()
    {
        if(!m_name.empty())
        {
            tool::Beacon& beacon = getBeacon(m_name);
            beacon.unbind(*this);
        }
    }
    
    void Receive::receive(size_t, std::vector<tool::Atom> const& args)
    {
        
    }
    
    void Receive::receive(std::vector<tool::Atom> const& args)
    {
        defer([this, args]()
        {
            send(0, args);
        });
    }
    
}}

