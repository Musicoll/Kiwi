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

#ifndef KIWI_ENGINE_GUI_DEVICE_HPP_INCLUDED
#define KIWI_ENGINE_GUI_DEVICE_HPP_INCLUDED

#include "KiwiTimer.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                     GUI DEVICE                                   //
        // ================================================================================ //
 
        //! @brief The gui device.
        class GuiDevice
        {
        public:
            
            //! @brief Constructor.
            GuiDevice() = default;
            
            //! @brief Destructor.
            virtual ~GuiDevice() = default;
            
            //! @brief Creates and returns a timer implementation.
            virtual std::unique_ptr<engine::Timer::Impl> createTimer(std::function<void()> callback);
        };
    }
}

#endif // KIWI_ENGINE_GUI_DEVICE_HPP_INCLUDED
