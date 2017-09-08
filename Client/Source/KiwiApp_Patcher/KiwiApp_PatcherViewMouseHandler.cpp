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
    m_last_drag()
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
                    mc = m_patcher_view.getMouseCursorForBorder(hit_tester.getBorder());
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
