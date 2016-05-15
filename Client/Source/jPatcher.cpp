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
#include <KiwiModel/KiwiConsole.hpp>

#include "jInstance.hpp"
#include "jPatcher.hpp"
#include "jPatcherHelper.hpp"
#include "jObject.hpp"
#include "jLink.hpp"
#include "Application.hpp"
#include "CommandIDs.hpp"

namespace kiwi
{
    bool jPatcher::m_command_manager_binded = false;
    
    jPatcher::jPatcher(jInstance& instance, model::Patcher& patcher, model::Patcher::View& view) :
    m_instance(instance),
    m_patcher_model(patcher),
    m_view_model(view),
    m_hittester(new HitTester(*this)),
    m_object_border_down_status(HitTester::Border::None)
    {
        if(!m_command_manager_binded)
        {
            KiwiApp::bindToCommandManager(this);
            m_command_manager_binded = true;
        }
        
        KiwiApp::bindToKeyMapping(this);
        
        setSize(600, 400);
        loadPatcher();
        
        setWantsKeyboardFocus(true);
    }
    
    jPatcher::~jPatcher()
    {
        m_links.clear();
        m_objects.clear();
    }
    
    // ================================================================================ //
    //                                       PAINT                                      //
    // ================================================================================ //
    
    void jPatcher::paint(juce::Graphics & g)
    {
        const bool locked = m_is_locked;
        const juce::Colour bgcolor = juce::Colours::lightgrey;
        const int grid_size = 20;
        const juce::Rectangle<int> bounds(g.getClipBounds());
        
        g.setColour(bgcolor);
        g.fillAll();
        
        if(!locked)
        {
            g.setColour(bgcolor.contrasting(0.5));
            for(int x = bounds.getX() - (bounds.getX() % grid_size); x < bounds.getRight(); x += grid_size)
            {
                for(int y = bounds.getY() - (bounds.getY() % grid_size) ; y < bounds.getBottom(); y += grid_size)
                {
                    g.setPixel(x, y);
                }
            }
        }
    }
    
    // ================================================================================ //
    //                                   MOUSE DOWN                                     //
    // ================================================================================ //
    
    void jPatcher::mouseDown(juce::MouseEvent const& e)
    {
        m_mouse_has_just_been_clicked   = true;
        
        m_object_received_down_event    = false;
        m_copy_on_drag                  = false;
        m_is_dragging                   = false;
        m_is_dragging_links             = false;
        
        if(!isLocked())
        {
            HitTester& hit = *m_hittester.get();
            hit.test(e.getPosition());

            if(hit.targetObject())
            {
                jObject* jbox = hit.getObject();
                if(jbox)
                {
                    if(hit.getZone() == HitTester::Zone::Inside)
                    {
                        if(e.mods.isAltDown())
                        {
                            m_copy_on_drag = true;
                            m_select_on_mouse_down_status = selectOnMouseDown(*jbox, true);
                        }
                        else if (e.mods.isCommandDown())
                        {
                            jbox->mouseDown(e);
                            m_object_received_down_event = true;
                        }
                        else
                        {
                            if(e.mods.isPopupMenu())
                            {
                                if (!jbox->isSelected())
                                {
                                    m_select_on_mouse_down_status = selectOnMouseDown(*jbox, true);
                                }
                                
                                //showObjectPopupMenu(*box);
                            }
                            else
                            {
                                m_select_on_mouse_down_status = selectOnMouseDown(*jbox, !e.mods.isShiftDown());
                            }
                        }
                    }
                }
            }
            else if(hit.targetPatcher())
            {
                if(e.mods.isRightButtonDown())
                {
                    showPatcherPopupMenu(e.getPosition());
                }
                else if(!e.mods.isShiftDown())
                {
                    unselectAll();
                }
                else
                {
                    //m_lasso->begin(e, e.mods.isShiftDown());
                }
            }
        }
        
        m_last_drag = e.getPosition();
    }
    
    // ================================================================================ //
    //                                     MOUSE DOWN                                   //
    // ================================================================================ //
    
    void jPatcher::mouseDrag(MouseEvent const& e)
    {
        MouseCursor::StandardCursorType mc = MouseCursor::NormalCursor;
        
        //m_is_dragging = ! m_mouse_has_just_been_clicked;
        
        if(!isLocked())
        {
            HitTester& hit = *m_hittester.get();
            
            if(hit.targetObject())
            {
                jObject* object_j = hit.getObject();
                if(object_j)
                {
                    if(m_object_received_down_event && hit.getZone() == HitTester::Zone::Inside)
                    {
                        if(m_object_received_down_event)
                        {
                            object_j->mouseDrag(e);
                        }
                    }
                    else if(m_object_border_down_status != HitTester::Border::None)
                    {
                        if(m_mouse_has_just_been_clicked)
                        {
                            //startMoveOrResizeObjects();
                        }

                        //resizeSelectedBoxes(e.getOffsetFromDragStart(),
                          //                  m_object_border_down_status, e.mods.isShiftDown());
                        
                        m_last_drag = e.getPosition();
                        
                        mc = getMouseCursorForBorder(m_object_border_down_status);
                    }
                    else if(isAnyObjectSelected())
                    {
                        if(m_mouse_has_just_been_clicked)
                        {
                            startMoveOrResizeObjects();
                        }
                        
                        const juce::Point<int> pos = e.getPosition();
                        auto delta = pos - m_last_drag;
                        moveSelectedObjects(delta, true, true);
                        m_last_drag = pos;
                    }
                }
            }
        }
        
        setMouseCursor(mc);
        m_mouse_has_just_been_clicked = false;
        m_is_dragging = true;
    }
    
    // ================================================================================ //
    //                                      MOUSE UP                                    //
    // ================================================================================ //
    
    void jPatcher::mouseUp(MouseEvent const& e)
    {
        m_object_border_down_status = HitTester::Border::None;
        
        if(!isLocked())
        {
            HitTester& hit = *m_hittester.get();
            
            if(hit.targetObject() && hit.getZone() == HitTester::Zone::Inside && e.mods.isCommandDown())
            {
                jObject* object_j = hit.getObject();
                if(object_j)
                {
                    object_j->mouseUp(e);
                    return;
                }
            }
            
            hit.test(e.getPosition());
            
            if(hit.targetObject())
            {
                jObject* object_j = hit.getObject();
                if(object_j)
                {
                    selectOnMouseUp(*object_j, !e.mods.isShiftDown(), m_is_dragging, m_select_on_mouse_down_status);
                }
            }
            else if(e.mods.isCommandDown())
            {
                setLock(true);
            }
            
        }
        else if(e.mods.isCommandDown())
        {
            setLock(false);
        }
        
        if(m_is_in_move_or_resize_gesture)
        {
            endMoveOrResizeObjects();
        }
        
        m_mouse_has_just_been_clicked = false;
    }
    
    // ================================================================================ //
    //                                     MOUSE MOVE                                   //
    // ================================================================================ //
    
    void jPatcher::mouseMove(juce::MouseEvent const& event)
    {
        MouseCursor::StandardCursorType mc = MouseCursor::NormalCursor;
        
        if(!m_is_locked)
        {
            HitTester hit(*this);
            hit.test(event.getPosition());
            
            if(hit.targetObject())
            {
                if(hit.getZone() == HitTester::Zone::Border)
                {
                    mc = getMouseCursorForBorder(hit.getBorder());
                }
            }
        }
        
        setMouseCursor(mc);
    }

    juce::MouseCursor::StandardCursorType jPatcher::getMouseCursorForBorder(int border_flag) const
    {
        MouseCursor::StandardCursorType mc = MouseCursor::NormalCursor;
        
        switch(border_flag)
        {
            case (HitTester::Border::Top) :
            { mc = MouseCursor::TopEdgeResizeCursor; break; }
                
            case (HitTester::Border::Left):
            { mc = MouseCursor::LeftEdgeResizeCursor; break; }
                
            case (HitTester::Border::Right):
            { mc = MouseCursor::RightEdgeResizeCursor; break; }
                
            case (HitTester::Border::Bottom):
            { mc = MouseCursor::BottomEdgeResizeCursor; break; }
                
            case (HitTester::Border::Top | HitTester::Border::Left):
            { mc = MouseCursor::TopLeftCornerResizeCursor; break; }
                
            case (HitTester::Border::Top | HitTester::Border::Right):
            { mc = MouseCursor::TopRightCornerResizeCursor; break;}
                
            case (HitTester::Border::Bottom | HitTester::Border::Left):
            { mc = MouseCursor::BottomLeftCornerResizeCursor; break; }
                
            case (HitTester::Border::Bottom | HitTester::Border::Right):
            { mc = MouseCursor::BottomRightCornerResizeCursor; break; }
                
            default: break;
        }
        
        return mc;
    }
    
    void jPatcher::showPatcherPopupMenu(juce::Point<int> const& position)
    {
        juce::PopupMenu m;
        m.addItem(1, "Add Plus");
        m.addItem(2, "Add Print");
        m.addSeparator();

        auto r = m.show();
        
        switch(r)
        {
            case 1: { createObjectModel("plus", position.getX(), position.getY()); break; }
            case 2: { createObjectModel("print", position.getX(), position.getY()); break; }
            default: break;
        }
    }
    
    void jPatcher::startMoveOrResizeObjects()
    {
        m_is_in_move_or_resize_gesture = true;
        DocumentManager::startCommitGesture(m_patcher_model);
    }

    void jPatcher::endMoveOrResizeObjects()
    {
        DocumentManager::endCommitGesture(m_patcher_model);
        m_is_in_move_or_resize_gesture = false;
    }
    
    void jPatcher::moveSelectedObjects(juce::Point<int> const& delta, bool commit, bool commit_gesture)
    {
        if(isAnyObjectSelected())
        {
            for(auto& object : m_view_model.getSelectedObjects())
            {
                object->setPosition(object->getX() + delta.x, object->getY() + delta.y);
            }
            
            if(commit)
            {
                if(commit_gesture)
                {
                    DocumentManager::commitGesture(m_patcher_model, "Move selected objects");
                }
                else
                {
                    DocumentManager::commit(m_patcher_model, "Move selected objects");
                }
            }
        }
    }
    
    void jPatcher::addToSelectionBasedOnModifiers(jObject& object, bool select_only)
    {
        if(select_only)
        {
            selectObjectOnly(object);
        }
        else if(m_view_model.isSelected(object.getModel()))
        {
            unselectObject(object);
        }
        else
        {
            selectObject(object);
        }
    }
    
    void jPatcher::addToSelectionBasedOnModifiers(jLink& link, bool select_only)
    {
        if(select_only)
        {
            selectLinkOnly(link);
        }
        else if(m_view_model.isSelected(link.getModel()))
        {
            unselectLink(link);
        }
        else
        {
            selectLink(link);
        }
    }
    
    bool jPatcher::selectOnMouseDown(jObject& object, bool select_only)
    {
        if(m_view_model.isSelected(object.getModel())) return true;
        
        addToSelectionBasedOnModifiers(object, select_only);
        return false;
    }
    
    bool jPatcher::selectOnMouseDown(jLink& link, bool select_only)
    {
        if(m_view_model.isSelected(link.getModel()))
        {
            return true;
        }
        
        addToSelectionBasedOnModifiers(link, select_only);
        return false;
    }
    
    void jPatcher::selectOnMouseUp(jObject& object, bool select_only,
                                   const bool box_was_dragged, const bool result_of_mouse_down_select_method)
    {
        if(result_of_mouse_down_select_method && ! box_was_dragged)
        {
            addToSelectionBasedOnModifiers(object, select_only);
        }
    }
    
    
    void jPatcher::selectOnMouseUp(jLink& link, bool select_only,
                                   const bool box_was_dragged, const bool result_of_mouse_down_select_method)
    {
        if(result_of_mouse_down_select_method && ! box_was_dragged)
        {
            addToSelectionBasedOnModifiers(link, select_only);
        }
    }
    
    void jPatcher::bringsLinksToFront()
    {
        for(auto& link_uptr : m_links)
        {
            link_uptr->toFront(false);
        }
    }
    
    void jPatcher::bringsObjectsToFront()
    {
        for(auto& object_uptr : m_objects)
        {
            object_uptr->toFront(false);
        }
    }
    
    bool jPatcher::keyPressed(const KeyPress& key)
    {
        if(key.isKeyCode(KeyPress::deleteKey) || key.isKeyCode(KeyPress::backspaceKey))
        {
            deleteSelection();
            return true;
        }
        else if(key.isKeyCode(KeyPress::returnKey))
        {
            //if a box is selected (only one) and this box is editable => give it textediting focus
            //selected_box.showTextEditor();
        }
        else
        {
            const bool snap = key.getModifiers().isShiftDown();
            const int gridsize = 20;
            const int amount = snap ? gridsize : 1;
            
            if(key.isKeyCode(KeyPress::rightKey))
            {
                moveSelectedObjects({amount, 0});
                return true;
            }
            else if(key.isKeyCode(KeyPress::downKey))
            {
                moveSelectedObjects({0, amount});
                return true;
            }
            else if(key.isKeyCode(KeyPress::leftKey))
            {
                moveSelectedObjects({-amount, 0});
                return true;
            }
            else if(key.isKeyCode(KeyPress::upKey))
            {
                moveSelectedObjects({0, -amount});
                return true;
            }
        }
        return false;
    }
    
    void jPatcher::loadPatcher()
    {
        // create resident objects
        for(auto& object : m_patcher_model.getObjects())
        {
            if(object.resident()) { addjObject(object); }
        }
        
        // create resident links
        for(auto& link : m_patcher_model.getLinks())
        {
            if(link.resident()) { addjLink(link); }
        }
    }
    
    void jPatcher::setLock(bool locked)
    {
        if(locked)
        {
            m_view_model.unselectAll();
        }
        
        m_view_model.setLock(locked);
        DocumentManager::commit(m_patcher_model, "Edit mode switch");
    }
    
    bool jPatcher::isLocked()
    {
        return m_is_locked;
    }
    
    void jPatcher::patcherChanged(model::Patcher& patcher, model::Patcher::View& view)
    {
        if(! patcher.changed()) return; // abort
        
        if(view.added()) {}
        
        // create jObject for each newly added objects
        for(auto& object : patcher.getObjects())
        {
            if(object.added()) { addjObject(object); }
        }
        
        // create jLink for each newly added links
        for(auto& link : patcher.getLinks())
        {
            if(link.added()) { addjLink(link); }
        }
        
        // send jObject change notification
        for(auto& object : patcher.getObjects())
        {
            if(object.changed()) { objectChanged(view, object); }
        }
        
        // send jLink change notification
        for(auto& link : patcher.getLinks())
        {
            if(link.changed()) { linkChanged(link); }
            
            // send to jLink jObject change notification
            if(patcher.objectsChanged())
            {
                for(auto& object : patcher.getObjects())
                {
                    if(object.changed())
                    {
                        jLink* jlink = getLink(link);
                        
                        if(jlink) { jlink->objectChanged(object); }
                    }
                }
            }
        }
        
        checkViewInfos(view);
        checkSelectionChanges(patcher);
        
        // delete jLink for each removed links
        for(auto& link : patcher.getLinks())
        {
            if(link.removed()) { removejLink(link); }
        }
        
        // delete jObject for each removed objects
        for(auto& object : patcher.getObjects())
        {
            if(object.removed()) { removejObject(object); }
        }
        
        if(view.removed()) {}
    }
    
    void jPatcher::checkViewInfos(model::Patcher::View& view)
    {
        const bool was_locked = m_is_locked;
        m_is_locked = view.getLock();
        if(was_locked != m_is_locked)
        {
            m_is_locked ? bringsObjectsToFront() : bringsLinksToFront();
            
            for(auto& object : m_objects)
            {
                object->lockStatusChanged(m_is_locked);
            }
            
            repaint();
            KiwiApp::commandStatusChanged();
        }
    }
    
    void jPatcher::checkSelectionChanges(model::Patcher& patcher)
    {
        if(! patcher.changed()) return; // abort
        
        std::set<model::Object*>                        new_local_objects_selection;
        std::map<model::Object*, std::set<uint64_t>>    new_distant_objects_selection;
        
        for(auto& object_m : patcher.getObjects())
        {
            if(object_m.removed()) break;
            
            std::set<uint64_t> selected_for_users;
            
            for(auto& user : patcher.getUsers())
            {
                bool selected_for_local_view = false;
                bool selected_for_other_view = false;
                
                const uint64_t user_id = user.getId();
                const bool is_distant_user = user_id != m_instance.getUserId();
                
                for(auto& view : user.getViews())
                {
                    const bool is_local_view = ( &m_view_model == &view );
                    
                    const bool is_selected = view.isSelected(object_m);
                    
                    if(is_selected)
                    {
                        if(is_distant_user)
                        {
                            selected_for_other_view = true;
                            
                            // an object is considered selected for a given user
                            // when it's selected in at least one of its patcher's views.
                            break;
                        }
                        else if(is_local_view)
                        {
                            selected_for_local_view = true;
                        }
                        else
                        {
                            selected_for_other_view = true;
                        }
                    }
                }
                
                if(selected_for_local_view)
                {
                    new_local_objects_selection.emplace(&object_m);
                }
                
                if(selected_for_other_view)
                {
                    selected_for_users.emplace(user_id);
                }
            }
            
            new_distant_objects_selection.insert({&object_m, selected_for_users});
        }
        
        // check diff between old and new distant selection
        // and notify objects if their selection state changed
        for(auto& local_object : m_objects)
        {
            const auto local_object_m = &local_object->getModel();
            
            // local diff
            const bool old_local_selected_state = m_local_objects_selection.find(local_object_m) != m_local_objects_selection.end();
            
            bool new_local_selected_state = new_local_objects_selection.find(local_object_m) != new_local_objects_selection.end();
            
            if(old_local_selected_state != new_local_selected_state)
            {
                local_object->localSelectionChanged(new_local_selected_state);
                selectionChanged();
            }
            
            // distant diff
            bool distant_selection_changed_for_object = false;
            for(auto distant_it : new_distant_objects_selection)
            {
                model::Object* distant_object_lookup = distant_it.first;
                
                if(distant_object_lookup == &local_object->getModel())
                {
                    distant_selection_changed_for_object =
                    m_distant_objects_selection[distant_object_lookup] != distant_it.second;
                    
                    // notify object
                    if(distant_selection_changed_for_object)
                    {
                        local_object->distantSelectionChanged(distant_it.second);
                        selectionChanged();
                    }
                }
            }
        }
        
        // cache new selection state
        std::swap(m_distant_objects_selection, new_distant_objects_selection);
        std::swap(m_local_objects_selection, new_local_objects_selection);
    }
    
    void jPatcher::selectionChanged()
    {
        KiwiApp::commandStatusChanged();
    }
    
    void jPatcher::addjObject(model::Object& object)
    {
        const auto it = findjObject(object);
        
        if(it == m_objects.cend())
        {
            auto result = m_objects.emplace(new jObject(*this, object));
            if(result.second)
            {
                jObject& jobj = *result.first->get();
                addAndMakeVisible(jobj);
            }
        }
    }
    
    void jPatcher::objectChanged(model::Patcher::View& view, model::Object& object)
    {
        const auto it = findjObject(object);
        
        if(it != m_objects.cend())
        {
            jObject& jobject = *it->get();
            jobject.objectChanged(view, object);
        }
    }
    
    void jPatcher::removejObject(model::Object& object)
    {
        const auto it = findjObject(object);
        
        if(it != m_objects.cend())
        {
            removeChildComponent(it->get());
            m_objects.erase(it);
        }
    }
    
    void jPatcher::addjLink(model::Link& link)
    {
        const auto it = findjLink(link);
        
        if(it == m_links.cend())
        {
            auto result = m_links.emplace(new jLink(*this, link));
            
            if(result.second)
            {
                jLink& jlink = *result.first->get();
                addAndMakeVisible(jlink);
            }
        }
    }
    
    void jPatcher::linkChanged(model::Link& link)
    {
        const auto it = findjLink(link);
        
        if(it != m_links.cend())
        {
            jLink& jlink = *it->get();
            jlink.linkChanged(link);
        }
    }
    
    void jPatcher::removejLink(model::Link& link)
    {
        const auto it = findjLink(link);
        
        if(it != m_links.cend())
        {
            removeChildComponent(it->get());
            m_links.erase(it);
        }
    }
    
    std::set<std::unique_ptr<jObject>>::iterator jPatcher::findjObject(model::Object const& object) const
    {
        const auto find_jobj = [&object](std::unique_ptr<jObject> const& jobj)
        {
            return (&object == &jobj->getModel());
        };
        
        return std::find_if(m_objects.begin(), m_objects.end(), find_jobj);
    }
    
    std::set<std::unique_ptr<jLink>>::iterator jPatcher::findjLink(model::Link const& link) const
    {
        const auto find_jlink = [&link](std::unique_ptr<jLink> const& jlink)
        {
            return (&link == &jlink->getModel());
        };
        
        return std::find_if(m_links.begin(), m_links.end(), find_jlink);
    }
    
    jPatcher::jObjects const& jPatcher::getObjects() const
    {
        return m_objects;
    }
    
    //! @brief Returns the jLinks.
    jPatcher::jLinks const& jPatcher::getLinks() const
    {
        return m_links;
    }
    
    jObject* jPatcher::getObject(model::Object const& object) const
    {
        const auto it = findjObject(object);
        return (it != m_objects.cend()) ? it->get() : nullptr;
    }
    
    jLink* jPatcher::getLink(model::Link const& link) const
    {
        const auto it = findjLink(link);
        return (it != m_links.cend()) ? it->get() : nullptr;
    }
    
    // ================================================================================ //
    //                                  COMMANDS ACTIONS                                //
    // ================================================================================ //
    
    void jPatcher::createObjectModel(std::string const& text, double pos_x, double pos_y)
    {
        if(! DocumentManager::isInCommitGesture(m_patcher_model))
        {
            auto& obj = m_patcher_model.addObject(text);
            obj.setPosition(pos_x, pos_y);
            DocumentManager::commit(m_patcher_model, "Insert Object");
            KiwiApp::commandStatusChanged();
        }
    }
    
    void jPatcher::undo()
    {
        auto& doc = m_patcher_model.entity().use<DocumentManager>();
        if(doc.canUndo())
        {
            doc.undo();
            doc.commit(m_patcher_model, "undo last action");
        }
    }
    
    bool jPatcher::canUndo()
    {
        return m_patcher_model.entity().use<DocumentManager>().canUndo();
    }
    
    std::string jPatcher::getUndoLabel()
    {
        auto& doc = m_patcher_model.entity().use<DocumentManager>();
        return doc.canUndo() ? doc.getUndoLabel() : "";
    }

    void jPatcher::redo()
    {
        auto& doc = m_patcher_model.entity().use<DocumentManager>();
        if(doc.canRedo())
        {
            doc.redo();
            doc.commit(m_patcher_model, "redo last action");
        }
    }
    
    bool jPatcher::canRedo()
    {
        return m_patcher_model.entity().use<DocumentManager>().canRedo();
    }
    
    std::string jPatcher::getRedoLabel()
    {
        auto& doc = m_patcher_model.entity().use<DocumentManager>();
        return doc.canRedo() ? doc.getRedoLabel() : "";
    }
    
    // ================================================================================ //
    //                                     SELECTION                                    //
    // ================================================================================ //
    
    bool jPatcher::isAnythingSelected()
    {
        return isAnyObjectSelected() || isAnyLinksSelected();
    }
    
    bool jPatcher::isAnyObjectSelected()
    {
        return !m_local_objects_selection.empty();
    }
    
    bool jPatcher::isAnyLinksSelected()
    {
        return !m_local_links_selection.empty();
    }
    
    void jPatcher::selectObject(jObject& object)
    {
        m_view_model.selectObject(object.getModel());
        DocumentManager::commit(m_patcher_model, "select object");
    }
    
    void jPatcher::selectLink(jLink& link)
    {
        m_view_model.selectLink(link.getModel());
        DocumentManager::commit(m_patcher_model, "select link");
    }
    
    void jPatcher::unselectObject(jObject& object)
    {
        m_view_model.unselectObject(object.getModel());
        DocumentManager::commit(m_patcher_model, "unselect object");
    }
    
    void jPatcher::unselectLink(jLink& link)
    {
        m_view_model.unselectLink(link.getModel());
        DocumentManager::commit(m_patcher_model, "unselect link");
    }
    
    void jPatcher::selectObjectOnly(jObject& object)
    {
        unselectAll();
        selectObject(object);
        DocumentManager::commit(m_patcher_model, "Select object");
    }

    void jPatcher::selectLinkOnly(jLink& link)
    {
        unselectAll();
        selectLink(link);
        DocumentManager::commit(m_patcher_model, "Select link");
    }
    
    void jPatcher::selectAllObjects()
    {
        m_view_model.selectAll();
        DocumentManager::commit(m_patcher_model, "Select all objects");
    }
    
    void jPatcher::unselectAll()
    {
        if(isAnythingSelected() && !DocumentManager::isInCommitGesture(m_patcher_model))
        {
            m_view_model.unselectAll();
            DocumentManager::commit(m_patcher_model, "Unselect all");
        }
    }
    
    void jPatcher::deleteSelection()
    {
        for(model::Link* link : m_view_model.getSelectedLinks())
        {
            m_patcher_model.removeLink(*link);
        }
        
        for(model::Object* object : m_view_model.getSelectedObjects())
        {
            m_patcher_model.removeObject(*object);
        }
        
        DocumentManager::commit(m_patcher_model, "Delete objects and links");
    }
    
    // ================================================================================ //
    //                              APPLICATION COMMAND TARGET                          //
    // ================================================================================ //
    
    ApplicationCommandTarget* jPatcher::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void jPatcher::getAllCommands(Array<CommandID>& commands)
    {
        commands.add(CommandIDs::save);
        commands.add(StandardApplicationCommandIDs::undo);
        commands.add(StandardApplicationCommandIDs::redo);
        commands.add(StandardApplicationCommandIDs::cut);
        commands.add(StandardApplicationCommandIDs::copy);
        commands.add(StandardApplicationCommandIDs::paste);
        commands.add(CommandIDs::pasteReplace);
        commands.add(CommandIDs::duplicate);
        commands.add(StandardApplicationCommandIDs::del);
        commands.add(StandardApplicationCommandIDs::selectAll);
        
        commands.add(CommandIDs::toFront);
        commands.add(CommandIDs::toBack);
        
        commands.add(CommandIDs::zoomIn);
        commands.add(CommandIDs::zoomOut);
        commands.add(CommandIDs::zoomNormal);
        commands.add(CommandIDs::editModeSwitch);
        commands.add(CommandIDs::gridModeSwitch);
        commands.add(CommandIDs::enableSnapToGrid);
        
        commands.add(CommandIDs::showPatcherInspector);
        commands.add(CommandIDs::showObjectInspector);
        
        //CommandIDs::openObjectHelp
    }
    
    void jPatcher::getCommandInfo(const CommandID commandID, ApplicationCommandInfo& result)
    {
        switch(commandID)
        {
            case CommandIDs::save:
                result.setInfo(TRANS("Save"), TRANS("Save document"), CommandCategories::general, 0);
                result.addDefaultKeypress('s',  ModifierKeys::commandModifier);
                break;
                
            case StandardApplicationCommandIDs::undo:
            {
                juce::String label = TRANS("Undo");
                const bool hasUndo = canUndo();
                if(hasUndo) { label += ' ' + getUndoLabel(); }
                
                result.setInfo(label, TRANS("Undo last action"), CommandCategories::general, 0);
                result.addDefaultKeypress('z',  ModifierKeys::commandModifier);
                result.setActive(!m_is_locked && hasUndo);
                break;
            }
            case StandardApplicationCommandIDs::redo:
            {
                juce::String label = TRANS("Redo");
                const bool hasRedo = canRedo();
                if(hasRedo) { label += ' ' + getRedoLabel(); }
                
                result.setInfo(label, TRANS("Redo action"), CommandCategories::general, 0);
                result.addDefaultKeypress('z',  ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
                result.setActive(!m_is_locked && hasRedo);
                break;
            }
            case StandardApplicationCommandIDs::cut:
                result.setInfo(TRANS("Cut"), TRANS("Cut"), CommandCategories::editing, 0);
                result.addDefaultKeypress('x', ModifierKeys::commandModifier);
                result.setActive(isAnyObjectSelected());
                break;
                
            case StandardApplicationCommandIDs::copy:
                result.setInfo(TRANS("Copy"), TRANS("Copy"), CommandCategories::editing, 0);
                result.addDefaultKeypress('c', ModifierKeys::commandModifier);
                result.setActive(isAnyObjectSelected());
                break;
                
            case StandardApplicationCommandIDs::paste:
                result.setInfo(TRANS("Paste"), TRANS("Paste"), CommandCategories::editing, 0);
                result.addDefaultKeypress('v', ModifierKeys::commandModifier);
                //result.setActive(!m_is_locked && SystemClipboard::getTextFromClipboard().isNotEmpty());
                break;
                
            case CommandIDs::pasteReplace:
                result.setInfo(TRANS("Paste replace"),
                               TRANS("Replace selected objects with the object on the clipboard"),
                               CommandCategories::editing, 0);
                
                result.addDefaultKeypress('v', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
                //result.setActive(isAnyObjectSelected() && SystemClipboard::getTextFromClipboard().isNotEmpty());
                break;
                
            case CommandIDs::duplicate:
                result.setInfo(TRANS("Duplicate"), TRANS("Duplicate the selection"),
                               CommandCategories::editing, 0);
                
                result.addDefaultKeypress('d', ModifierKeys::commandModifier);
                result.setActive(isAnyObjectSelected());
                break;
                
            case StandardApplicationCommandIDs::del:
                result.setInfo(TRANS("Delete"), TRANS("Delete all selected objects and links"),
                               CommandCategories::editing, 0);
                
                result.addDefaultKeypress(KeyPress::backspaceKey, ModifierKeys::noModifiers);
                result.setActive(isAnythingSelected());
                break;
                
            case StandardApplicationCommandIDs::selectAll:
                result.setInfo(TRANS("Select All"), TRANS("Select all boxes and links"), CommandCategories::editing, 0);
                result.addDefaultKeypress('a', ModifierKeys::commandModifier);
                result.setActive(!m_is_locked);
                break;
                
            case CommandIDs::toFront:
                result.setInfo(TRANS("Bring to Front"), TRANS("Bring selected boxes to front"), CommandCategories::editing, 0);
                result.addDefaultKeypress('f', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
                result.setActive(isAnyObjectSelected());
                break;
                
            case CommandIDs::toBack:
                result.setInfo(TRANS("Send to Back"), TRANS("Send selected boxes to back"), CommandCategories::editing, 0);
                result.addDefaultKeypress('b', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
                result.setActive(isAnyObjectSelected());
                break;
                
            case CommandIDs::editModeSwitch:
            {
                result.setInfo(TRANS("Edit"),
                               TRANS("Switch between edition and play mode"),
                               CommandCategories::view, 0);
                
                result.addDefaultKeypress ('e',  ModifierKeys::commandModifier);
                result.setTicked(!m_view_model.getLock());
                break;
            }
                
            default:
                result.setInfo (TRANS("[unknown command]"), TRANS("dada"), CommandCategories::view, 0);
                break;
        }
    }
    
    bool jPatcher::perform(const InvocationInfo& info)
    {
        switch (info.commandID)
        {
            case CommandIDs::save:
            {
                Console::post("|- try to save page");
                break;
            }
            case StandardApplicationCommandIDs::undo: { undo(); break; }
            case StandardApplicationCommandIDs::redo: { redo(); break; }
            case StandardApplicationCommandIDs::cut:
            {
                Console::post("|- cut");
                //copySelectionToClipboard();
                //deleteSelection();
                break;
            }
            case StandardApplicationCommandIDs::copy:
            {
                Console::post("|- copy selected objects");
                //copySelectionToClipboard();
                break;
            }
            case StandardApplicationCommandIDs::paste:
            {
                Console::post("|- paste objects");
                //const long gridsize = getPage()->getGridSize();
                //pasteFromClipboard(juce::Point<int>(gridsize, gridsize));
                break;
            }
            case CommandIDs::pasteReplace:
            {
                Console::post("|- paste replace objects");
                //replaceBoxesFromClipboard();
                break;
            }
            case CommandIDs::duplicate:
            {
                Console::post("|- duplicate objects");
                //copySelectionToClipboard();
                //const long gridsize = getPage()->getGridSize();
                //pasteFromClipboard(juce::Point<int>(gridsize, gridsize));
                //unselectAllLinks();
                break;
            }
            case StandardApplicationCommandIDs::del:        { deleteSelection(); break; }
            case StandardApplicationCommandIDs::selectAll:  { selectAllObjects(); break; }
            case CommandIDs::toFront:                       { break; }
            case CommandIDs::toBack:                        { break; }
            case CommandIDs::editModeSwitch:                { setLock(!m_is_locked); break; }
                
            default: return false;
        }
        
        return true;
    }
}
