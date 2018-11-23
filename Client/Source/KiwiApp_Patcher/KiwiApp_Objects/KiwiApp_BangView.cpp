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
    
    BangView::BangView(model::Object & model)
    : ObjectView(model)
    , m_trigger_signal(model.getSignal<>(model::Bang::Signal::TriggerBang))
    , m_flash_signal(model.getSignal<>(model::Bang::Signal::FlashBang))
    , m_trigger_connection(m_trigger_signal.connect(std::bind(&BangView::signalTriggered, this)))
    , m_flash_connection(m_flash_signal.connect(std::bind(&BangView::signalTriggered, this)))
    , m_active(false)
    , m_mouse_down(false)
    {
        setMinimumSize(20.f, 20.f);
        setFixedAspectRatio(1.f);
    }
    
    BangView::~BangView()
    {}
    
    void BangView::paint(juce::Graphics & g)
    {
        g.fillAll(findColour(ObjectView::ColourIds::Background));
        g.setColour(findColour(ObjectView::ColourIds::Outline));
        drawOutline(g);
        
        const auto bounds = getLocalBounds().toFloat();
        const auto width = std::max(bounds.getWidth(), bounds.getHeight());
        
        const float circle_outer = 80.f * (width * 0.01f);
        const float circle_thickness = 10.f * (width * 0.01f);
        
        g.drawEllipse(bounds.reduced(width - circle_outer), circle_thickness);
        
        if (m_mouse_down || m_active)
        {
            g.setColour(findColour(ObjectView::ColourIds::Active));
            g.fillEllipse(bounds.reduced(width - circle_outer + circle_thickness));
        }
    }
    
    void BangView::mouseDown(juce::MouseEvent const& e)
    {
        m_mouse_down = true;
        repaint();
        
        m_trigger_signal();
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
            
            schedule([this]() {
                m_active = false;
                repaint();
            }, std::chrono::milliseconds(150));
        }
    }
    
    void BangView::signalTriggered()
    {
        defer([this]()
        {
            flash();
        });
    }
}
