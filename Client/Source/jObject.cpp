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

#include "Application.hpp"
#include "jObject.hpp"

namespace kiwi
{
    jObject::jObject(model::Object& object_m) :
    m_model(&object_m),
    m_io_color(0.3, 0.3, 0.3),
    m_selection_width(2),
    m_is_selected(false)
    {
        m_inlets = m_model->getNumberOfInlets();
        m_outlets = m_model->getNumberOfOutlets();
        
        updateBounds();
    }
    
    jObject::~jObject()
    {
        ;
    }
    
    void jObject::updateBounds()
    {
        const juce::Rectangle<int> model_bounds(m_model->getX(), m_model->getY(), 60, 20);
        
        const auto new_bounds = model_bounds.expanded(m_selection_width, m_selection_width);
        
        m_local_box_bounds = model_bounds.withPosition(model_bounds.getX() - new_bounds.getX(),
                                                       model_bounds.getY() - new_bounds.getY());
        
        setBounds(new_bounds);
    }
    
    void jObject::paint(juce::Graphics & g)
    {
        const auto bounds = getLocalBounds();
        const auto box_bounds = m_local_box_bounds;
        
        const juce::Colour selection_color = Colour::fromFloatRGBA(0., 0.5, 0.9, 0.8);
        
        /*
        for(auto user_id : m_distant_selection)
        {
            // auto color = getColorForUserId(user_id);
        }
        */
        
        const bool selected = m_is_selected;
        const bool other_selected = ! m_distant_selection.empty();
        const bool other_view_selected = (other_selected &&
                                          m_distant_selection.find(KiwiApp::userID())
                                          != m_distant_selection.end());
        
        if(selected || other_view_selected)
        {
            const juce::Colour color = selected ? selection_color : selection_color.withAlpha(0.3f);
            g.setColour(color);
            g.drawRect(bounds.reduced(m_selection_width*0.5), m_selection_width*0.5);
            
            g.setColour(juce::Colours::black.darker(0.2));
            g.drawRect(bounds, 1);
        }

        g.setColour(juce::Colours::white);
        g.fillRect(box_bounds);
        
        g.setColour(juce::Colours::black);
        g.drawRect(box_bounds);
        
        g.drawFittedText(m_model->getText(), box_bounds.reduced(5), juce::Justification::centredLeft, 1);
        
        drawInletsOutlets(g);
    }
    
    void jObject::drawInletsOutlets(juce::Graphics & g)
    {
        const juce::Rectangle<int> bounds = m_local_box_bounds;
        
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
    
    bool jObject::hitTest(int x, int y)
    {
        return m_local_box_bounds.contains(x, y);
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
            updateBounds();
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

    void jObject::localSelectionChanged(bool selected)
    {
        if(m_is_selected != selected)
        {
            m_is_selected = selected;
            repaint();
        }
    }
    
    void jObject::distantSelectionChanged(std::set<uint64_t> distant_user_id_selection)
    {
        m_distant_selection = distant_user_id_selection;
        repaint();
    }
    
    juce::Point<int> jObject::getInletPatcherPosition(const size_t index) const
    {
        return getPosition() + getInletLocalBounds(index, m_local_box_bounds).getCentre();
    }
    
    juce::Point<int> jObject::getOutletPatcherPosition(const size_t index) const
    {
        return getPosition() + getOutletLocalBounds(index, m_local_box_bounds).getCentre();
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
