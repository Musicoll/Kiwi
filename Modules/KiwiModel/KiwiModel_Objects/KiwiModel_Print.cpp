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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Print.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                    OBJECT PRINT                                  //
        // ================================================================================ //
        
        void Print::declare()
        {
            std::unique_ptr<ObjectClass> print_class(new ObjectClass("print",
                                                                     &Print::create));
            
            // data model
            
            flip::Class<Print> & print_model = DataModel::declare<Print>()
                                               .name(print_class->getModelName().c_str())
                                               .inherit<Object>();
            
            Factory::add<Print>(std::move(print_class), print_model);
        }
        
        std::unique_ptr<Object> Print::create(std::vector<tool::Atom> const& args)
        {
            return std::make_unique<Print>();
        }
        
        Print::Print():
        Object()
        {
            pushInlet({PinType::IType::Control});
        }
        
        std::string Print::getIODescription(bool is_inlet, size_t index) const
        {
            return "Message to post in the Kiwi console";
        }
    }
}
