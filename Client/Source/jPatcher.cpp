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
#include <KiwiCore/KiwiFile.hpp>

#include "jPatcher.hpp"
#include "jObject.hpp"
#include "jLink.hpp"
#include "jFileBrowser.hpp"
#include "Application.hpp"
#include "CommandIDs.hpp"

namespace kiwi
{
    jPatcher::jPatcher(model::Patcher& patcher, model::Patcher::View& view) :
    m_patcher_model(patcher),
    m_view_model(view)
    {
        setSize(600, 400);
        loadPatcher();
        
        KiwiApp::bindToCommandManager(this);
        KiwiApp::bindToKeyMapping(this);
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
        ;
    }

    void jPatcher::rightClick(juce::MouseEvent const& event)
    {
        juce::PopupMenu m;
        m.addItem(1, "Add Plus (1)");
        m.addItem(2, "Add Print");
        m.addSeparator();

        auto r = m.show();
        
        switch(r)
        {
            case 1:
            {
                auto& obj = m_patcher_model.addObject("plus");
                obj.setPosition(event.x, event.y);
                DocumentManager::commit(m_patcher_model, "Add Plus Object");
                break;
            }
                
            case 2:
            {
                auto& obj = m_patcher_model.addObject("print");
                obj.setPosition(event.getMouseDownX(), event.getMouseDownY());
                DocumentManager::commit(m_patcher_model, "Add Print Object");
                break;
            }
                
            default: break;
        }
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
    
    void jPatcher::openSaveDialog(File & save_file) const
    {
        jFileBrowser file_browser(jFileBrowser::Mode::save);
        juce::OptionalScopedPointer<Component> file_browser_cmp(&file_browser, false);
        
        juce::DialogWindow::LaunchOptions option;
        option.dialogTitle = juce::String("Save File");
        option.content = file_browser_cmp;
        option.resizable = true;
        
        option.runModal();
        
        save_file = file_browser.getSelectedFile();
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
                result.setInfo(TRANS("Undo"), TRANS("Undo last action"), CommandCategories::general, 0);
                result.addDefaultKeypress('z',  ModifierKeys::commandModifier);
                break;
                
            case StandardApplicationCommandIDs::redo:
                result.setInfo(TRANS("Redo"), TRANS("Redo action"), CommandCategories::general, 0);
                result.addDefaultKeypress('z',  ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
                break;
                
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
                result.setInfo(TRANS("Paste replace"), TRANS("Replace selected objects with the object on the clipboard"), CommandCategories::editing, 0);
                result.addDefaultKeypress('v', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
                //result.setActive(isAnyBoxSelected() && SystemClipboard::getTextFromClipboard().isNotEmpty());
                break;
                
            case CommandIDs::duplicate:
                result.setInfo(TRANS("Duplicate"), TRANS("Duplicate the selection"), CommandCategories::editing, 0);
                result.addDefaultKeypress('d', ModifierKeys::commandModifier);
                //result.setActive(isAnyBoxSelected());
                break;
                
            case StandardApplicationCommandIDs::del:
                result.setInfo(TRANS("Delete"), TRANS("Delete all selected boxes and links"), CommandCategories::editing, 0);
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
        Console::post("perform command");
        switch (info.commandID)
        {
            case CommandIDs::save:
            {
                File const& current_save_file = DocumentManager::getSelectedFile(m_patcher_model);
                
                if (current_save_file.exist())
                {
                    DocumentManager::save(m_patcher_model, current_save_file);
                }
                else
                {
                    File save_file;
                    openSaveDialog(save_file);
                    DocumentManager::save(m_patcher_model, save_file);
                }
                break;
            }
            case StandardApplicationCommandIDs::undo:
            {
                Console::post("|- Undo");
                auto& doc = m_patcher_model.entity().use<DocumentManager>();
                doc.undo();
                doc.commit(m_patcher_model);
                break;
            }
            case StandardApplicationCommandIDs::redo:
            {
                Console::post("|- Redo");
                auto& doc = m_patcher_model.entity().use<DocumentManager>();
                doc.redo();
                doc.commit(m_patcher_model);
                break;
            }
            case StandardApplicationCommandIDs::cut:
            {
                Console::post("|- cut box");
                //copySelectionToClipboard();
                //deleteSelection();
                break;
            }
            case StandardApplicationCommandIDs::copy:
            {
                Console::post("|- copy box");
                //copySelectionToClipboard();
                break;
            }
            case StandardApplicationCommandIDs::paste:
            {
                Console::post("|- paste boxes");
                //const long gridsize = getPage()->getGridSize();
                //pasteFromClipboard(Gui::Point(gridsize, gridsize));
                break;
            }
            case CommandIDs::pasteReplace:
            {
                Console::post("|- paste replace boxes");
                //replaceBoxesFromClipboard();
                break;
            }
            case CommandIDs::duplicate:
            {
                Console::post("|- duplicate boxes");
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
            case StandardApplicationCommandIDs::selectAll:
            {
                //selectAllBoxes();
                break;
            }
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
