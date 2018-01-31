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

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectView.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                   TOGGLE VIEW                                    //
    // ================================================================================ //
    
    class ToggleView : public ObjectView
    {
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<ObjectView> create(model::Object & model);
        
        ToggleView(model::Object & object_model);
        
        ~ToggleView();
        
    private: // methods
        
        void paint(juce::Graphics & g) override final;
        
        void mouseDown(juce::MouseEvent const& e) override final;
        
        void parameterChanged(std::string const& name, tool::Parameter const& param) override final;
        
    private: // members
        
        flip::Signal<> &            m_signal;
        bool                        m_is_on;
        
    private: // deleted methods
        
        ToggleView() = delete;
        ToggleView(ToggleView const& other) = delete;
        ToggleView(ToggleView && other) = delete;
        ToggleView& operator=(ToggleView const& other) = delete;
        ToggleView& operator=(ToggleView && other) = delete;
    };
}
