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

#include "KiwiEngineFactory.hpp"
#include "KiwiEngineObjects.hpp"
#include "KiwiEngineInstance.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      INSTANCE                                    //
        // ================================================================================ //
        
        Instance::Instance()
        {
            addObjectsToFactory();
        }
        
        Instance::~Instance()
        {
            ;
        }
        
        // ================================================================================ //
        //                                      CONSOLE                                     //
        // ================================================================================ //
        
        void Instance::log(std::string const& text) const
        {
            m_console.post({text, Console::Message::Type::Log});
        }
        
        void Instance::post(std::string const& text) const
        {
            m_console.post({text, Console::Message::Type::Normal});
        }
        
        void Instance::warning(std::string const& text) const
        {
            m_console.post({text, Console::Message::Type::Warning});
        }
        
        void Instance::error(std::string const& text) const
        {
            m_console.post({text, Console::Message::Type::Error});
        }

        void Instance::addConsoleListener(Console::Listener& listener)
        {
            m_console.addListener(listener);
        }

        void Instance::removeConsoleListener(Console::Listener& listener)
        {
            m_console.removeListener(listener);
        }
        
        void Instance::addObjectsToFactory()
        {
            engine::Factory::add<NewBox>("newbox");
            engine::Factory::add<ErrorBox>("errorbox");
            engine::Factory::add<ObjectPlus>("plus");
            engine::Factory::add<ObjectPrint>("print");
        }
    }
}
