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

#include "KiwiModelPatcherModel.hpp"
#include "KiwiModelPatcher.hpp"

namespace kiwi
{
    namespace model
    {
        void PatcherModel::init(std::string const& version)
        {
            PatcherModel::version(version);
            
            // patcher elements declaration :
            model::Object::declare();
            
            declareObjects();
            
            declareExternalObjects();
            
            // Links
            Link::declare();
            
            // Patcher
            Patcher::declare();
            
            endOfModelDeclaration();
        }
        
        void PatcherModel::declareObjects()
        {
            NewBox::declare();
            ErrorBox::declare();
            ObjectPlus::declare();
            ObjectPrint::declare();
        }
    }
}
