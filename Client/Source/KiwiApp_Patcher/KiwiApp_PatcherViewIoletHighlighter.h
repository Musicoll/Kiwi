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
#include "../KiwiApp_Components/KiwiApp_TooltipWindow.h"

namespace kiwi
{
    class ObjectFrame;
    
    // ================================================================================ //
    //                                   IOLET HILIGHTER                                //
    // ================================================================================ //
    
    class IoletHighlighter
    : public juce::Component
    , public CustomTooltipClient
    {
    public:
        
        //! @brief Constructor.
        IoletHighlighter();
        
        //! @brief Destructor.
        ~IoletHighlighter() = default;
        
        //! @brief The paint method.
        void paint(juce::Graphics& g) override;
        
        //! @brief Stop highlighting.
        void hide();
        
        //! @brief Highlight inlet
        void highlightInlet(ObjectFrame const& object, const size_t index);
        
        //! @brief Highlight outlet
        void highlightOutlet(ObjectFrame const& object, const size_t index);
        
        //! @brief Returns the string that this object wants to show as its tooltip.
        juce::String getTooltip() override;
        
        //! @brief Returns the bounds of the tooltip to show.
        juce::Rectangle<int> getTooltipBounds(juce::String const& tip,
                                              juce::Point<int>,
                                              juce::Rectangle<int> parent_area,
                                              int width,
                                              int height) override;
        
        //! @brief Overriden to provide a custom drawing method.
        bool drawTooltip(juce::Graphics& g,
                         juce::String const& text,
                         int width, int height) override;
        
    private: // methods
        
        void highlight(ObjectFrame const& object, const size_t index);
        
    private: // members
        
        bool                    m_is_inlet;
        std::string             m_text;
        std::string             m_object_name;
        bool                    m_show_tooltip_on_left;
        size_t                  m_last_index;
    };
}
