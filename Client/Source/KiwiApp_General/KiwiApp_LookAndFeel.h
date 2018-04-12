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

#include <juce_gui_basics/juce_gui_basics.h>

namespace kiwi
{   
    class LookAndFeel final : public juce::LookAndFeel_V4
    {
    public: // methods
        
        //! @brief Constructor.
        LookAndFeel();
        
        //! @brief Destructor.
        ~LookAndFeel() = default;
        
        //! @brief Returns the typeface for a certain font name and style.
        //! @details LookAndFeel doesn't cache typeface because juce has its own caching mechanism
        juce::Typeface::Ptr getTypefaceForFont(juce::Font const& font) override;
        
        //! @brief Overriden to draw a custom PropertyPanel section header
        void drawPropertyPanelSectionHeader(juce::Graphics& g,
                                            const juce::String& name,
                                            bool is_open, int width, int height) override;
        
        //! @brief Overriden to draw a custom Table header background.
        void drawTableHeaderBackground(juce::Graphics& g, juce::TableHeaderComponent& header) override;
        
        //! @brief Overriden to draw a custom Table header column.
        void drawTableHeaderColumn(juce::Graphics&, juce::TableHeaderComponent&,
                                   const juce::String& columnName, int columnId,
                                   int width, int height,
                                   bool isMouseOver, bool isMouseDown, int columnFlags) override;
        
        //! @brief Custom Button background drawing
        void drawButtonBackground(juce::Graphics& g, juce::Button& b,
                                  juce::Colour const& bgcolor,
                                  bool mouse_over, bool mouse_down) override;
        
        //! @brief Custom Toolbar background drawing
        void paintToolbarBackground(juce::Graphics& g, int w, int h, juce::Toolbar& toolbar) override;
        
        //! @brief Custom Toolbar Button background drawing
        void paintToolbarButtonBackground(juce::Graphics& g, int /*width*/, int /*height*/,
                                          bool isMouseOver, bool isMouseDown,
                                          juce::ToolbarItemComponent& component) override;
        
        //! @brief Make a textLayout for tootips
        static juce::TextLayout layoutTooltipText(juce::String const& text,
                                                  juce::Colour colour = juce::Colours::black) noexcept;
        
    private: // deleted methods
        
        LookAndFeel(LookAndFeel const& other) = delete;
        LookAndFeel(LookAndFeel && other) = delete;
        LookAndFeel& operator=(LookAndFeel const& other) = delete;
        LookAndFeel& operator=(LookAndFeel && other) = delete;
    };
}
