/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "../KiwiApp.h"
#include "KiwiApp_LinkView.h"
#include "KiwiApp_PatcherView.h"

namespace kiwi
{
    // ================================================================================ //
    //                                   LINK VIEW BASE                                 //
    // ================================================================================ //
    
    void LinkViewBase::updateBounds()
    {
        const auto inlet = m_last_inlet_pos;
        const auto outlet = m_last_outlet_pos;
        const juce::Rectangle<int> link_bounds(outlet, inlet);
        
        juce::Point<float> start = outlet.translated(0.f, 2.f).toFloat();
        juce::Point<float> end = inlet.translated(0.f, -1.f).toFloat();
        
        juce::Path path;
        
        if(link_bounds.getWidth() == 0)
        {
            path.startNewSubPath(start.x, start.y);
            path.lineTo(end.x, end.y);
        }
        else
        {
            const float width = link_bounds.getWidth();
            const float height = link_bounds.getHeight();
            
            const float min = std::min<float>(width, height);
            const float max = std::max<float>(width, height);
            
            const float max_shift_y = 20.f;
            const float max_shift_x = 20.f;
            
            const float shift_y = std::min<float>(max_shift_y, max * 0.5);
            
            const float shift_x = ((start.y >= end.y)
                                   ? std::min<float>(max_shift_x, min * 0.5)
                                   : 0.f) * ((start.x < end.x) ? -1. : 1.);
            
            const juce::Point<float> ctrl_pt1 { start.x - shift_x, start.y + shift_y };
            const juce::Point<float> ctrl_pt2 { end.x + shift_x, end.y - shift_y };
            
            path.startNewSubPath(start);
            path.cubicTo(ctrl_pt1, ctrl_pt2, end);
        }
        
        setBounds(path.getBounds().toNearestInt().expanded(2, 2));
        path.applyTransform(juce::AffineTransform::translation(-1 * getX(), -1 * getY()));
        m_path = path;
    }
    
    // ================================================================================ //
    //                                     LINK VIEW                                    //
    // ================================================================================ //
    
    LinkView::LinkView(PatcherView& patcherview, model::Link& link_m)
    : m_patcherview(patcherview)
    , m_model(&link_m)
    {
        auto& sender_object_m = m_model->getSenderObject();
        auto& receiver_object_m = m_model->getReceiverObject();
        
        ObjectFrame* jbox_sender = m_patcherview.getObject(sender_object_m);
        ObjectFrame* jbox_receiver = m_patcherview.getObject(receiver_object_m);
        
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
    
    LinkView::~LinkView()
    {
        auto& sender_object_m = m_model->getSenderObject();
        ObjectFrame* jbox_sender = m_patcherview.getObject(sender_object_m);
        if(jbox_sender)
        {
            jbox_sender->removeComponentListener(this);
        }
        
        auto& receiver_object_m = m_model->getReceiverObject();
        ObjectFrame* jbox_receiver = m_patcherview.getObject(receiver_object_m);
        if(jbox_receiver)
        {
            jbox_receiver->removeComponentListener(this);
        }
    }
    
    model::Link& LinkView::getModel() const
    {
        return *m_model;
    }
    
    bool LinkView::isSelected() const noexcept
    {
        return m_selected.on_this_view;
    }
    
    void LinkView::linkChanged(model::Link& link)
    {
        ;
    }
    
    void LinkView::objectChanged(model::Object& object)
    {
        ;
    }
    
    void LinkView::localSelectionChanged(bool selected)
    {
        if(m_selected.on_this_view != selected)
        {
            m_selected.on_this_view = selected;
            repaint();
        }
    }
    
    void LinkView::distantSelectionChanged(std::set<uint64_t> distant_user_id_selection)
    {
        const bool was_selected_in_another_view = m_selected.in_another_view;
        const bool was_selected_by_another_user = m_selected.by_another_user;
        
        bool should_repaint = false;
        if(distant_user_id_selection.empty())
        {
            should_repaint = (was_selected_in_another_view || was_selected_by_another_user);
            m_selected.in_another_view = false;
            m_selected.by_another_user = false;
        }
        else
        {
            const auto user_id = KiwiApp::userID();
            for(auto distant_user_id : distant_user_id_selection)
            {
                if(distant_user_id == user_id)
                {
                    m_selected.in_another_view = true;
                }
                else
                {
                    m_selected.by_another_user = true;
                }
                
                if(m_selected.in_another_view && m_selected.by_another_user)
                {
                    break;
                }
            }
            
            should_repaint = ((was_selected_in_another_view != m_selected.in_another_view)
                              || (was_selected_in_another_view != m_selected.by_another_user));
        }
        
        if(should_repaint)
        {
            repaint();
        }
    }
    
    void LinkView::componentMovedOrResized(Component& component,
                                           bool /*was_moved*/, bool /*was_resized*/)
    {
        if(auto* box = dynamic_cast<ObjectFrame*>(&component))
        {
            if(&box->getModel() == &m_model->getSenderObject())
            {
                m_last_outlet_pos = box->getOutletPatcherPosition(m_model->getSenderIndex());
                updateBounds();
            }
            else if(&box->getModel() == &m_model->getReceiverObject())
            {
                m_last_inlet_pos = box->getInletPatcherPosition(m_model->getReceiverIndex());
                updateBounds();
            }
        }
    }
    
    bool LinkView::isSignal() const
    {
        return m_model->isSignal();
    }
    
    void LinkView::paint(juce::Graphics & g)
    {
        const bool selected_by_other = (m_selected.by_another_user
                                        || m_selected.in_another_view);
        
        const float stroke = (isSignal() ? 2.f : 1.5f);
        
        juce::Colour bgcolor;
        if(m_selected.on_this_view)
        {
            bgcolor = findColour(PatcherView::ColourIds::Selection);
        }
        else if (m_selected.by_another_user)
        {
            bgcolor = findColour(PatcherView::ColourIds::SelectionOtherUser);
        }
        else if (m_selected.in_another_view)
        {
            bgcolor = findColour(PatcherView::ColourIds::SelectionOtherView);
        }
        else
        {
            bgcolor = findColour(isSignal()
                                 ? LinkView::ColourIds::SignalBackground
                                 : LinkView::ColourIds::ControlBackground);
        }
        
        g.setColour(bgcolor);
        g.strokePath(m_path, juce::PathStrokeType(stroke));
        
        if(m_selected.on_this_view && selected_by_other)
        {
            g.setColour(findColour(m_selected.by_another_user
                                   ? PatcherView::ColourIds::SelectionOtherUser
                                   : PatcherView::ColourIds::SelectionOtherView)
                        .withAlpha(1.f));
            
            juce::Path path;
            const juce::PathStrokeType path_stroke(stroke * 0.5);
            float const dashed_length[2] {10.f, 10.f};
            path_stroke.createDashedStroke(path, m_path, dashed_length, 2);
            g.strokePath(path, path_stroke);
        }
    }
    
    bool LinkView::hitTest(juce::Point<int> const& pt, HitTester& hit) const
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
    
    bool LinkView::hitTest(juce::Rectangle<float> const& rect)
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
    //                                 LINK VIEW CREATOR                                //
    // ================================================================================ //
    
    LinkViewCreator::LinkViewCreator(ObjectFrame& binded_object,
                               const size_t index,
                               bool is_sender,
                               juce::Point<int> dragged_pos) :
    m_binded_object(binded_object),
    m_index(index),
    m_is_sender(is_sender)
    {
        m_last_outlet_pos = m_is_sender ? m_binded_object.getOutletPatcherPosition(m_index) : dragged_pos;
        m_last_inlet_pos = m_is_sender ? dragged_pos : m_binded_object.getInletPatcherPosition(m_index);
        updateBounds();
    }
    
    void LinkViewCreator::setEndPosition(juce::Point<int> const& pos)
    {
        if(m_is_sender)
        {
            m_last_inlet_pos = pos;
        }
        else
        {
            m_last_outlet_pos = pos;
        }
        
        updateBounds();
    }
    
    juce::Point<int> LinkViewCreator::getEndPosition() const noexcept
    {
        return m_is_sender ? m_last_inlet_pos : m_last_outlet_pos;
    }
    
    void LinkViewCreator::paint(juce::Graphics & g)
    {
        const juce::Colour link_color = juce::Colour::fromFloatRGBA(0.2, 0.2, 0.2, 1.);
        g.setColour(link_color);
        g.strokePath(m_path, juce::PathStrokeType(1.5f));
    }
}
