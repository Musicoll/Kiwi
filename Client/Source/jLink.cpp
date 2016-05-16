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
        auto& sender_object_m = m_model->getSenderObject();
        auto& receiver_object_m = m_model->getReceiverObject();
        
        jObject* jbox_sender = m_jpatcher.getObject(sender_object_m);
        jObject* jbox_receiver = m_jpatcher.getObject(receiver_object_m);
        
        if(jbox_sender && jbox_receiver)
        {
            m_last_outlet_pos = jbox_sender->getOutletPatcherPosition(m_model->getSenderIndex());
            m_last_inlet_pos  = jbox_receiver->getInletPatcherPosition(m_model->getReceiverIndex());
            
            jbox_sender->addComponentListener(this);
            jbox_receiver->addComponentListener(this);
            updateBounds();
        }

        setInterceptsMouseClicks(false, false);
    }
    
    jLink::~jLink()
    {
        auto& sender_object_m = m_model->getSenderObject();
        jObject* jbox_sender = m_jpatcher.getObject(sender_object_m);
        if(jbox_sender)
        {
            jbox_sender->removeComponentListener(this);
        }
        
        auto& receiver_object_m = m_model->getReceiverObject();
        jObject* jbox_receiver = m_jpatcher.getObject(receiver_object_m);
        if(jbox_receiver)
        {
            jbox_receiver->removeComponentListener(this);
        }
    }
    
    void jLink::linkChanged(model::Link& link)
    {
        ;
    }
    
    void jLink::objectChanged(model::Object& object)
    {
        /*
        if(!object.removed() && object.positionChanged())
        {
            auto& sender_object = m_model->getSenderObject();
            auto& receiver_object = m_model->getReceiverObject();
            
            if(&object == &sender_object)
            {
                auto jobject = m_jpatcher.getObject(sender_object);
                if(jobject)
                {
                    m_last_outlet_pos = jobject->getOutletPatcherPosition(m_model->getSenderIndex());
                    updateBounds();
                }
            }
            else if(&object == &receiver_object)
            {
                auto jobject = m_jpatcher.getObject(receiver_object);
                if(jobject)
                {
                    m_last_inlet_pos = jobject->getInletPatcherPosition(m_model->getReceiverIndex());
                    updateBounds();
                }
            }
        }
        */
    }
    
    void jLink::updateBounds()
    {
        const juce::Rectangle<int> bounds(m_last_inlet_pos, m_last_outlet_pos);
        
        setBounds(bounds.expanded(10));
    }
    
    void jLink::componentMovedOrResized(Component& component, bool was_moved, bool was_resized)
    {
        jObject* jbox = dynamic_cast<jObject*>(&component);
        if(jbox)
        {
            if(&jbox->getModel() == &m_model->getSenderObject())
            {
                m_last_outlet_pos = jbox->getOutletPatcherPosition(m_model->getSenderIndex());
                updateBounds();
            }
            else if(&jbox->getModel() == &m_model->getReceiverObject())
            {
                m_last_inlet_pos = jbox->getInletPatcherPosition(m_model->getReceiverIndex());
                updateBounds();
            }
        }
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
    
    bool jLink::hitTest(juce::Point<int> const& pt, HitTester& hit) const
    {
        return false;
    }
    
    bool jLink::hitTest(juce::Rectangle<int> const& rect)
    {
        return false;
    }
}
