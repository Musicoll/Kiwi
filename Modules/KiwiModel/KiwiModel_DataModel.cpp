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

#include <flip/DataModel.h>

#include <KiwiModel/KiwiModel_Def.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_PatcherUser.h>

namespace kiwi
{
    namespace model
    {
        bool DataModel::initialised = false;
        
        void DataModel::declareObjects()
        {
            model::NewBox::declare();
            model::ErrorBox::declare();
            model::Slider::declare();
            model::Print::declare();
            model::Receive::declare();
            model::Plus::declare();
            model::Times::declare();
            model::Delay::declare();
            model::Metro::declare();
            model::Pipe::declare();
            model::Bang::declare();
            model::Toggle::declare();
            model::AdcTilde::declare();
            model::DacTilde::declare();
            model::OscTilde::declare();
            model::Loadmess::declare();
            model::PlusTilde::declare();
            model::TimesTilde::declare();
            model::SigTilde::declare();
            model::MeterTilde::declare();
            model::DelaySimpleTilde::declare();
            model::Message::declare();
            model::NoiseTilde::declare();
            model::PhasorTilde::declare();
            model::SahTilde::declare();
            model::SnapshotTilde::declare();
            model::Trigger::declare();
            model::LineTilde::declare();
        }
        
        void DataModel::init(std::function<void()> declare_object)
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
            
            declare_object();
            
            // Links
            Link::declare();
            
            // Patcher
            Patcher::declare();
            
            initialised = true;
        }
    }
}
