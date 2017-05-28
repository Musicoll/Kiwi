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

#include <juce_gui_extra/juce_gui_extra.h>

namespace kiwi
{
    // ================================================================================ //
    //                              CUSTOM TOOLTIP CLIENT                               //
    // ================================================================================ //
 
    //! @brief A custom tooltip client that provides the bounds of the tooltip to show.
    class CustomTooltipClient : public juce::TooltipClient
    {
    public:
        //! @brief Destructor.
        virtual ~CustomTooltipClient() = default;
        
        //! @brief Returns the bounds of the tooltip to show.
        virtual juce::Rectangle<int> getTooltipBounds(juce::String const& tip,
                                                      juce::Point<int> pos,
                                                      juce::Rectangle<int> parent_area,
                                                      int width,
                                                      int height) = 0;
    };
    
    // ================================================================================ //
    //                              CUSTOM TOOLTIP WINDOW                               //
    // ================================================================================ //
    
    //! @brief A custom TooltipWindow
    //! @details This is basically a copy of the juce::TooltipWindow class
    //! that adds the possibility to set a custom tooltip client
    //! that can be used to overwrite the original "mouse over component" behavior.
    //! @see juce::TooltipWindow, CustomTooltipClient, juce::SettableTooltipClient
    class TooltipWindow : public juce::Component, private juce::Timer
    {
    public: // methods
        
        //! @brief Constructor.
        //! @details Please see juce::TooltipWindow() for more details.
        //! @see juce::TooltipWindow
        explicit TooltipWindow(juce::Component* parentComponent = nullptr,
                               int m_milliseconds_before_tip_appears = 700);
        
        //! @brief Destructor.
        ~TooltipWindow();
        
        //! @brief Changes the time before the tip appears.
        //! @details This lets you change the value that was set in the constructor.
        void setMillisecondsBeforeTipAppears(int new_delay_ms = 700) noexcept;
        
        //! @brief Can be called to manually force a tip to be shown at a particular location.
        void displayTip(juce::Point<int> screen_position, juce::String const& text);
        
        //! @brief Set a custom tooltip client.
        //! @details When a CustomTooltipClient is set, the TooltipWindow will allways display it,
        //! ignoring other Tooltip clients over the mouse, until it is manually unset or hidden.
        //! @see CustomTooltipClient, unsetCustomTooltipClient
        void setCustomTooltipClient(CustomTooltipClient& client);
        
        //! @brief Unset a custom tooltip client.
        //! @details If the client parameter is the one that is currently used, this will hide the tip.
        //! @see CustomTooltipClient, setCustomTooltipClient
        void unsetCustomTooltipClient(CustomTooltipClient& client);
        
        //! @brief Can be called to manually hide the tip if it's showing.
        void hideTip();
        
    private: // methods
        
        void paint(juce::Graphics&) override;
        void mouseEnter(juce::MouseEvent const&) override;
        void timerCallback() override;
        void updatePosition(juce::String const&, juce::Point<int>, juce::Rectangle<int>);
        
    private: // members
        
        juce::Point<float>      m_last_mouse_pos;
        juce::Component*        m_last_component_under_mouse = nullptr;
        CustomTooltipClient*    m_custom_client = nullptr;
        CustomTooltipClient*    m_last_custom_client = nullptr;
        
        juce::String            m_tip_showing, m_last_tip_under_mouse;
        int                     m_milliseconds_before_tip_appears;
        int                     m_mouse_clicks, m_mouse_wheel_moves;
        unsigned int            m_last_comp_change_time, m_last_hide_time;
        bool                    m_reentrant;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TooltipWindow)
    };
}
