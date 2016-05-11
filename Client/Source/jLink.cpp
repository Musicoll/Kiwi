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

#include "jLink.hpp"

#include "jPatcher.hpp"

namespace kiwi
{
    jLink::jLink(jPatcher const& jpatcher, model::Link& link_m) :
    m_jpatcher(jpatcher),
    m_model(&link_m)
    {
        setInterceptsMouseClicks(false, false);
    }
    
    jLink::~jLink()
    {
        ;
    }
    
    void jLink::linkChanged(model::Link& link)
    {
        if(link.added())
        {
            m_model = &link;
        }
        
        if(link.removed())
        {
            m_model = nullptr;
        }
    }
    
    void jLink::objectChanged(model::Object& object)
    {
        if(!object.removed() && object.positionChanged())
        {
            auto& sender_object = m_model->getSenderObject();
            auto& receiver_object = m_model->getReceiverObject();
            
            if(&object == &sender_object)
            {
                auto jobject = m_jpatcher.getjObject(sender_object);
                if(jobject)
                {
                    m_last_outlet_pos = jobject->getOutletPatcherPosition(m_model->getSenderIndex());
                    updateBounds();
                }
            }
            else if(&object == &receiver_object)
            {
                auto jobject = m_jpatcher.getjObject(receiver_object);
                if(jobject)
                {
                    m_last_inlet_pos = jobject->getInletPatcherPosition(m_model->getReceiverIndex());
                    updateBounds();
                }
            }
        }
    }
    
    void jLink::updateBounds()
    {
        const juce::Rectangle<int> bounds(m_last_inlet_pos, m_last_outlet_pos);
        
        setBounds(bounds.expanded(10));
    }
    
    void jLink::paint(juce::Graphics & g)
    {
        /* // draw link bounds
        const auto bounds = getLocalBounds();
        g.setColour(juce::Colours::black);
        g.drawRect(bounds);
        */
        
        // draw link
        const juce::Point<int> edge_pt_width(3, 3);
        const juce::Point<int> comp_pos = getPosition();
        
        const juce::Point<int> local_inlet_pos(m_last_inlet_pos - comp_pos);
        const juce::Point<int> local_outlet_pos(m_last_outlet_pos - comp_pos);
        
        g.setColour(juce::Colours::black);
        g.drawLine(local_inlet_pos.x, local_inlet_pos.y, local_outlet_pos.x, local_outlet_pos.y, 1.f);
        
        /* // draw edge points
        const juce::Rectangle<int> link_start(local_inlet_pos - edge_pt_width, local_inlet_pos + edge_pt_width);
        const juce::Rectangle<int> link_end(local_outlet_pos - edge_pt_width, local_outlet_pos + edge_pt_width);
        g.setColour(juce::Colours::red);
        g.fillEllipse(link_start.toFloat());
        g.fillEllipse(link_end.toFloat());
        */
    }
}
