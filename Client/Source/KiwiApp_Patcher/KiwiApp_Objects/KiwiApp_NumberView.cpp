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
    , m_sensitivity(1)
    , m_mouse_info()
    {
        setInterceptsMouseClicks(true, true);
    }
    
    NumberView::~NumberView()
    {
    }
    
    void NumberView::drawIcon (juce::Graphics& g) const
    {
        g.setColour(findColour(ObjectView::ColourIds::Outline));
        
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
        
        m_mouse_info.m_mouse_down_value = getDisplayNumber();
        m_mouse_info.m_mouse_down_y = e.y;
        
        if (e.mods.isShiftDown())
        {
            m_mouse_info.m_is_alt_down = true;
            m_mouse_info.m_alt_down_value = getDisplayNumber();
            m_mouse_info.m_alt_down_y = e.y;
        }
    }
    
    void NumberView::mouseDrag(juce::MouseEvent const& e)
    {
        if (e.getDistanceFromDragStartY() != 0)
        {
            double new_value = m_value;
            
            if (!m_mouse_info.m_is_alt_down && e.mods.isShiftDown())
            {
                m_mouse_info.m_is_alt_down = true;
                m_mouse_info.m_alt_down_value = getDisplayNumber();
                m_mouse_info.m_alt_down_y = e.y;
            }
            else if(m_mouse_info.m_is_alt_down && !e.mods.isShiftDown())
            {
                m_mouse_info.m_mouse_down_value = getDisplayNumber();
                m_mouse_info.m_mouse_down_y = e.y;
                m_mouse_info.m_is_alt_down = false;
                m_mouse_info.m_is_alt_down = 0;
                m_mouse_info.m_alt_down_y = 0;
            }
            
            if (e.mods.isShiftDown())
            {
                new_value = m_mouse_info.m_alt_down_value - (m_sensitivity * (e.y - m_mouse_info.m_alt_down_y)) / 100.;
            }
            else
            {
                new_value = m_mouse_info.m_mouse_down_value - m_sensitivity * (e.y - m_mouse_info.m_mouse_down_y);
            }
            
            if(new_value != m_value)
            {
                setParameter("value", tool::Parameter(tool::Parameter::Type::Float, {new_value}));
                m_output_message();
                repaint();
            }
        }
    }
    
    void NumberView::mouseUp(juce::MouseEvent const& e)
    {
        m_mouse_info.m_mouse_down_value = 0;
        m_mouse_info.m_mouse_down_y = 0;
        m_mouse_info.m_is_alt_down = false;
        m_mouse_info.m_alt_down_value = 0;
        m_mouse_info.m_alt_down_y = 0;
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

