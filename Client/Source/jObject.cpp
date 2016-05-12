/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#include <KiwiEngine/KiwiDocumentManager.hpp>

#include "jObject.hpp"

namespace kiwi
{
    jObject::jObject(model::Object& object_m) :
    m_model(&object_m),
    m_io_color(0.3, 0.3, 0.3)
    {
        setSize(60, 20);
        m_inlets = m_model->getNumberOfInlets();
        m_outlets = m_model->getNumberOfOutlets();
        
        setTopLeftPosition(juce::Point<int>(m_model->getX(), m_model->getY()));
    }
    
    jObject::~jObject()
    {
        ;
    }
    
    void jObject::objectChanged(model::Patcher::View& view, model::Object& object)
    {
        bool need_redraw = false;
        
        if(view.added())
        {
            m_model = &object;
        }
        
        if(object.inletsChanged())
        {
            m_inlets = object.getNumberOfInlets();
            need_redraw = true;
        }
        
        if(object.outletsChanged())
        {
            m_outlets = object.getNumberOfOutlets();
            need_redraw = true;
        }
        
        if(object.positionChanged())
        {
            setTopLeftPosition(juce::Point<int>(object.getX(), object.getY()));
            need_redraw = true;
        }
        
        if(view.removed())
        {
            m_model = nullptr;
            need_redraw = false;
        }
        
        if(need_redraw)
        {
            repaint();
        }
    }
    
    void jObject::paint(juce::Graphics & g)
    {
        auto bounds = getLocalBounds();
        
        g.setColour(juce::Colours::white);
        g.fillRect(bounds);
        
        g.setColour(juce::Colours::black);
        g.drawRect(bounds);
        
        if(m_model)
        {
            g.drawFittedText(m_model->getText(), bounds.reduced(5), juce::Justification::centredLeft, 1);
            
            drawInletsOutlets(g);
        }
    }
    
    void jObject::drawInletsOutlets(juce::Graphics & g)
    {
        const juce::Rectangle<int> bounds = getLocalBounds();
        
        g.setColour(m_io_color);
        
        for(unsigned int i = 0; i < m_inlets; ++i)
        {
            g.fillRect(getInletLocalBounds(i, bounds));
        }
        
        for(unsigned int i = 0; i < m_outlets; ++i)
        {
            g.fillRect(getOutletLocalBounds(i, bounds));
        }
    }
    
    juce::Point<int> jObject::getInletPatcherPosition(const size_t index) const
    {
        return getPosition() + getInletLocalBounds(index, getLocalBounds()).getCentre();
    }
    
    juce::Point<int> jObject::getOutletPatcherPosition(const size_t index) const
    {
        return getPosition() + getOutletLocalBounds(index, getLocalBounds()).getCentre();
    }
    
    juce::Rectangle<int> jObject::getInletLocalBounds(const size_t index,
                                                      juce::Rectangle<int> const& object_bounds) const
    {
        juce::Rectangle<int> rect;
        
        if(m_inlets > 0 && index < m_inlets)
        {
            if(m_inlets == 1 && index == 0)
            {
                rect.setBounds(object_bounds.getX(), object_bounds.getY(), m_io_width, m_io_height);
            }
            
            if(m_inlets > 1)
            {
                const double ratio = (object_bounds.getWidth() - m_io_width) / (double)(m_inlets - 1);
                rect.setBounds(object_bounds.getX() + ratio * index, object_bounds.getY(),
                               m_io_width, m_io_height);
            }
        }
        
        return rect;
    }
    
    juce::Rectangle<int> jObject::getOutletLocalBounds(const size_t index,
                                                       juce::Rectangle<int> const& object_bounds) const
    {
        juce::Rectangle<int> rect;
        
        if(m_outlets > 0 && index < m_outlets)
        {
            if(m_outlets == 1 && index == 0)
            {
                rect.setBounds(object_bounds.getX(),
                               object_bounds.getY() + object_bounds.getHeight() - m_io_height,
                               m_io_width, m_io_height);
            }
            
            if(m_outlets > 1)
            {
                const double ratio = (object_bounds.getWidth() - m_io_width) / (double)(m_outlets - 1);
                rect.setBounds(object_bounds.getX() + ratio * index,
                               object_bounds.getY() + object_bounds.getHeight() - m_io_height,
                               m_io_width, m_io_height);
            }
        }
        
        return rect;
    }
    
    void jObject::mouseDown(juce::MouseEvent const& event)
    {
        // signal engine
        m_model->signalTrigger();
    }
    
    void jObject::mouseDrag(juce::MouseEvent const& event)
    {
        ;
    }
}
