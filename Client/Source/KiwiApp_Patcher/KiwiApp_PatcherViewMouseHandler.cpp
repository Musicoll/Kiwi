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

#include <algorithm>

#include <KiwiModel/KiwiModel_Object.h>
#include <KiwiModel/KiwiModel_DocumentManager.h>

#include <KiwiApp.h>
#include <KiwiApp_Patcher/KiwiApp_PatcherView.h>
#include <KiwiApp_Patcher/KiwiApp_LinkView.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectFrame.h>
#include <KiwiApp_Patcher/KiwiApp_PatcherViewHitTester.h>

namespace kiwi
{
    // ================================================================================ //
    //                                    HITTESTER                                     //
    // ================================================================================ //
    
    MouseHandler::MouseHandler(PatcherView & patcher_view):
    m_patcher_view(patcher_view),
    m_current_action(Action::None),
    m_last_drag(),  
    m_mousedown_bounds(),
    m_direction()
    {
    }
    
    MouseHandler::~MouseHandler()
    {
    }
    
    MouseHandler::Action MouseHandler::getCurrentAction()
    {
        return m_current_action;
    }
    
    void MouseHandler::startAction(Action action, juce::MouseEvent const& e)
    {
        m_current_action = action;
        
        HitTester& hit_tester = m_patcher_view.m_hittester;
        
        switch (m_current_action)
        {
            case Action::CopyOnDrag:
            {
                ObjectFrame & object = *hit_tester.getObject();
                
                if (!m_patcher_view.isSelected(object))
                {
                    m_patcher_view.selectObjectOnly(object);
                }
                
                break;
            }
            case Action::Object:
            {
                ObjectFrame & object = *hit_tester.getObject();
                
                object.mouseDown(e.getEventRelativeTo(&object));
                
                break;
            }
            case Action::CreateLink:
            {
                m_patcher_view.unselectAll();
                
                auto& object = *hit_tester.getObject();
                
                const size_t index = hit_tester.getIndex();
                
                const bool is_sender = hit_tester.getZone() == HitTester::Zone::Outlet;
                
                m_patcher_view.m_link_creator.reset(new LinkViewCreator(object,
                                                                        index,
                                                                        is_sender,
                                                                        e.getPosition()));
                
                m_patcher_view.addAndMakeVisible(*m_patcher_view.m_link_creator);
                
                break;
            }
            case Action::Lasso:
            {
                m_patcher_view.m_lasso.begin(e.getPosition(), e.mods.isShiftDown());
                
                break;
            }
            case Action::MoveObjects:
            {
                KiwiApp::commandStatusChanged();
                
                model::DocumentManager::startCommitGesture(m_patcher_view.m_patcher_model);
                
                break;
            }
            case Action::PopupMenu:
            {
                if(hit_tester.objectTouched() && hit_tester.getZone() == HitTester::Zone::Inside)
                {
                    ObjectFrame & object = *hit_tester.getObject();
                    
                    if (!m_patcher_view.isSelected(object))
                    {
                        m_patcher_view.selectObject(object);
                    }
                    
                    const auto pos = e.getPosition() - m_patcher_view.m_viewport.getOriginPosition();
                    
                    m_patcher_view.showObjectPopupMenu(object, pos);
                }
                else if (hit_tester.linkTouched() && hit_tester.getZone() == HitTester::Zone::Inside)
                {
                    LinkView& link_view = *hit_tester.getLink();
                    
                    if (!m_patcher_view.isSelected(link_view))
                    {
                        m_patcher_view.selectLink(link_view);
                    }
                    
                    const auto pos = e.getPosition() - m_patcher_view.m_viewport.getOriginPosition();
                    
                    m_patcher_view.showLinkPopupMenu(link_view, pos);
                }
                else if (hit_tester.patcherTouched())
                {
                    m_patcher_view.showPatcherPopupMenu(e.getPosition() - m_patcher_view.m_viewport.getOriginPosition());
                }
                
                break;
            }
            case Action::SwitchSelection:
            {
                if (hit_tester.objectTouched())
                {
                    ObjectFrame & object = *hit_tester.getObject();
                    
                    if (m_patcher_view.isSelected(object))
                    {
                        m_patcher_view.unselectObject(object);
                    }
                    else
                    {
                        m_patcher_view.selectObject(object);
                    }
                }
                else if (hit_tester.linkTouched())
                {
                    LinkView & link = *hit_tester.getLink();
                    
                    if (m_patcher_view.isSelected(link))
                    {
                        m_patcher_view.unselectLink(link);
                    }
                    else
                    {
                        m_patcher_view.selectLink(link);
                    }
                }
                break;
            }
            case Action::Selection:
            {
                if (hit_tester.objectTouched())
                {
                    m_patcher_view.selectObjectOnly(*hit_tester.getObject());
                }
                else if (hit_tester.linkTouched())
                {
                    m_patcher_view.selectLinkOnly(*hit_tester.getLink());
                }
                break;
            }
            case Action::ResizeObjects:
            {
                m_direction = getResizeDirection(hit_tester);
                
                auto& document = m_patcher_view.m_patcher_model.entity().use<model::DocumentManager>();
                
                for (flip::Ref const& ref : m_patcher_view.getSelectedObjects())
                {
                    model::Object const& object = *document.get<model::Object>(ref);
                    
                    m_mousedown_bounds[ref] = juce::Rectangle<int>(object.getX(),
                                                                   object.getY(),
                                                                   object.getWidth(),
                                                                   object.getHeight());
                }
                
                model::DocumentManager::startCommitGesture(m_patcher_view.m_patcher_model);
                
                break;
            }
            default:
            {
                break;
            }
        }
    }
    
    void MouseHandler::continueAction(juce::MouseEvent const& e)
    {
        HitTester& hit_tester = m_patcher_view.m_hittester;
        
        switch (m_current_action)
        {
            case Action::CopyOnDrag:
            {
                if (e.getMouseDownPosition() != e.getPosition())
                {
                    m_patcher_view.copySelectionToClipboard();
                    m_patcher_view.pasteFromClipboard({0, 0});
                    
                    startAction(Action::MoveObjects, e);
                }
                break;
            }
            case Action::Object:
            {
                ObjectFrame & object = *hit_tester.getObject();
                
                object.mouseDrag(e.getEventRelativeTo(&object));
                
                break;
            }
            case Action::CreateLink:
            {
                auto end_pair = m_patcher_view.getLinkCreatorNearestEndingIolet();
                
                if(end_pair.first != nullptr)
                {
                    const bool sender = m_patcher_view.m_link_creator->isBindedToSender();
                    
                    ObjectFrame* object_view = end_pair.first;
                    
                    if(object_view != nullptr)
                    {
                        if(sender)
                        {
                            m_patcher_view.m_io_highlighter.highlightInlet(*object_view, end_pair.second);
                        }
                        else
                        {
                            m_patcher_view.m_io_highlighter.highlightOutlet(*object_view, end_pair.second);
                        }
                    }
                }
                else
                {
                    m_patcher_view.m_io_highlighter.hide();
                }
                
                if (hit_tester.getZone() == HitTester::Zone::Outlet
                    || hit_tester.getZone() == HitTester::Zone::Inlet)
                {
                    m_patcher_view.m_link_creator->setEndPosition(e.getPosition());
                }
                
                break;
            }
            case Action::Lasso:
            {
                if(m_patcher_view.m_lasso.isPerforming())
                {
                    m_patcher_view.m_lasso.perform(e.getPosition(), true, e.mods.isAltDown(), e.mods.isShiftDown());
                }
                
                break;
            }
            case Action::MoveObjects:
            {
                if (m_patcher_view.isAnyObjectSelected())
                {
                    m_patcher_view.moveSelectedObjects(e.getPosition() - m_last_drag, true, true);

                    if(!m_patcher_view.m_viewport.getRelativeViewArea().contains(e.getPosition()))
                    {
                        m_patcher_view.beginDragAutoRepeat(50);
                        const juce::MouseEvent e2(e.getEventRelativeTo(&m_patcher_view.m_viewport));
                        m_patcher_view.m_viewport.autoScroll(e2.x, e2.y, 5, 5);
                    }
                }
                
                break;
            }
            case Action::ObjectEdition:
            {
                if (hit_tester.objectTouched() && e.getMouseDownPosition() != e.getPosition())
                {
                    startAction(Action::MoveObjects, e);
                }
                break;
            }
            case Action::SwitchSelection:
            {
                if (hit_tester.objectTouched() && e.getMouseDownPosition() != e.getPosition())
                {
                    startAction(Action::MoveObjects, e);
                }
                break;
            }
            case Action::Selection:
            {
                if (hit_tester.objectTouched() && e.getMouseDownPosition() != e.getPosition())
                {
                    startAction(Action::MoveObjects, e);
                }
                break;
            }
            case Action::ResizeObjects:
            {
                juce::Point<int> delta = e.getPosition() - e.getMouseDownPosition();
                auto& patcher_model = m_patcher_view.m_patcher_model;
                
                for (auto bounds_it : m_mousedown_bounds)
                {
                    auto& document = patcher_model.entity().use<model::DocumentManager>();
                    const auto object_ref = bounds_it.first;
                    
                    if(auto model = document.get<model::Object>(object_ref))
                    {
                        if(auto* box = m_patcher_view.getObject(*model))
                        {
                            resizeModelObjectBounds(*model, *box,
                                                    bounds_it.second,
                                                    delta, e.mods.isShiftDown());
                        }
                    }
                }
                
                std::string commit_msg = "Resize object";
                commit_msg += (m_mousedown_bounds.size() > 1 ? "s" : "");
                model::DocumentManager::commitGesture(patcher_model, commit_msg);
                
                break;
            }
            default:
            {
                break;
            }
        }
    }
    
    void MouseHandler::endAction(juce::MouseEvent const& e)
    {
        HitTester& hit_tester = m_patcher_view.m_hittester;
        
        switch (m_current_action)
        {
            case Action::Object:
            {
                ObjectFrame & object = *hit_tester.getObject();
                object.mouseUp(e.getEventRelativeTo(&object));
                break;
            }
            case Action::CreateLink:
            {
                m_patcher_view.m_io_highlighter.hide();
                
                auto end_pair = m_patcher_view.getLinkCreatorNearestEndingIolet();
                
                if(end_pair.first != nullptr)
                {
                    LinkViewCreator& link_creator = *m_patcher_view.m_link_creator;
                    
                    const bool sender = link_creator.isBindedToSender();
                    
                    model::Object const& binded_object = link_creator.getBindedObject().getModel();
                    model::Object const& ending_object = end_pair.first->getModel();
                    
                    model::Object const& from = sender ? binded_object : ending_object;
                    model::Object const& to = sender ? ending_object : binded_object;
                    
                    const size_t outlet = sender ? link_creator.getBindedIndex() : end_pair.second;
                    const size_t inlet = sender ? end_pair.second : link_creator.getBindedIndex();
                    
                    model::Link* link = m_patcher_view.m_patcher_model.addLink(from, outlet, to, inlet);
                    
                    if(link != nullptr)
                    {
                        m_patcher_view.m_view_model.selectLink(*link);
                        model::DocumentManager::commit(m_patcher_view.m_patcher_model, "Add link");
                    }
                }
                
                m_patcher_view.removeChildComponent(m_patcher_view.m_link_creator.get());
                m_patcher_view.m_link_creator.reset();
                break;
            }
            case Action::Lasso:
            {
                if(m_patcher_view.m_lasso.isPerforming())
                {
                    m_patcher_view.m_lasso.end();
                }
                break;
            }
            case Action::MoveObjects:
            {
                model::DocumentManager::endCommitGesture(m_patcher_view.m_patcher_model);
                
                m_patcher_view.m_viewport.updatePatcherArea(true);
                m_patcher_view.m_viewport.jumpViewToObject(*hit_tester.getObject());
                
                KiwiApp::commandStatusChanged();
                break;
            }
            case Action::ObjectEdition:
            {
                ObjectFrame & object = *hit_tester.getObject();
                m_patcher_view.editObject(object);
                break;
            }
            case Action::ResizeObjects:
            {
                model::DocumentManager::endCommitGesture(m_patcher_view.m_patcher_model);
                
                m_direction = Direction::None;
                m_mousedown_bounds.clear();
                
                m_patcher_view.m_viewport.updatePatcherArea(true);
                
                KiwiApp::commandStatusChanged();
                break;
            }
            case Action::SwitchLock:
            {
                m_patcher_view.setLock(!m_patcher_view.isLocked());
                break;
            }
            default:
            {
                break;
            }
        }
        
        m_current_action = Action::None;
    }
    
    void MouseHandler::handleMouseDown(juce::MouseEvent const& e)
    {
        HitTester& hit_tester = m_patcher_view.m_hittester;
        
        hit_tester.test(e.getPosition());
        
        if(!m_patcher_view.isLocked())
        {
            if(hit_tester.objectTouched())
            {
                if(hit_tester.getZone() == HitTester::Zone::Inside)
                {
                    if (e.mods.isCommandDown())
                    {
                        startAction(Action::Object, e);
                    }
                    else if(e.mods.isAltDown())
                    {
                        startAction(Action::CopyOnDrag, e);
                    }
                    else
                    {
                        if(e.mods.isPopupMenu())
                        {
                            startAction(Action::PopupMenu, e);
                        }
                        else
                        {
                            ObjectFrame& object = *hit_tester.getObject();
                            
                            if (e.mods.isShiftDown())
                            {
                                startAction(Action::SwitchSelection, e);
                            }
                            else if(m_patcher_view.isSelected(object))
                            {
                                startAction(Action::ObjectEdition, e);
                            }
                            else
                            {
                                startAction(Action::Selection, e);
                            }
                        }
                    }
                }
                else if(hit_tester.getZone() == HitTester::Zone::Outlet
                        || hit_tester.getZone() == HitTester::Zone::Inlet)
                {
                    startAction(Action::CreateLink, e);
                }
                else if (hit_tester.getZone() == HitTester::Zone::Border)
                {
                    startAction(Action::ResizeObjects, e);
                }
            }
            else if(hit_tester.linkTouched())
            {
                if(hit_tester.getZone() == HitTester::Zone::Inside)
                {
                    if(e.mods.isPopupMenu())
                    {
                        startAction(Action::PopupMenu, e);
                    }
                    else
                    {
                        if (e.mods.isShiftDown())
                        {
                            startAction(Action::SwitchSelection, e);
                        }
                        else
                        {
                            startAction(Action::Selection, e);
                        }
                    }
                }
            }
            else if(hit_tester.patcherTouched())
            {
                if(e.mods.isPopupMenu())
                {
                    startAction(Action::PopupMenu, e);
                }
                else if (e.mods.isCommandDown())
                {
                    startAction(Action::SwitchLock, e);
                }
                else
                {
                    startAction(Action::Lasso, e);
                }
            }
        }
        else if (e.mods.isCommandDown() && hit_tester.patcherTouched())
        {
            startAction(Action::SwitchLock, e);
        }
        else if(e.mods.isRightButtonDown())
        {
            startAction(Action::PopupMenu, e);
        }
        
        m_last_drag = e.getPosition();
    }
    
    void MouseHandler::handleMouseDrag(juce::MouseEvent const& e)
    {
        continueAction(e);
        
        m_last_drag = e.getPosition();
    }
    
    void MouseHandler::handleMouseUp(juce::MouseEvent const& e)
    {
        endAction(e);
    }
    
    void MouseHandler::handleMouseDoubleClick(juce::MouseEvent const& e)
    {
        if(!m_patcher_view.isLocked())
        {
            HitTester& hit_tester = m_patcher_view.m_hittester;
            
            hit_tester.test(e.getPosition());
            
            if(e.mods.isCommandDown() && hit_tester.objectTouched())
            {
                ObjectFrame & object= *hit_tester.getObject();
                
                object.mouseDoubleClick(e.getEventRelativeTo(&object));
            }
            else if(hit_tester.patcherTouched())
            {
                m_patcher_view.createObjectModel("", true);
            }
        }
    }
    
    int MouseHandler::getResizeDirection(HitTester const& hit_tester) const
    {
        int border = hit_tester.getBorder();
        
        int direction = Direction::None;
        
        int resize_flags = HitTester::Border::None;
        
        if(auto* box = hit_tester.getObject())
        {
            resize_flags = box->getResizingFlags();
        }
        
        const bool grow_x = ((resize_flags & HitTester::Border::Left)
                             || resize_flags & HitTester::Border::Right);
        
        const bool grow_y = ((resize_flags & HitTester::Border::Top)
                             || resize_flags & HitTester::Border::Bottom);
        
        if(grow_x)
        {
            if (border & HitTester::Border::Right)
            {
                direction |= Direction::Right;
            }
            
            if (border & HitTester::Border::Left)
            {
                direction |= Direction::Left;
            }
        }
        
        if(grow_y)
        {
            if (border & HitTester::Border::Top)
            {
                direction |= Direction::Up;
            }
            
            if ((border & HitTester::Border::Bottom))
            {
                direction |= Direction::Down;
            }
        }
        
        return direction;
    }
    
    juce::MouseCursor::StandardCursorType MouseHandler::getMouseCursorForBorder(HitTester const& hit_tester) const
    {
        juce::MouseCursor::StandardCursorType mc = juce::MouseCursor::NormalCursor;
        
        int direction = getResizeDirection(hit_tester);
        
        int resize_flags = HitTester::Border::None;
        
        if(auto* box = hit_tester.getObject())
        {
            resize_flags = box->getResizingFlags();
        }
        
        const bool grow_y = ((resize_flags & HitTester::Border::Top)
                             || resize_flags & HitTester::Border::Bottom);
        
        if(!grow_y &&
           (direction == Direction::Left || direction == Direction::Right))
        {
            return juce::MouseCursor::LeftRightResizeCursor;
        }
        
        switch(direction)
        {
            case (Direction::Up) :
            {
                mc = juce::MouseCursor::TopEdgeResizeCursor;
                break;
            }
            case (Direction::Left):
            {
                mc = juce::MouseCursor::LeftEdgeResizeCursor;
                break;
            }
            case (Direction::Right):
            {
                mc = juce::MouseCursor::RightEdgeResizeCursor;
                break;
            }
            case (Direction::Down):
            {
                mc = juce::MouseCursor::BottomEdgeResizeCursor;
                break;
            }
            case (Direction::Up | Direction::Left):
            {
                mc = juce::MouseCursor::TopLeftCornerResizeCursor;
                break;
            }
            case (Direction::Up | Direction::Right):
            {
                mc = juce::MouseCursor::TopRightCornerResizeCursor;
                break;
            }
            case (Direction::Down | Direction::Left):
            {
                mc = juce::MouseCursor::BottomLeftCornerResizeCursor;
                break;
            }
            case (Direction::Down | Direction::Right):
            {
                mc = juce::MouseCursor::BottomRightCornerResizeCursor;
                break;
            }
            default: break;
        }
        
        return mc;
    }
    
    void MouseHandler::handleMouseMove(juce::MouseEvent const& e)
    {
        juce::MouseCursor::StandardCursorType mc = juce::MouseCursor::NormalCursor;
        
        HitTester& hit_tester = m_patcher_view.m_hittester;
        
        hit_tester.test(e.getPosition());
        
        if (hit_tester.getZone() != HitTester::Zone::Outlet
            && hit_tester.getZone() != HitTester::Zone::Inlet)
        {
            m_patcher_view.m_io_highlighter.hide();
        }
        
        if(!m_patcher_view.isLocked())
        {
            if(hit_tester.objectTouched())
            {
                if(hit_tester.getZone() == HitTester::Zone::Border)
                {
                    mc = getMouseCursorForBorder(hit_tester);
                }
                else if(hit_tester.getZone() == HitTester::Zone::Outlet
                        || hit_tester.getZone() == HitTester::Zone::Inlet)
                {
                    if(hit_tester.getZone() == HitTester::Zone::Inlet)
                    {
                        m_patcher_view.m_io_highlighter.highlightInlet(*hit_tester.getObject(), hit_tester.getIndex());
                    }
                    else
                    {
                        m_patcher_view.m_io_highlighter.highlightOutlet(*hit_tester.getObject(), hit_tester.getIndex());
                    }
                    
                    mc = juce::MouseCursor::PointingHandCursor;
                }
            }
        }
        
        m_patcher_view.setMouseCursor(mc);
    }
    
    void MouseHandler::resizeModelObjectBounds(model::Object& model,
                                               ObjectFrame& box,
                                               juce::Rectangle<int> prev_bounds,
                                               juce::Point<int> delta, bool fixed_ratio)
    {
        juce::Rectangle<int> new_bounds = prev_bounds;
        
        double ratio = 0.;
        const bool stretching_top = m_direction & Direction::Up;
        const bool stretching_left = m_direction & Direction::Left;
        const bool stretching_bottom = m_direction & Direction::Down;
        const bool stretching_right = m_direction & Direction::Right;
        
        if (fixed_ratio && new_bounds.getWidth() != 0)
        {
            ratio = (static_cast<double>(new_bounds.getWidth())
                     / static_cast<double>(new_bounds.getHeight()));
        }
        
        if (stretching_left)
        {
            new_bounds.setLeft(std::min(new_bounds.getX() + delta.getX(),
                                        new_bounds.getRight()));
        }
        
        if (stretching_top)
        {
            new_bounds.setTop(std::min(new_bounds.getY() + delta.getY(),
                                       new_bounds.getBottom()));
        }
        
        if (stretching_right)
        {
            new_bounds.setRight(std::max(new_bounds.getRight() + delta.getX(),
                                         new_bounds.getX()));
        }
        
        if (stretching_bottom)
        {
            new_bounds.setBottom(std::max(new_bounds.getBottom() + delta.getY(),
                                          new_bounds.getY()));
        }
        
        const juce::Rectangle<int> limits {};
        
        juce::Rectangle<int> target_bounds = new_bounds;
        
        auto* box_constrainer = box.getBoundsConstrainer();
        
        juce::ComponentBoundsConstrainer constrainer;
        constrainer.setFixedAspectRatio(box_constrainer->getFixedAspectRatio() == 0 ? ratio : box_constrainer->getFixedAspectRatio());
        
        auto box_ratio = box_constrainer->getFixedAspectRatio();
        
        // impose ratio if not set
        box_constrainer->setFixedAspectRatio(box_ratio == 0 ? ratio : box_ratio);
        box_constrainer->checkBounds(target_bounds, prev_bounds, limits,
                                     stretching_top, stretching_left,
                                     stretching_bottom, stretching_right);
        // restore ratio
        box_constrainer->setFixedAspectRatio(box_ratio);
        
        model.setPosition(target_bounds.getX(), target_bounds.getY());
        model.setWidth(target_bounds.getWidth());
        model.setHeight(target_bounds.getHeight());
    }
}
