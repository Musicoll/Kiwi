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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Trigger.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    TRIGGER                                       //
    // ================================================================================ //
    
    void Trigger::declare()
    {
        Factory::add<Trigger>("trigger", &Trigger::create);
    }
    
    std::unique_ptr<Object> Trigger::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Trigger>(model, patcher);
    }
    
    Trigger::Trigger(model::Object const& model, Patcher& patcher)
    : Object(model, patcher)
    , m_triggers(initializeTriggers(model.getArguments()))
    {
        ;
    }
    
    auto Trigger::initializeTriggers(std::vector<tool::Atom> const& _args) -> std::vector<trigger_fn_t>
    {
        const auto trigger_fn_factory = [](tool::Atom const& atom) -> trigger_fn_t {
            
            if(atom.isNumber())
            {
                const auto value = atom.isInt() ? atom.getInt() : atom.getFloat();
                
                return [value](std::vector<tool::Atom> const&){
                    return std::vector<tool::Atom>{{value}};
                };
            }
            
            const auto str = atom.getString();
            
            if(str == "b")
            {
                return [](std::vector<tool::Atom> const&) { return std::vector<tool::Atom>{{"bang"}}; };
            }
            
            if(str == "i")
            {
                return [](std::vector<tool::Atom> const& args) {
                    return std::vector<tool::Atom> {{args[0].getInt()}};
                };
            }
            
            if(str == "f")
            {
                return [](std::vector<tool::Atom> const& args) {
                    return std::vector<tool::Atom> {{args[0].getFloat()}};
                };
            }
            
            if(str == "s")
            {
                return [](std::vector<tool::Atom> const& args) {
                    return std::vector<tool::Atom> {{args[0].getString()}};
                };
            }
            
            if(str == "l")
            {
                return [](std::vector<tool::Atom> const& args) { return args; };
            }
            
            return [str](std::vector<tool::Atom> const&) { return std::vector<tool::Atom>{{str}}; };
        };
        
        std::vector<trigger_fn_t> triggers;
        triggers.reserve(_args.size());
        
        for(auto& atom : _args)
        {
            triggers.emplace_back(trigger_fn_factory(atom));
        }
        
        return triggers;
    }
    
    void Trigger::receive(size_t, std::vector<tool::Atom> const& args)
    {
        if(m_triggers.empty())
            return;
        
        const auto size = m_triggers.size();
        size_t idx = size - 1;
        for (auto rit = m_triggers.rbegin(); rit != m_triggers.rend(); ++rit)
        {
            send(idx--, (*rit)(args));
        }
    }
    
}}
