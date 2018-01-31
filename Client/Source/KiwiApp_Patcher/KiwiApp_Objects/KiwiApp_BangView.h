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

#include <juce_graphics/juce_graphics.h>

#include <KiwiModel/KiwiModel_Object.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectView.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                   BANG VIEW                                      //
    // ================================================================================ //
    
    class BangView : public ObjectView
    {
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<ObjectView> create(model::Object & model);
        
        BangView(model::Object & object_model);
        
        ~BangView();
        
    private: // methods
        
        void paint(juce::Graphics & g) override final;
        
        void mouseDown(juce::MouseEvent const& e) override final;
        
        void mouseUp(juce::MouseEvent const& e) override final;
        
        void flash();
        
        void switchOff();
        
        void signalTriggered();
        
    private: // members
        
        //! @todo Put border into ObjectView.
        flip::Signal<>&                                 m_signal;
        flip::SignalConnection                          m_connection;
        bool                                            m_active;
        bool                                            m_mouse_down;
        std::shared_ptr<tool::Scheduler<>::CallBack>    m_switch_off;
        
    private: // deleted methods
        
        BangView() = delete;
        BangView(BangView const& other) = delete;
        BangView(BangView && other) = delete;
        BangView& operator=(BangView const& other) = delete;
        BangView& operator=(BangView && other) = delete;
    };
}
