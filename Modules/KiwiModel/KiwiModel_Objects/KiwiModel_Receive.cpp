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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Receive.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT RECEIVE                                  //
    // ================================================================================ //
    
    void Receive::declare()
    {
        // class
        std::unique_ptr<ObjectClass> receive_class(new ObjectClass("receive",
                                                                   &Receive::create));
        
        receive_class->addAlias("r");
        
        // data model
        
        flip::Class<Receive> & receive_model = DataModel::declare<Receive>()
                                               .name(receive_class->getModelName().c_str())
                                               .inherit<Object>();
        
        Factory::add<Receive>(std::move(receive_class), receive_model);
    }
    
    std::unique_ptr<Object> Receive::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Receive>(args);
    }
    
    Receive::Receive(std::vector<tool::Atom> const& args):
    Object()
    {
        if (args.empty())
        {
            throw Error("receive requires one argument");
        }
        
        if (args.size() > 1)
        {
            throw Error("receive requires only one argument");
        }
        
        if (args.size() ==  1 && args[0].isString())
        {
            throw Error("receive argument must be a symbol");
        }
        
        pushOutlet(PinType::IType::Control);
    }
    
    std::string Receive::getIODescription(bool is_inlet, size_t index) const
    {
        if(!is_inlet)
        {
            return "Receive messages";
        }
        
        return {};
    }
    
}}
