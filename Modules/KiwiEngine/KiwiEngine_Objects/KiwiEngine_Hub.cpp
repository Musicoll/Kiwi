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

#include <KiwiTool/KiwiTool_Atom.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Hub.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT HUB                                      //
    // ================================================================================ //
    
    void Hub::declare()
    {
        Factory::add<Hub>("hub", &Hub::create);
    }
    
    std::unique_ptr<Object> Hub::create(model::Object const& model, Patcher& patcher)
    {
        return std::make_unique<Hub>(model, patcher);
    }
    
    Hub::Hub(model::Object const& object_model, Patcher& patcher):
    Object(object_model, patcher)
    {
    }
    
    
    Hub::~Hub()
    {
    }
    
    void Hub::attributeChanged(std::string const& name, tool::Parameter const& parameter)
    {
        if (name == "message")
        {
            send(0, tool::AtomHelper::parse(parameter[0].getString()));
        }
    }
    
    void Hub::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0 && !args.empty())
        {
            setAttribute("message", { tool::Parameter::Type::String, {tool::AtomHelper::toString(args)}});
        }
    }
    
}}
