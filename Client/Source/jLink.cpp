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
#include "Application.hpp"
#include "jPatcherHelper.hpp"

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
    
    bool jLink::isSelected() const noexcept
    {
        return m_is_selected;
    }
    
    void jLink::linkChanged(model::Link& link)
    {
        ;
    }
    
    void jLink::objectChanged(model::Object& object)
    {
        ;
    }
    
    void jLink::localSelectionChanged(bool selected)
    {
        if(m_is_selected != selected)
        {
            m_is_selected = selected;
            repaint();
        }
    }
    
    void jLink::distantSelectionChanged(std::set<uint64_t> distant_user_id_selection)
    {
        m_distant_selection = distant_user_id_selection;
        repaint();
    }
    
    void jLink::updateBounds()
    {
        const juce::Rectangle<int> link_bounds(m_last_inlet_pos, m_last_outlet_pos);
        const juce::Rectangle<int> new_bounds = link_bounds.expanded(20);
        
        const juce::Point<int> comp_pos = new_bounds.getPosition();
        
        const juce::Point<int> local_inlet_pos(m_last_inlet_pos - comp_pos);
        const juce::Point<int> local_outlet_pos(m_last_outlet_pos - comp_pos);
        
        const Point<float> start_point = local_outlet_pos.translated(0.f, 2.f).toFloat();
        const Point<float> end_point = local_inlet_pos.translated(0.f, -1.f).toFloat();
        
        const float max_shift = std::min(link_bounds.getWidth(), link_bounds.getHeight());
        const float shift = (max_shift < 10) ? max_shift * 0.2 : (max_shift * 0.5);
        
        const Point<float> ctrl_pt1 { start_point.x, static_cast<float>(start_point.y + shift) };
        const Point<float> ctrl_pt2 { end_point.x, static_cast<float>(end_point.y - shift) };
        
        m_path.clear();
        m_path.startNewSubPath(start_point.x, start_point.y);
        m_path.cubicTo(ctrl_pt1, ctrl_pt2, end_point);
        
        setBounds(new_bounds);
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
        
        const juce::Colour link_color = Colour::fromFloatRGBA(0.2, 0.2, 0.2, 1.);
        const juce::Colour selection_color = Colour::fromFloatRGBA(0., 0.5, 1., 1.);
        const juce::Colour other_view_selected_color = Colour::fromFloatRGBA(0.8, 0.3, 0.3, 1.);
        
        const bool selected = m_is_selected;
        const bool other_selected = ! m_distant_selection.empty();
        const bool other_view_selected = (other_selected &&
                                          m_distant_selection.find(KiwiApp::userID())
                                          != m_distant_selection.end());
        
        g.setColour(link_color);
        g.strokePath(m_path, juce::PathStrokeType(2.f));
        
        juce::Colour inner_color = link_color.contrasting(0.4);
        
        if(selected || other_view_selected)
        {
            inner_color = selected ? selection_color : other_view_selected_color;
        }
        
        g.setColour(inner_color);
        g.strokePath(m_path, juce::PathStrokeType(1.f));
        
        /* // draw edge points
        const juce::Point<int> edge_pt_width(3, 3);
        const juce::Rectangle<int> link_start(local_inlet_pos - edge_pt_width, local_inlet_pos + edge_pt_width);
        const juce::Rectangle<int> link_end(local_outlet_pos - edge_pt_width, local_outlet_pos + edge_pt_width);
        g.setColour(juce::Colours::red);
        g.fillEllipse(link_start.toFloat());
        g.fillEllipse(link_end.toFloat());
        */
    }
    
    bool jLink::hitTest(juce::Point<int> const& pt, HitTester& hit) const
    {
        const float max_distance = 3.f;
        juce::Point<float> point_on_line;
                
        const float distance_from_start = m_path.getNearestPoint(pt.toFloat(), point_on_line);
        const juce::Point<float> point_on_path = m_path.getPointAlongPath(distance_from_start);
        const float distance = point_on_path.getDistanceFrom(pt.toFloat());
        
        if(distance <= max_distance)
        {
            return true;
        }
        
        return false;
    }
    
    bool jLink::hitTest(juce::Rectangle<float> const& rect)
    {
        juce::Path p = m_path;
        p.applyTransform(juce::AffineTransform::translation(getPosition()));
        
        return (rect.contains(m_last_outlet_pos.toFloat()) || rect.contains(m_last_inlet_pos.toFloat())
                || p.intersectsLine({rect.getPosition(), rect.getTopRight()})
                || p.intersectsLine({rect.getTopRight(), rect.getBottomRight()})
                || p.intersectsLine({rect.getBottomLeft(), rect.getBottomRight()})
                || p.intersectsLine({rect.getPosition(), rect.getBottomLeft()}));
    }
    
    // ================================================================================ //
    //                                   JLINK CREATOR                                  //
    // ================================================================================ //
    
    jLinkCreator::jLinkCreator(jObject& binded_object,
                               const size_t index,
                               bool is_sender,
                               juce::Point<int> dragged_pos) :
    m_binded_object(binded_object),
    m_index(index),
    m_is_sender(is_sender)
    {
        m_outlet_pos = m_is_sender ? m_binded_object.getOutletPatcherPosition(m_index) : dragged_pos;
        m_inlet_pos = m_is_sender ? dragged_pos : m_binded_object.getInletPatcherPosition(m_index);
        updateBounds();
    }
    
    void jLinkCreator::setEndPosition(juce::Point<int> const& pos)
    {
        if(m_is_sender)
        {
            m_inlet_pos = pos;
        }
        else
        {
            m_outlet_pos = pos;
        }
        
        updateBounds();
    }
    
    juce::Point<int> jLinkCreator::getEndPosition() const noexcept
    {
        return m_is_sender ? m_inlet_pos : m_outlet_pos;
    }
    
    void jLinkCreator::paint(juce::Graphics & g)
    {
        const juce::Colour link_color = Colour::fromFloatRGBA(0.2, 0.2, 0.2, 1.);
        g.setColour(link_color);
        g.strokePath(m_path, juce::PathStrokeType(1.5f));
    }
    
    void jLinkCreator::updateBounds()
    {
        const juce::Rectangle<int> link_bounds(m_outlet_pos, m_inlet_pos);
        const juce::Rectangle<int> new_bounds = link_bounds.expanded(20);
        
        const juce::Point<int> comp_pos = new_bounds.getPosition();
        
        const juce::Point<int> local_inlet_pos(m_inlet_pos - comp_pos);
        const juce::Point<int> local_outlet_pos(m_outlet_pos - comp_pos);
        
        const Point<float> start_point = local_outlet_pos.translated(0.f, 2.f).toFloat();
        const Point<float> end_point = local_inlet_pos.translated(0.f, -1.f).toFloat();
        
        const float max_shift = std::min(link_bounds.getWidth(), link_bounds.getHeight());
        const float shift = (max_shift < 10) ? max_shift * 0.2 : (max_shift * 0.5);
        
        const Point<float> ctrl_pt1 { start_point.x, static_cast<float>(start_point.y + shift) };
        const Point<float> ctrl_pt2 { end_point.x, static_cast<float>(end_point.y - shift) };
        
        m_path.clear();
        m_path.startNewSubPath(start_point.x, start_point.y);
        m_path.cubicTo(ctrl_pt1, ctrl_pt2, end_point);
        
        setBounds(new_bounds);
    }
}
