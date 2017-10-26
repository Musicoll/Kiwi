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

#include <memory>
#include <set>

#include <flip/Signal.h>

#include <KiwiTool/KiwiTool_Scheduler.h>
#include <KiwiTool/KiwiTool_ConcurrentQueue.h>

#include <KiwiModel/KiwiModel_Object.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Toggle.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectView.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   TOGGLE VIEW                                    //
    // ================================================================================ //
    
    class ToggleView : public ObjectView
    {
    private: // classes
        
        class Task;
        
    public: // methods
        
        //! @brief Constructor.
        ToggleView(model::Object & object_model);
        
        //! @brief Destructor.
        ~ToggleView();
        
    private: // methods
        
        //! @brief The graphical rendering method.
        void paint(juce::Graphics & g) override final;
        
        //! @brief Called when the bang is clicked.
        void mouseDown(juce::MouseEvent const& e) override final;
        
        //! @brief Called whenever the toggle is switched on or off.
        //! @details Can ba called on the engine thread or the gui thread.
        void toggleSwitched(model::Toggle::Request request, bool shall_ouptut);
        
    private: // members
        
        flip::Signal<model::Toggle::Request, bool> &    m_signal;
        flip::SignalConnection                          m_connection;
        bool                                            m_is_on;
        tool::ConcurrentQueue<std::shared_ptr<Task>>    m_tasks;
        
    private: // deleted methods
        
        ToggleView() = delete;
        ToggleView(ToggleView const& other) = delete;
        ToggleView(ToggleView && other) = delete;
        ToggleView& operator=(ToggleView const& other) = delete;
        ToggleView& operator=(ToggleView && other) = delete;
    };
}
