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

#pragma once

#include <memory.h>
#include <set>

#include <flip/Signal.h>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Toggle.h>

#include <KiwiEngine/KiwiEngine_ConcurrentQueue.h>
#include <KiwiEngine/KiwiEngine_Scheduler.h>

#include <KiwiEngine/KiwiEngine_Object.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                  OBJECT TOGGLE                                   //
        // ================================================================================ //
        
        class Toggle : public engine::Object
        {
        private: // classes
            
            class Task;
            
        public: // methods
            
            Toggle(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            ~Toggle();
            
            void receive(size_t index, std::vector<Atom> const& args) override final;
            
        private: // methods
            
            void toggleSwitched(model::Toggle::Request request, bool shall_output);
            
        private: // members
            
            flip::Signal<model::Toggle::Request, bool> &    m_signal;
            flip::SignalConnection                          m_connection;
            bool                                            m_is_on;
            engine::ConcurrentQueue<std::shared_ptr<Task>>  m_tasks;
        };
    }
}
