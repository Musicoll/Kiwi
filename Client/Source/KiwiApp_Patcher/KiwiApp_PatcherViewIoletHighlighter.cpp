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

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectFrame.h>

#include "KiwiApp_PatcherViewIoletHighlighter.h"
#include "../KiwiApp.h"

namespace kiwi
{
    // ================================================================================ //
    //                                   IOLET HILIGHTER                                //
    // ================================================================================ //
    
    IoletHighlighter::IoletHighlighter():
    m_text(""),
    m_object_name(""),
    m_show_tooltip_on_left(false),
    m_last_index(),
    m_object_ref()
    {
        setInterceptsMouseClicks(false, false);
        setAlwaysOnTop(true);
        
        setVisible(false);
        setBounds(0, 0, 1, 1);
    }
    
    void IoletHighlighter::hide()
    {
        KiwiApp::useTooltipWindow().unsetCustomTooltipClient(*this);
        m_text.clear();
        setVisible(false);
    }
    
    void IoletHighlighter::paint(juce::Graphics& g)
    {
        const juce::Colour bgcolor = m_is_inlet ? juce::Colour(0xFF17BEBB) : juce::Colour(0xFFCD5334);
        const juce::Colour bd_color(0xFF2E282A);
        
        const juce::Rectangle<float> bounds = getLocalBounds().reduced(1).toFloat();
        
        g.setColour(bgcolor);
        g.fillRect(bounds);
        
        g.setColour(bd_color);
        g.drawRect(bounds);
    }
    
    void IoletHighlighter::highlightInlet(ObjectFrame const& object, const size_t index)
    {
        highlight(object, index, true);
    }
    
    void IoletHighlighter::highlightOutlet(ObjectFrame const& object, const size_t index)
    {
        highlight(object, index, false);
    }
    
    void IoletHighlighter::highlight(ObjectFrame const& object, const size_t index, bool is_inlet)
    {
        auto const& object_model = object.getModel();
        
        //! Only hilight if either no pin is currently hilighted
        //! or hilighted pin is different.
        if(!isVisible() || (m_object_ref != object_model.ref() || m_is_inlet != is_inlet || m_last_index != index))
        {
            auto pos = is_inlet
            ? object.getInletPatcherPosition(index) : object.getOutletPatcherPosition(index);
            
            m_text = object_model.getIODescription(is_inlet, index);
            m_object_name = object_model.getName();
            m_is_inlet = is_inlet;
            m_object_ref = object_model.ref();
            m_last_index = index;
            
            setBounds(juce::Rectangle<int>(pos, pos).expanded(5));
            setVisible(true);
            
            m_show_tooltip_on_left = m_is_inlet ? index < object_model.getNumberOfInlets() * 0.5 : index < object_model.getNumberOfOutlets() * 0.5;
            
            KiwiApp::useTooltipWindow().setCustomTooltipClient(*this);
        }
    }
    
    juce::String IoletHighlighter::getTooltip()
    {
        return m_object_name + ": " + m_text;
    }
    
    juce::Rectangle<int> IoletHighlighter::getTooltipBounds(juce::String const& tip,
                                                            juce::Point<int> /*pos*/,
                                                            juce::Rectangle<int> parent_area, int w, int h)
    {
        h += 5;
        w += 5;
        const int margin = 12;
        const auto pos = getScreenBounds().getCentre();
        
        const int on_left_pos = pos.x - w - margin;
        int x_pos = on_left_pos;
        
        if(!m_show_tooltip_on_left || (m_show_tooltip_on_left && on_left_pos < parent_area.getX()))
        {
            x_pos = pos.x + margin;
        }
        
        if(x_pos + w > parent_area.getRight())
        {
            x_pos = on_left_pos;
        }
        
        return juce::Rectangle<int>(x_pos, m_is_inlet ? pos.y - h - margin : pos.y + margin, w, h)
        .constrainedWithin(parent_area);
    }
    
    bool IoletHighlighter::drawTooltip(juce::Graphics& g, juce::String const& text, int width, int height)
    {
        juce::Rectangle<int> bounds(width, height);
        const auto corner_size = 0.f;
        
        const juce::Colour bgcolor = m_is_inlet ? juce::Colour(0xFF17BEBB) : juce::Colour(0xFFCD5334);
        
        g.setColour(bgcolor);
        g.fillRoundedRectangle(bounds.toFloat(), 0);
        
        g.setColour(juce::Colours::whitesmoke);
        g.fillRoundedRectangle(bounds.toFloat().reduced(0.5f, 0.5f), corner_size);
        
        g.setColour(bgcolor);
        g.drawRoundedRectangle(bounds.toFloat().reduced(0.5f, 0.5f), corner_size, 1.f);
        
        LookAndFeel::layoutTooltipText(text, juce::Colours::black)
        .draw(g, juce::Rectangle<float>((float) width, (float) height));
        
        return true;
    }
}
