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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Slider.h>

#include <KiwiTool/KiwiTool_Scheduler.h>

#include <KiwiApp.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_Controller/KiwiApp_SliderView.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   SLIDER VIEW                                    //
    // ================================================================================ //
    
    class SliderView::Task final : public tool::Scheduler<>::Task
    {
    public: // methods
        
        Task(SliderView & object, double new_value):
        tool::Scheduler<>::Task(),
        m_object(object),
        m_new_value(new_value)
        {
        }
        
        ~Task()
        {
        }
        
        void execute()
        {
            m_object.m_slider.setValue(m_new_value, juce::NotificationType::dontSendNotification);
            m_object.repaint();
            
            std::shared_ptr<Task> task;
            m_object.m_tasks.pop(task);
        }
        
    private: // members
        
        SliderView &            m_object;
        double                  m_new_value;
    };
    
    SliderView::SliderView(model::Object & object_model):
    ObjectView(object_model),
    m_slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::NoTextBox),
    m_signal(object_model.getSignal<double>(model::Slider::Signal::ValueChanged)),
    m_connection(m_signal.connect(std::bind(&SliderView::valueChanged, this, std::placeholders::_1))),
    m_tasks(100)
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
        while(m_tasks.load_size() > 0)
        {
            std::shared_ptr<Task> task;
            m_tasks.pop(task);
            KiwiApp::useInstance().useScheduler().unschedule(task);
        }
        
        m_slider.removeListener(this);
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
        m_signal(slider->getValue());
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
    
    void SliderView::valueChanged(double new_value)
    {
        std::shared_ptr<Task> task(new Task(*this, new_value));
        m_tasks.push(task);
        KiwiApp::useInstance().useScheduler().schedule(task);
    }
}
