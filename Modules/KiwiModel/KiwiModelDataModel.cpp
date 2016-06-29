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

#include "flip/DataModel.h"

#include "KiwiModelDataModel.hpp"
#include "KiwiModelPatcher.hpp"

namespace kiwi
{
    namespace model
    {
        void DataModel::init()
        {
            // @todo => version in Macro
            DataModel::version("v0.0.2");
            
            // patcher elements declaration :
            model::Object::declare();
            
            declareObjects();
            
            // Links
            Link::declare();
            
            // Patcher
            Patcher::declare();
        }
        
        void DataModel::declareObjects()
        {
            NewBox::declare();
            ErrorBox::declare();
            ObjectPlus::declare();
            ObjectPrint::declare();
        }
    }
}
