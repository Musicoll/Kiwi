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

#include <vector>
#include <memory>

#include <flip/Signal.h>

#include <KiwiModel/KiwiModel_Atom.h>
#include <KiwiModel/KiwiModel_Object.h>

#include <KiwiEngine/KiwiEngine_Object.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT BANG                                     //
    // ================================================================================ //
    
    class Slider : public engine::Object
    {
    private: // classes
        
        class Task;
        
    public: // methods
        
        Slider(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
        
        ~Slider();
        
        void valueChanged(double new_value);
        
        void receive(size_t index, std::vector<Atom> const& args) override final;
        
    private: // members
        
        flip::Signal<double> &                          m_signal;
        flip::SignalConnection                          m_connection;
        engine::ConcurrentQueue<std::shared_ptr<Task>>  m_tasks;
    };
    
}}
