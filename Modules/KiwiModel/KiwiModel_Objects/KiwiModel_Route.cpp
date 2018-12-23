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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Route.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                       ROUTE                                      //
    // ================================================================================ //
    
    void Route::declare()
    {
        auto object_class = std::make_unique<ObjectClass>("route", &Route::create);
        
        auto& model = DataModel::declare<Route>()
        .name(object_class->getModelName().c_str())
        .inherit<Object>();
        
        Factory::add<Route>(std::move(object_class), model);
    }
    
    std::unique_ptr<Object> Route::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<Route>(args);
    }
    
    Route::Route(std::vector<tool::Atom> const& args)
    {
        pushInlet({PinType::IType::Control});
        
        const auto nargs = args.size();
        if(nargs == 0)
        {
            pushInlet({PinType::IType::Control}); // custom selector input
            pushOutlet(PinType::IType::Control); // custom selector output
        }
        else
        {
            for(size_t i = 0; i < args.size(); ++i)
            {
                pushOutlet(PinType::IType::Control);
            }
        }
        
        pushOutlet(PinType::IType::Control); // not match outlet
    }
    
    std::string Route::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            if(index == 0)
                return "(anything) Message to be routed";
            
            if (index == 1)
                return "(int/float/symbol) Change selector";
        }
        
        auto const& args = getArguments();
        
        if(args.empty() && index == 0)
        {
            return {"Message matches input selector"};
        }
        
        if(!args.empty() && index < args.size())
        {
            return {"Message matches " + tool::AtomHelper::toString(args[index])};
        }
        
        return "Doesn't match";
    }
    
}}
