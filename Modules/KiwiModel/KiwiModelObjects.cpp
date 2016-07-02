/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiModelObjects.hpp"

#include "KiwiModelDataModel.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                       NEWBOX                                     //
        // ================================================================================ //
        
        void NewBox::declare()
        {
            if(DataModel::has<NewBox>()) return;
            
            DataModel::declare<NewBox>()
            .name("cicm.kiwi.NewBox")
            .inherit<model::Object>();
            
            Factory::add<NewBox>("newbox");
        }
        
        NewBox::NewBox(std::string const& name, std::vector<Atom> const& args)
        {
            setNumberOfInlets(1);
            setNumberOfOutlets(0);
        }
        
        // ================================================================================ //
        //                                      ERRORBOX                                    //
        // ================================================================================ //
        
        void ErrorBox::declare()
        {
            if(DataModel::has<ErrorBox>()) return;
            
            DataModel::declare<ErrorBox>()
            .name("cicm.kiwi.ErrorBox")
            .inherit<model::Object>();
            
            Factory::add<ErrorBox>("errorbox");
        }
        
        ErrorBox::ErrorBox(std::string const& name, std::vector<Atom> const& args)
        {
            setNumberOfInlets(0);
            setNumberOfOutlets(0);
        }
        
        void ErrorBox::setNumberOfInlets(size_t inlets)
        {
            model::Object::setNumberOfInlets(inlets);
        }
        
        void ErrorBox::setNumberOfOutlets(size_t outlets)
        {
            model::Object::setNumberOfOutlets(outlets);
        }
        
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
 
        void ObjectPlus::declare()
        {
            if(DataModel::has<ObjectPlus>()) return;
            
            DataModel::declare<ObjectPlus>()
            .name("cicm.kiwi.ObjectPlus")
            .inherit<model::Object>();
            
            Factory::add<ObjectPlus>("plus");
        }
        
        ObjectPlus::ObjectPlus(std::string const& name, std::vector<Atom> const& args)
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
        
        ObjectPrint::ObjectPrint(std::string const& name, std::vector<Atom> const& args)
        {
            ;
        }
        
        //! @internal flip static declare method
        void ObjectPrint::declare()
        {
            if(DataModel::has<ObjectPrint>()) return;
            
            DataModel::declare<ObjectPrint>()
            .name("cicm.kiwi.ObjectPrint")
            .inherit<model::Object>();
            
            Factory::add<ObjectPrint>("print");
        }
    }
}
