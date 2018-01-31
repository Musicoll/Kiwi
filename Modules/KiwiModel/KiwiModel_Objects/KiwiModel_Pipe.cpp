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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Pipe.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                      OBJECT PIPE                                 //
    // ================================================================================ //
    
    void Pipe::declare()
    {
        std::unique_ptr<ObjectClass> pipe_class(new ObjectClass("pipe", &Pipe::create));
        
        flip::Class<Pipe> & pipe_model = DataModel::declare<Pipe>()
                                         .name(pipe_class->getModelName().c_str())
                                         .inherit<Object>();
        
        Factory::add<Pipe>(std::move(pipe_class), pipe_model);
    }
    
    std::unique_ptr<Object> Pipe::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Pipe>(args);
    }
    
    Pipe::Pipe(std::vector<tool::Atom> const& args)
    {
        if (args.size() > 1)
            throw Error("pipe too many arguments");
        
        if(args.size() == 1 && !args[0].isNumber())
            throw Error("pipe argument must be a number");
        
        pushInlet({PinType::IType::Control});
        
        if (args.empty())
        {
            pushInlet({PinType::IType::Control});
        }
        
        pushOutlet(PinType::IType::Control);
    }
    
    std::string Pipe::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            if(index == 0)
            {
                return "Anything to be delayed";
            }
            else if(index == 1)
            {
                return "Set delay time";
            }
        }
        else
        {
            return "Delayed messages";
        }
        
        return {};
    }

}}
