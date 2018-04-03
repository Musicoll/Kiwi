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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_FaustTilde.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT PLUS~                                    //
    // ================================================================================ //
    
    void FaustTilde::declare()
    {
        std::unique_ptr<ObjectClass> fausttilde_class(new ObjectClass("faust~", &FaustTilde::create));
        
        flip::Class<FaustTilde> & fausttilde_model = DataModel::declare<FaustTilde>()
                                                   .name(fausttilde_class->getModelName().c_str())
                                                   .inherit<Object>();
        
        Factory::add<FaustTilde>(std::move(fausttilde_class), fausttilde_model);
    }
    
    std::unique_ptr<Object> FaustTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<FaustTilde>(args);
    }
    
    FaustTilde::FaustTilde(std::vector<tool::Atom> const& args)
    {
        if (args.size() < 3)
        {
            throw Error("faust~ expects 3 default arguments: the number of inlets, the number of outlets and the name of the file containing the FAUST code.");
        }
        if (args.size() > 3)
        {
            throw Error("faust~ too many arguments.");
        }
        
        if (!args[0].isInt() || args[0].getInt() < 0)
        {
            throw Error("faust~ 1st argument must be the number of inlets - positive or null");
        }
        if (!args[1].isInt() || args[1].getInt() < 0)
        {
            throw Error("faust~ 2nd argument must be the number of outlets - positive or null");
        }
        if (!args[2].isString())
        {
            throw Error("faust~ 3rd argument must be the name of the file containing the FAUST code");
        }
        pushInlet({PinType::IType::Control, PinType::IType::Signal});
        for(int i = 0; i < args[0].getInt(); ++i)
        {
            pushInlet({PinType::IType::Signal});
        }
        for(int i = 0; i < args[1].getInt(); ++i)
        {
            pushOutlet(PinType::IType::Signal);
        }
        pushOutlet(PinType::IType::Control);
    }
    
    std::string FaustTilde::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            if(index == 0)
            {
                return std::string("Messages and Audio Input Channel") + std::to_string(index);
            }
            return std::string("Audio Input Channel") + std::to_string(index);
        }
        else if (index < getNumberOfOutlets())
        {
            return std::string("Audio Output Channel") + std::to_string(index);
        }
        return std::string("Informations");
    }
    
}}
