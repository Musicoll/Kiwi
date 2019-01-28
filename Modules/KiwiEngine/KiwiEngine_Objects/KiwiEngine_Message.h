/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
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

#include <array>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT MESSAGE                                  //
    // ================================================================================ //
    
    class Message : public engine::Object
    {
    public: // methods
        
        Message(model::Object const& model, Patcher& patcher);
        
        ~Message();
        
        void attributeChanged(std::string const& name, tool::Parameter const& param) override final;
        
        void receive(size_t index, std::vector<tool::Atom> const& args) override final;
        
        void outputMessage();
        
        static void declare();
        
        static std::unique_ptr<Object> create(model::Object const& model, Patcher& patcher);

    private: // members
        
        void sendMessages();
        void prepareMessagesForText(std::string const& text);

        flip::SignalConnection      m_connection;
        std::array<tool::Atom, 10>  m_input_args {};
        
        struct Sequence
        {
            Sequence(std::vector<tool::Atom>&& _atoms, bool _has_dollar)
            : atoms(_atoms), has_dollar(_has_dollar)
            {}
            
            ~Sequence() = default;
            
            const std::vector<tool::Atom> atoms {};
            const bool has_dollar = false;
        };
        
        std::vector<Sequence> m_messages {};
    };
    
}}
