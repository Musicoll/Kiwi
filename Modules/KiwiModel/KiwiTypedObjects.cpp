/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#include "KiwiTypedObjects.hpp"

#include "KiwiPatcherModel.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
 
        void ObjectPlus::declare()
        {
            if(PatcherModel::has<ObjectPlus>()) return;
            
            PatcherModel::declare<ObjectPlus>()
            .name("cicm.kiwi.ObjectPlus")
            .inherit<model::Object>();
            
            ObjectFactory::registerModel<ObjectPlus>("plus");
        }
        
        ObjectPlus::ObjectPlus(std::string const& name, std::vector<Atom> args)
        {
            if(!args.empty() && args[0].isNumber())
            {
                setNumberOfInlets(1);
            }
            else
            {
                setNumberOfInlets(2);
            }
            
            setNumberOfOutlets(1);
        }
        
        // ================================================================================ //
        //                                    OBJECT PRINT                                  //
        // ================================================================================ //
        
        ObjectPrint::ObjectPrint(std::string const& name, std::vector<Atom> args)
        {
            ;
        }
        
        //! @internal flip static declare method
        void ObjectPrint::declare()
        {
            if(PatcherModel::has<ObjectPrint>()) return;
            
            PatcherModel::declare<ObjectPrint>()
            .name("cicm.kiwi.ObjectPrint")
            .inherit<model::Object>();
            
            ObjectFactory::registerModel<ObjectPrint>("print");
        }
    }
}
