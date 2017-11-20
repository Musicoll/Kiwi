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

#include <KiwiEngine/KiwiEngine_Object.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT TOGGLE                                   //
    // ================================================================================ //
    
    class Toggle : public engine::Object
    {
    private: // classes
        
        class Task;
        
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<Object> create(model::Object const& model, Patcher & patcher);
        
        Toggle(model::Object const& model, Patcher& patcher);
        
        ~Toggle();
        
        void parameterChanged(std::string const& name, tool::Parameter const& param) override final;
        
        void receive(size_t index, std::vector<tool::Atom> const& args) override final;
        
    private: // methods
        
        void outputValue();
        
    private: // members
        
        flip::SignalConnection      m_connection;
        bool                        m_is_on;
    };
}}
