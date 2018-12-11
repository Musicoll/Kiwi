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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Number.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_NumberView.h>
#include <KiwiApp_Patcher/KiwiApp_Factory.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                   NUMBER VIEW                                    //
    // ================================================================================ //
    
    void NumberView::declare()
    {
        Factory::add<NumberView>("number", &NumberView::create);
    }
    
    std::unique_ptr<ObjectView> NumberView::create(model::Object & object_model)
    {
        return std::make_unique<NumberView>(object_model);
    }
    
    NumberView::NumberView(model::Object & object_model)
    : NumberViewBase(object_model)
    , m_output_message(object_model.getSignal<>(model::Message::Signal::outputMessage))
    {
        setInterceptsMouseClicks(true, true);
    }
    
    NumberView::~NumberView()
    {}
    
    void NumberView::drawIcon (juce::Graphics& g) const
    {
        g.setColour(findColour(ObjectView::ColourIds::Background).contrasting(0.2));
        
        const juce::Rectangle<int> icon_bounds = getLocalBounds()
        .withWidth(m_indent - 4).withHeight(getHeight() - 8).translated(2, 4);
        
        juce::Path corner;
        
        corner.addTriangle(icon_bounds.getTopLeft().toFloat(),
                           icon_bounds.getTopRight().toFloat()
                           + juce::Point<float>(0, (icon_bounds.getHeight() / 2.)),
                           icon_bounds.getBottomLeft().toFloat());
        
        g.fillPath(corner);
    }
    
    void NumberView::mouseDoubleClick(juce::MouseEvent const& e)
    {
        getLabel().showEditor();
    }
    
    void NumberView::mouseDown(juce::MouseEvent const& e)
    {
        e.source.enableUnboundedMouseMovement(true, true);
        
        m_last_drag_pos = e.position;
        m_drag_value = m_value;
        
        auto const& label = getLabel();
        const auto textArea = label.getBorderSize().subtractedFrom(label.getBounds());
        
        juce::GlyphArrangement glyphs;
        glyphs.addFittedText (label.getFont(), label.getText(),
                              textArea.getX(), 0., textArea.getWidth(), getHeight(),
                              juce::Justification::centredLeft, 1,
                              label.getMinimumHorizontalScale());
        
        double decimal_x = getWidth();
        for(int i = 0; i < glyphs.getNumGlyphs(); ++i)
        {
            auto const& glyph = glyphs.getGlyph(i);
            if(glyph.getCharacter() == '.')
            {
                decimal_x = glyph.getRight();
            }
        }
        
        const bool is_dragging_decimal = e.x > decimal_x;
        
        m_decimal_drag = is_dragging_decimal ? 6 : 0;
        
        if(is_dragging_decimal)
        {
            juce::GlyphArrangement decimals_glyph;
            static const juce::String decimals_str("000000");
            
            decimals_glyph.addFittedText (label.getFont(), decimals_str,
                                          decimal_x, 0, getWidth(), getHeight(),
                                          juce::Justification::centredLeft, 1,
                                          label.getMinimumHorizontalScale());
            
            for(int i = 0; i < decimals_glyph.getNumGlyphs(); ++i)
            {
                auto const& glyph = decimals_glyph.getGlyph(i);
                if(e.x <= glyph.getRight())
                {
                    m_decimal_drag = i+1;
                    break;
                }
            }
        }
    }
    
    void NumberView::mouseDrag(juce::MouseEvent const& e)
    {
        setMouseCursor(juce::MouseCursor::NoCursor);
        updateMouseCursor();
        
        if (e.mouseWasDraggedSinceMouseDown())
        {
            const int decimal = m_decimal_drag + e.mods.isShiftDown();
            const double increment = (decimal == 0) ? 1. : (1. / std::pow(10., decimal));
            const double delta_y = e.y - m_last_drag_pos.y;
            m_last_drag_pos = e.position;
            
            m_drag_value += increment * -delta_y;

            if(m_drag_value != m_value)
            {
                // truncate value and set
                
                double new_value = m_drag_value;
                
                if(decimal > 0)
                {
                    const int sign = (new_value > 0) ? 1 : -1;
                    unsigned int ui_temp = (new_value * std::pow(10, decimal)) * sign;
                    new_value = (((double)ui_temp)/std::pow(10, decimal) * sign);
                }
                else
                {
                    new_value = static_cast<int64_t>(new_value);
                }
                
                setParameter("value",
                             tool::Parameter(tool::Parameter::Type::Float, {new_value}));
                m_output_message();
                repaint();
            }
        }
    }
    
    void NumberView::mouseUp(juce::MouseEvent const& e)
    {
        setMouseCursor(juce::MouseCursor::NormalCursor);
        updateMouseCursor();
        
        juce::Desktop::getInstance()
        .getMainMouseSource().setScreenPosition(e.getMouseDownScreenPosition().toFloat());
    }
    
    void NumberView::parameterChanged(std::string const& name, tool::Parameter const& param)
    {
        if (name == "value")
        {
            setDisplayNumber(param[0].getFloat());
        }
    }
    
    void NumberView::displayNumberChanged(double number)
    {
        setParameter("value", tool::Parameter(tool::Parameter::Type::Float, {number}));
        m_output_message();
    }
}

