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
    m_view_model(view)
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
    
    void jPatcher::paint(juce::Graphics & g)
    {
        const bool locked = false;
        const juce::Colour bgcolor = juce::Colours::lightgrey;
        const int grid_size = 20;
        const juce::Rectangle<int> bounds(g.getClipBounds());
        
        g.setColour(bgcolor);
        g.fillAll();
        
        if (!locked)
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
    
    void jPatcher::mouseDown(juce::MouseEvent const& event)
    {
        if(event.mods.isLeftButtonDown())
        {
            leftClick(event);
        }
        else if(event.mods.isRightButtonDown())
        {
            rightClick(event);
        }
    }
    
    void jPatcher::leftClick(juce::MouseEvent const& event)
    {
        unselectAll();
    }

    void jPatcher::rightClick(juce::MouseEvent const& event)
    {
        juce::PopupMenu m;
        m.addItem(1, "Add Plus");
        m.addItem(2, "Add Print");
        m.addSeparator();

        auto r = m.show();
        
        switch(r)
        {
            case 1: { createObjectModel("plus", event.x, event.y); break; }
            case 2: { createObjectModel("print", event.x, event.y); break; }
            default: break;
        }
    }
    
    void jPatcher::moveSelectedObjects(juce::Point<int> const& delta)
    {
        if(true /*isAnyBoxSelected()*/)
        {
            auto& selection = m_view_model.useSelection();
            
            for(auto& object : selection.getObjects())
            {
                object->setPosition(object->getX() + delta.x, object->getY() + delta.y);
            }
            
            DocumentManager::commit(m_patcher_model, "Move selected objects");
        }
    }

    
    bool jPatcher::keyPressed(const KeyPress& key)
    {
        if(key.isKeyCode(KeyPress::deleteKey) || key.isKeyCode(KeyPress::backspaceKey))
        {
            //deleteSelection();
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
                        jLink* jlink = getjLink(link);
                        
                        if(jlink) { jlink->objectChanged(object); }
                    }
                }
            }
        }
        
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
                    
                    const bool is_selected = view.useSelection().isSelected(object_m);
                    
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
            auto result = m_objects.emplace(new jObject(object));
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
    
    jObject* jPatcher::getjObject(model::Object const& object) const
    {
        const auto it = findjObject(object);
        return (it != m_objects.cend()) ? it->get() : nullptr;
    }
    
    jLink* jPatcher::getjLink(model::Link const& link) const
    {
        const auto it = findjLink(link);
        return (it != m_links.cend()) ? it->get() : nullptr;
    }
    
    // ================================================================================ //
    //                                  COMMANDS ACTIONS                                //
    // ================================================================================ //
    
    void jPatcher::createObjectModel(std::string const& text, double pos_x, double pos_y)
    {
        auto& obj = m_patcher_model.addObject(text);
        obj.setPosition(pos_x, pos_y);
        DocumentManager::commit(m_patcher_model, "Insert Object");
        KiwiApp::commandStatusChanged();
    }
    
    void jPatcher::undo()
    {
        auto& doc = m_patcher_model.entity().use<DocumentManager>();
        if(doc.canUndo())
        {
            doc.undo();
            doc.commit(m_patcher_model);
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
            doc.commit(m_patcher_model);
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
    
    void jPatcher::selectAllObjects()
    {
        m_view_model.selectAll();
        DocumentManager::commit(m_patcher_model, "Select all objects");
    }
    
    void jPatcher::unselectAll()
    {
        m_view_model.unSelectAll();
        DocumentManager::commit(m_patcher_model, "Unselect all");
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
                result.setActive(hasUndo);
                break;
            }
            case StandardApplicationCommandIDs::redo:
            {
                juce::String label = TRANS("Redo");
                const bool hasRedo = canRedo();
                if(hasRedo) { label += ' ' + getRedoLabel(); }
                
                result.setInfo(label, TRANS("Redo action"), CommandCategories::general, 0);
                result.addDefaultKeypress('z',  ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
                result.setActive(canRedo());
                break;
            }
            case StandardApplicationCommandIDs::cut:
                result.setInfo(TRANS("Cut"), TRANS("Cut"), CommandCategories::editing, 0);
                result.addDefaultKeypress('x', ModifierKeys::commandModifier);
                //result.setActive(isAnyBoxSelected());
                break;
                
            case StandardApplicationCommandIDs::copy:
                result.setInfo(TRANS("Copy"), TRANS("Copy"), CommandCategories::editing, 0);
                result.addDefaultKeypress('c', ModifierKeys::commandModifier);
                //result.setActive(isAnyBoxSelected());
                break;
                
            case StandardApplicationCommandIDs::paste:
                result.setInfo(TRANS("Paste"), TRANS("Paste"), CommandCategories::editing, 0);
                result.addDefaultKeypress('v', ModifierKeys::commandModifier);
                //result.setActive(!getLockStatus() && SystemClipboard::getTextFromClipboard().isNotEmpty());
                break;
                
            case CommandIDs::pasteReplace:
                result.setInfo(TRANS("Paste replace"),
                               TRANS("Replace selected objects with the object on the clipboard"),
                               CommandCategories::editing, 0);
                
                result.addDefaultKeypress('v', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
                //result.setActive(isAnyBoxSelected() && SystemClipboard::getTextFromClipboard().isNotEmpty());
                break;
                
            case CommandIDs::duplicate:
                result.setInfo(TRANS("Duplicate"), TRANS("Duplicate the selection"),
                               CommandCategories::editing, 0);
                
                result.addDefaultKeypress('d', ModifierKeys::commandModifier);
                //result.setActive(isAnyBoxSelected());
                break;
                
            case StandardApplicationCommandIDs::del:
                result.setInfo(TRANS("Delete"), TRANS("Delete all selected objects and links"),
                               CommandCategories::editing, 0);
                
                result.addDefaultKeypress(KeyPress::backspaceKey, ModifierKeys::noModifiers);
                //result.setActive(isAnythingSelected());
                break;
                
            case StandardApplicationCommandIDs::selectAll:
                result.setInfo(TRANS("Select All"), TRANS("Select all boxes and links"), CommandCategories::editing, 0);
                result.addDefaultKeypress('a', ModifierKeys::commandModifier);
                //result.setActive(!getLockStatus());
                break;
                
            case CommandIDs::toFront:
                result.setInfo(TRANS("Bring to Front"), TRANS("Bring selected boxes to front"), CommandCategories::editing, 0);
                result.addDefaultKeypress('f', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
                //result.setActive(isAnyBoxSelected());
                break;
                
            case CommandIDs::toBack:
                result.setInfo(TRANS("Send to Back"), TRANS("Send selected boxes to back"), CommandCategories::editing, 0);
                result.addDefaultKeypress('b', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
                //result.setActive(isAnyBoxSelected());
                break;
                
            case CommandIDs::editModeSwitch:
                result.setInfo (TRANS("Edit"), TRANS("Switch between edit and play mode"), CommandCategories::view, 0);
                result.addDefaultKeypress ('e',  ModifierKeys::commandModifier);
                //result.setTicked(!getLockStatus());
                break;
                
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
                //pasteFromClipboard(Gui::Point(gridsize, gridsize));
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
                //pasteFromClipboard(Gui::Point(gridsize, gridsize));
                //unselectAllLinks();
                break;
            }
            case StandardApplicationCommandIDs::del:
            {
                Console::post("|- delete selection");
                //deleteSelection();
                break;
            }
            case StandardApplicationCommandIDs::selectAll: { selectAllObjects(); break; }
            case CommandIDs::toFront:
            {
                break;
            }
            case CommandIDs::toBack:
            {
                break;
            }
            case CommandIDs::editModeSwitch:
            {
                //setLockStatus(!getLockStatus());
                break;
            }
            default: return false;
        }
        
        return true;
    }
}
