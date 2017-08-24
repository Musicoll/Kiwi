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

#include <flip/Signal.h>

#include <juce_graphics/juce_graphics.h>

#include <KiwiEngine/KiwiEngine_Scheduler.h>

#include <KiwiModel/KiwiModel_Object.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectView.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   BANG VIEW                                      //
    // ================================================================================ //
    
    class BangView : public ObjectView
    {
    public: // methods
        
        //! @brief Constructor.
        BangView(model::Object & object_model);
        
        //! @brief Destructor.
        ~BangView();
        
    private: // methods
        
        //! @brief The graphical rendering method.
        void paint(juce::Graphics & g) override final;
        
        //! @brief Called when the bang is clicked.
        void mouseDown(juce::MouseEvent const& e) override final;
        
        //! @brief Called when the bang is unclicked.
        void mouseUp(juce::MouseEvent const& e) override final;
        
        //! @brief Switches the bang view on and schedules to switch it off.
        void flash();
        
        //! @brief Switches the bang view off.
        void switchOff();
        
        //! @brief Called whenever bang is trigerred.
        void signalTriggered();
        
    private: // members
        
        //! @todo Put border into ObjectView.
        juce::BorderSize<int>                           m_border;
        flip::Signal<>&                                 m_signal;
        flip::SignalConnection                          m_connection;
        bool                                            m_active;
        bool                                            m_mouse_down;
        std::shared_ptr<engine::Scheduler<>::CallBack>  m_switch_off;
        
    private: // deleted methods
        
        BangView() = delete;
        BangView(BangView const& other) = delete;
        BangView(BangView && other) = delete;
        BangView& operator=(BangView const& other) = delete;
        BangView& operator=(BangView && other) = delete;
    };
}
