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

#include <math.h>
#include <functional>

#include <KiwiApp.h>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Toggle.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_Controller/KiwiApp_ToggleView.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   TOGGLE VIEW                                    //
    // ================================================================================ //
    
    class ToggleView::Task final : public tool::Scheduler<>::Task
    {
    public: // methods
        
        Task(ToggleView & object, model::Toggle::Request request):
        tool::Scheduler<>::Task(),
        m_object(object),
        m_request(request)
        {
        }
        
        ~Task()
        {
        }
        
        void execute()
        {
            switch(m_request)
            {
                case model::Toggle::Request::Switch:
                {
                    m_object.m_is_on = !m_object.m_is_on;
                    break;
                }
                case model::Toggle::Request::SwitchOn:
                {
                    m_object.m_is_on = true;
                    break;
                }
                case model::Toggle::Request::SwitchOff:
                {
                    m_object.m_is_on = false;
                    break;
                }
            }
            
            m_object.repaint();
            
            std::shared_ptr<Task> task;
            m_object.m_tasks.pop(task);
        }
        
    private: // members
        
        ToggleView &            m_object;
        model::Toggle::Request  m_request;
    };

    ToggleView::ToggleView(model::Object & model):
    ObjectView(model),
    m_signal(model.getSignal<model::Toggle::Request, bool>(model::Toggle::Signal::Switch)),
    m_connection(m_signal.connect(std::bind(&ToggleView::toggleSwitched,
                                            this,
                                            std::placeholders::_1,
                                            std::placeholders::_2))),
    m_is_on(false),
    m_tasks(10)
    {
    }
    
    ToggleView::~ToggleView()
    {
        while(m_tasks.load_size() > 0)
        {
            std::shared_ptr<Task> task;
            m_tasks.pop(task);
            KiwiApp::useInstance().useScheduler().unschedule(task);
        }
    }
    
    void ToggleView::mouseDown(juce::MouseEvent const& e)
    {
        m_signal(model::Toggle::Request::Switch, true);
    }
    
    void ToggleView::toggleSwitched(model::Toggle::Request request, bool shall_request)
    {
        std::shared_ptr<Task> task(new Task(*this, request));
        m_tasks.push(task);
        KiwiApp::useInstance().useScheduler().schedule(task);
    }
    
    void ToggleView::paint(juce::Graphics & g)
    {
        g.fillAll(findColour(ObjectView::ColourIds::Background));
        
        g.setColour(findColour(ObjectView::ColourIds::Outline));
        
        drawOutline(g);
        
        if (m_is_on)
        {
            g.setColour(findColour(ObjectView::ColourIds::Active));
        }
        
        juce::Rectangle<int> bounds = getLocalBounds();
        
        double cross_stroke_width = 10. * (bounds.getWidth() / 100.);
        
        juce::Rectangle<int> inner_bounds = bounds.reduced(30. * bounds.getWidth() / 100.,
                                                           30. * bounds.getHeight() / 100.);
        
        g.drawLine(inner_bounds.getBottomLeft().getX(),
                   inner_bounds.getBottomLeft().getY(),
                   inner_bounds.getTopRight().getX(),
                   inner_bounds.getTopRight().getY(),
                   cross_stroke_width);
        
        g.drawLine(inner_bounds.getTopLeft().getX(),
                   inner_bounds.getTopLeft().getY(),
                   inner_bounds.getBottomRight().getX(),
                   inner_bounds.getBottomRight().getY(),
                   cross_stroke_width);
    }
}
