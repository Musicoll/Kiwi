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

#include <functional>

#include <juce_gui_basics/juce_gui_basics.h>

#include <KiwiTool/KiwiTool_Scheduler.h>

#include <KiwiApp.h>
#include <KiwiApp_Patcher/KiwiApp_Factory.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_SliderView.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   SLIDER VIEW                                    //
    // ================================================================================ //
    
    SliderView::SliderView(model::Object & object_model):
    ObjectView(object_model),
    m_slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::NoTextBox)
    {
        m_slider.setColour(juce::Slider::ColourIds::backgroundColourId,
                           findColour(ObjectView::ColourIds::Outline));
        
        m_slider.setColour(juce::Slider::ColourIds::trackColourId,
                           findColour(ObjectView::ColourIds::Active).darker(0.5));
        
        m_slider.setColour(juce::Slider::ColourIds::thumbColourId,
                           findColour(ObjectView::ColourIds::Active));
        
        m_slider.setRange(0., 1.);
        
        m_slider.setVelocityModeParameters(1., 1, 0., false);
        
        m_slider.addListener(this);
        
        addAndMakeVisible(m_slider);
    }
    
    SliderView::~SliderView()
    {
        m_slider.removeListener(this);
    }
    
    void SliderView::declare()
    {
        Factory::add<SliderView>("slider", &SliderView::create);
    }
    
    std::unique_ptr<ObjectView> SliderView::create(model::Object & object_model)
    {
        return std::make_unique<SliderView>(object_model);
    }
    
    void SliderView::paint(juce::Graphics & g)
    {
        g.setColour(findColour(ObjectView::ColourIds::Background));
        g.fillRect(getLocalBounds());
        
        g.setColour(findColour(ObjectView::ColourIds::Outline));
        drawOutline(g);
    }
    
    void SliderView::mouseDown(juce::MouseEvent const& e)
    {
        m_slider.mouseDown(e.getEventRelativeTo(&m_slider));
    }
    
    void SliderView::mouseUp(juce::MouseEvent const& e)
    {
        m_slider.mouseUp(e.getEventRelativeTo(&m_slider));
    }
    
    void SliderView::mouseDrag(juce::MouseEvent const& e)
    {
        m_slider.mouseDrag(e.getEventRelativeTo(&m_slider));
    }
    
    void SliderView::sliderValueChanged(juce::Slider * slider)
    {
        setParameter("value", tool::Parameter(tool::Parameter::Type::Float, {slider->getValue()}));
    }
    
    void SliderView::resized()
    {
        if (getWidth() > getHeight() && m_slider.isVertical())
        {
            m_slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        }
        
        if (getHeight() > getWidth() && m_slider.isHorizontal())
        {
            m_slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        }
      
        m_slider.setBounds(getLocalBounds());
    }
    
    void SliderView::parameterChanged(std::string const& name, tool::Parameter const& parameter)
    {
        if (name == "value")
        {
            m_slider.setValue(parameter[0].getFloat(),
                              juce::NotificationType::dontSendNotification);
            repaint();
        }
    }
}
