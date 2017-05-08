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

#include "flip/DataModel.h"

#include "KiwiModel_Def.hpp"
#include "KiwiModel_DataModel.hpp"
#include "KiwiModel_PatcherUser.hpp"
#include "KiwiModel_Objects.hpp"

namespace kiwi
{
    namespace model
    {
        bool DataModel::initialised = false;
        
        void DataModel::init()
        {
            assert(!initialised);
            if(initialised) return; // abort
                
            const std::string model_version = KIWI_MODEL_VERSION_STRING;
            
            #ifdef DEBUG
            std::cout << "- kiwi model version : " << model_version << '\n';
            #endif
            
            DataModel::version(model_version);
            
            // patcher elements declaration :
            model::Object::declare();
            
            declareObjects();
            
            // Links
            Link::declare();
            
            // Patcher
            Patcher::declare();
            
            initialised = true;
        }
        
        void DataModel::declareObjects()
        {
            NewBox::declare();
            ErrorBox::declare();
            Plus::declare();
            Times::declare();
            Print::declare();
            Receive::declare();
            Loadmess::declare();
            Delay::declare();
            Pipe::declare();
            Metro::declare();
            DacTilde::declare();
            AdcTilde::declare();
            OscTilde::declare();
            TimesTilde::declare();
            PlusTilde::declare();
            SigTilde::declare();
        }
    }
}
