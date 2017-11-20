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

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ToggleView.h>
#include <KiwiApp_Patcher/KiwiApp_Factory.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                   TOGGLE VIEW                                    //
    // ================================================================================ //
    
    void ToggleView::declare()
    {
        Factory::add<ToggleView>("toggle", &ToggleView::create);
    }
    
    std::unique_ptr<ObjectView> ToggleView::create(model::Object & model)
    {
        return std::make_unique<ToggleView>(model);
    }

    ToggleView::ToggleView(model::Object & model):
    ObjectView(model),
    m_signal(model.getSignal<>(model::Toggle::Signal::OutputValue)),
    m_is_on(false)
    {
    }
    
    ToggleView::~ToggleView()
    {
    }
    
    void ToggleView::mouseDown(juce::MouseEvent const& e)
    {
        setParameter("value", tool::Parameter(tool::Parameter::Type::Int, {!m_is_on}));
        m_signal();
    }
    
    void ToggleView::parameterChanged(std::string const& name, tool::Parameter const& param)
    {
        if (name == "value")
        {
            m_is_on = static_cast<bool>(param[0].getInt());
            repaint();
        }
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
