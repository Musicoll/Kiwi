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

#include <chrono>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_BangView.h>
#include <KiwiApp_Patcher/KiwiApp_Factory.h>

namespace kiwi {
 
    // ================================================================================ //
    //                                   BANG VIEW                                      //
    // ================================================================================ //
    
    void BangView::declare()
    {
        Factory::add<BangView>("bang", &BangView::create);
    }
    
    std::unique_ptr<ObjectView> BangView::create(model::Object & model)
    {
        return std::make_unique<BangView>(model);
    }
    
    BangView::BangView(model::Object & model):
    ObjectView(model),
    m_signal(model.getSignal<>(model::Bang::Signal::TriggerBang)),
    m_connection(m_signal.connect(std::bind(&BangView::signalTriggered, this))),
    m_active(false),
    m_mouse_down(false)
    {
    }
    
    BangView::~BangView()
    {
    }
    
    void BangView::paint(juce::Graphics & g)
    {
        g.fillAll(findColour(ObjectView::ColourIds::Background));
        
        g.setColour(findColour(ObjectView::ColourIds::Outline));
        
        juce::Rectangle<int> bounds = getLocalBounds();
        
        double circle_outer = 80 * bounds.getWidth() / 100.;
        
        double circle_thickness = 10 * bounds.getWidth() / 100.;
        
        g.drawEllipse(bounds.reduced(bounds.getWidth() - circle_outer).toFloat(), circle_thickness);
        
        drawOutline(g);
        
        if (m_mouse_down || m_active)
        {
            g.setColour(findColour(ObjectView::ColourIds::Active));
            g.fillEllipse(bounds.reduced(bounds.getWidth() - circle_outer + circle_thickness).toFloat());
        }
    }
    
    void BangView::mouseDown(juce::MouseEvent const& e)
    {
        m_mouse_down = true;
        repaint();
        
        m_signal();
    }
    
    void BangView::mouseUp(juce::MouseEvent const& e)
    {
        m_mouse_down = false;
        repaint();
    }
    
    void BangView::flash()
    {
        if (!m_active)
        {
            m_active = true;
            repaint();
        }
        
        schedule([this]()
        {
            m_active = false;
            repaint();
        }, std::chrono::milliseconds(150));
    }
    
    void BangView::signalTriggered()
    {
        defer([this]()
        {
            flash();
        });
    }
}
