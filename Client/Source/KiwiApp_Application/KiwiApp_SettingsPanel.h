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

#include <juce_gui_extra/juce_gui_extra.h>

namespace kiwi
{
    // ================================================================================ //
    //                                  SETTINGS PANEL                                  //
    // ================================================================================ //
    
    //! @brief A Panel Component that shows application's settings.
    class SettingsPanel : public juce::Component,
                          public juce::Button::Listener
    {
    public: // classes
        
        //! @brief Constructor.
        SettingsPanel();
        
        //! @brief Destructor.
        ~SettingsPanel();
        
    private: // methods
        
        //! @brief Resized methods called when resized.
        void resized() override final;
        
        //! @brief Called when apply or reset button is clicked.
        void buttonClicked(juce::Button * button) override final;
        
    private: // members
        
        juce::ValueTree     m_settings;
        juce::PropertyPanel m_pannel;
        juce::TextButton    m_apply_button;
        juce::TextButton    m_reset_button;
    };
}
