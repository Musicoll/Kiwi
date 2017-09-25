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

#include <KiwiApp.h>
#include <KiwiApp_Patcher/KiwiApp_PatcherView.h>
#include <KiwiApp_Patcher/KiwiApp_LinkView.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectFrame.h>
#include <KiwiApp_Patcher/KiwiApp_PatcherViewHitTester.h>
#include <KiwiApp_Network/KiwiApp_DocumentManager.h>

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
                ObjectFrame & object = *hit_tester.getObject();
                
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
            case Action::MoveObject:
            {
                KiwiApp::commandStatusChanged();
                
                DocumentManager::startCommitGesture(m_patcher_view.m_patcher_model);
                
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
            case Action::ResizeObject:
            {
                m_direction = getResizeDirection(hit_tester);
                
                auto& document = m_patcher_view.m_patcher_model.entity().use<DocumentManager>();
                
                for (flip::Ref const& ref : m_patcher_view.getSelectedObjects())
                {
                    model::Object const& object = *document.get<model::Object>(ref);
                    
                    m_mousedown_bounds[ref] = juce::Rectangle<int>(object.getX(),
                                                                   object.getY(),
                                                                   object.getWidth(),
                                                                   object.getHeight());
                }
                
                DocumentManager::startCommitGesture(m_patcher_view.m_patcher_model);
                
                break;
            }
            default:
            {
                break;
            }
        }
    }
    
    void MouseHandler::applyNewBounds(model::Object & object_model, juce::Rectangle<int> new_bounds, double ratio) const
    {
        juce::ComponentBoundsConstrainer bounds_constrainer;
        
        if (!object_model.hasFlag(model::Object::Flag::ResizeWidth))
        {
            bounds_constrainer.setMinimumWidth(object_model.getWidth());
            bounds_constrainer.setMaximumWidth(object_model.getWidth());
        }
        else if (object_model.getMinWidth() != 0)
        {
            bounds_constrainer.setMinimumWidth(object_model.getMinWidth());
        }
        
        if (!object_model.hasFlag(model::Object::Flag::ResizeHeight))
        {
            bounds_constrainer.setMinimumHeight(object_model.getHeight());
            bounds_constrainer.setMinimumHeight(object_model.getHeight());
        }
        else if (object_model.getMinHeight() != 0)
        {
            bounds_constrainer.setMinimumHeight(object_model.getMinHeight());
        }
        
        if (object_model.getRatio() != 0)
        {
            bounds_constrainer.setFixedAspectRatio(1 / object_model.getRatio());
        }
        else if (ratio != 0)
        {
            bounds_constrainer.setFixedAspectRatio(1 / ratio);
        }
        
        juce::Rectangle<int> limits = m_patcher_view.getBounds();
        
        juce::Rectangle<int> previous_bounds(object_model.getX(),
                                             object_model.getY(),
                                             object_model.getWidth(),
                                             object_model.getHeight());
        
        juce::Rectangle<int> target_bounds = new_bounds;
        
        bounds_constrainer.checkBounds(target_bounds,
                                       previous_bounds,
                                       limits,
                                       m_direction & Direction::Up,
                                       m_direction & Direction::Left,
                                       m_direction & Direction::Down,
                                       m_direction & Direction::Right);
        
        object_model.setPosition(target_bounds.getX(), target_bounds.getY());
        
        object_model.setWidth(target_bounds.getWidth());
        
        object_model.setHeight(target_bounds.getHeight());
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
                    
                    startAction(Action::MoveObject, e);
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
            case Action::MoveObject:
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
                if (e.getMouseDownPosition() != e.getPosition())
                {
                    startAction(Action::MoveObject, e);
                }
                break;
            }
            case Action::SwitchSelection:
            {
                if (e.getMouseDownPosition() != e.getPosition())
                {
                    startAction(Action::MoveObject, e);
                }
                
                break;
            }
            case Action::Selection:
            {
                if (e.getMouseDownPosition() != e.getPosition())
                {
                    startAction(Action::MoveObject, e);
                }
                break;
            }
            case Action::ResizeObject:
            {
                juce::Point<int> delta = e.getPosition() - e.getMouseDownPosition();
                
                for (auto bounds_it : m_mousedown_bounds)
                {
                    juce::Rectangle<int> new_bounds = bounds_it.second;
                    
                    double ratio = 0.;
                    
                    if (e.mods.isShiftDown() && new_bounds.getWidth() != 0)
                    {
                        ratio = static_cast<double>(new_bounds.getHeight()) / static_cast<double>(new_bounds.getWidth());
                    }
                    
                    if (m_direction & Direction::Left)
                    {
                        new_bounds.setLeft(std::min(new_bounds.getX() + delta.getX(), new_bounds.getRight()));
                    }
                    
                    if (m_direction & Direction::Up)
                    {
                        new_bounds.setTop(std::min(new_bounds.getY() + delta.getY(), new_bounds.getBottom()));
                    }
                    
                    if (m_direction & Direction::Right)
                    {
                        new_bounds.setRight(std::max(new_bounds.getRight() + delta.getX(), new_bounds.getX()));
                    }
                    
                    if (m_direction & Direction::Down)
                    {
                        new_bounds.setBottom(std::max(new_bounds.getBottom() + delta.getY(), new_bounds.getY()));
                    }
                    
                    auto& document = m_patcher_view.m_patcher_model.entity().use<DocumentManager>();
                    
                    applyNewBounds(*document.get<model::Object>(bounds_it.first), new_bounds, ratio);
                }
                
                DocumentManager::commitGesture(m_patcher_view.m_patcher_model, "Resize object");
                
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
                        DocumentManager::commit(m_patcher_view.m_patcher_model, "Add link");
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
            case Action::MoveObject:
            {
                DocumentManager::endCommitGesture(m_patcher_view.m_patcher_model);
                
                KiwiApp::commandStatusChanged();
                
                m_patcher_view.m_viewport.updatePatcherArea(true);
                
                m_patcher_view.m_viewport.jumpViewToObject(*hit_tester.getObject());
                
                break;
            }
            case Action::ObjectEdition:
            {
                ObjectFrame & object = *hit_tester.getObject();
                m_patcher_view.editObject(object);
                break;
            }
            case Action::ResizeObject:
            {
                m_direction = Direction::None;
                m_mousedown_bounds.clear();
                
                DocumentManager::endCommitGesture(m_patcher_view.m_patcher_model);
                
                m_patcher_view.m_viewport.updatePatcherArea(true);
                
                m_patcher_view.m_viewport.jumpViewToObject(*hit_tester.getObject());
                
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
                    if(e.mods.isAltDown())
                    {
                        startAction(Action::CopyOnDrag, e);
                    }
                    else if (e.mods.isCommandDown())
                    {
                        startAction(Action::Object, e);
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
                    startAction(Action::ResizeObject, e);
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
        
        model::Object const& object_model = hit_tester.getObject()->getModel();
        
        if ((border & HitTester::Border::Top) && object_model.hasFlag(model::Object::Flag::ResizeHeight))
        {
            direction |= Direction::Up;
        }
        
        if ((border & HitTester::Border::Right) && object_model.hasFlag(model::Object::Flag::ResizeWidth))
        {
            direction |= Direction::Right;
        }
        
        if ((border & HitTester::Border::Bottom) && object_model.hasFlag(model::Object::Flag::ResizeHeight))
        {
            direction |= Direction::Down;
        }
        
        if ((border & HitTester::Border::Left) && object_model.hasFlag(model::Object::Flag::ResizeWidth))
        {
            direction |= Direction::Left;
        }
        
        return direction;
    }
    
    juce::MouseCursor::StandardCursorType MouseHandler::getMouseCursorForBorder(HitTester const& hit_tester) const
    {
        juce::MouseCursor::StandardCursorType mc = juce::MouseCursor::NormalCursor;
        
        int direction = getResizeDirection(hit_tester);
        
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
}
