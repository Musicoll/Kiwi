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

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                  OBJECT RECEIVE                                  //
        // ================================================================================ //
        
        class Receive : public engine::Object, public tool::Beacon::Castaway
        {
        private: // classes
            
            class Task;
            
        public: // methods
            
            Receive(model::Object const& model, Patcher& patcher, std::vector<tool::Atom> const& args);
            
            ~Receive();
            
            void receive(size_t, std::vector<tool::Atom> const& args) override;
            
            void receive(std::vector<tool::Atom> const& args) override;
            
        private: // members
            
            std::string                     m_name;
            std::set<std::shared_ptr<Task>> m_tasks;
        };
    }
}
