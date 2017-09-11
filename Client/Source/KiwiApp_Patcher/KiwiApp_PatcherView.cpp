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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Objects.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

#include "KiwiApp_PatcherView.h"

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectView.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_Objects.h>
#include <KiwiApp_Patcher/KiwiApp_Factory.h>

#include "../KiwiApp.h"
#include "../KiwiApp_General/KiwiApp_CommandIDs.h"
#include "../KiwiApp_Network/KiwiApp_DocumentManager.h"
#include "KiwiApp_LinkView.h"
#include "KiwiApp_PatcherComponent.h"

namespace kiwi
{
    // ================================================================================ //
    //                                    PATCHER VIEW                                  //
    // ================================================================================ //
    
    PatcherView::PatcherView(PatcherManager& manager,
                       Instance& instance,
                       model::Patcher& patcher,
                       model::Patcher::View& view) :
    m_manager(manager),
    m_instance(instance),
    m_patcher_model(patcher),
    m_view_model(view),
    m_viewport(*this),
    m_hittester(*this),
    m_mouse_handler(*this),
    m_io_highlighter(),
    m_lasso(*this),
    m_grid_size(20)
    {
        KiwiApp::bindToCommandManager(this);
        KiwiApp::bindToKeyMapping(this);
        setWantsKeyboardFocus(true);
        
        addChildComponent(m_io_highlighter);
        addChildComponent(m_lasso);

        loadPatcher();
        m_viewport.updatePatcherArea(true);
        
        m_manager.addListener(*this);
    }
    
    PatcherView::~PatcherView()
    {
        m_manager.removeListener(*this);
        
        removeChildComponent(&m_io_highlighter);
        removeChildComponent(&m_lasso);
        
        m_local_objects_selection.clear();
        m_local_links_selection.clear();
        m_distant_objects_selection.clear();
        m_distant_links_selection.clear();
        
        m_links.clear();
        m_objects.clear();
    }
    
    PatcherManager& PatcherView::usePatcherManager()
    {
        return m_manager;
    }
    
    // ================================================================================ //
    //                                       PAINT                                      //
    // ================================================================================ //
    
    void PatcherView::paint(juce::Graphics & g)
    {
        const juce::Colour bgcolor = juce::Colour::fromFloatRGBA(0.8, 0.8, 0.8, 1.);

        if(!isLocked())
        {
            const int grid_size = m_grid_size;
            
            const juce::Point<int> origin = getOriginPosition();
            const juce::Rectangle<int> bounds = getLocalBounds();
            const juce::Rectangle<int> clip_bounds = g.getClipBounds();
            const juce::Rectangle<int> origin_bounds = bounds.withPosition(origin);
            
            if(!origin.isOrigin())
            {
                const juce::Colour off_bgcolor = bgcolor.darker(0.1);
                g.setColour(off_bgcolor);
                g.fillRect(bounds);
                
                // draw origin
                g.setColour(off_bgcolor.contrasting(0.5));
                
                if(origin.getY() != 0)
                {
                    g.drawLine(origin.getX(), origin.getY(), bounds.getWidth(), origin.getY());
                }
                
                if(origin.getX() != 0)
                {
                    g.drawLine(origin.getX(), origin.getY(), origin.getX(), bounds.getHeight());
                }
            }
            
            g.setColour(bgcolor);
            g.fillRect(origin_bounds);
            
            g.setColour(bgcolor.contrasting(0.3));
            
            for(int x = (origin.getX() % grid_size); x < clip_bounds.getRight(); x += grid_size)
            {
                for(int y = (origin.getY() % grid_size); y < clip_bounds.getBottom(); y += grid_size)
                {
                    g.setPixel(x, y);
                }
            }
        }
        else
        {
            g.fillAll(bgcolor);
        }
    }
    
    // ================================================================================ //
    //                                   MOUSE DOWN                                     //
    // ================================================================================ //
    
    void PatcherView::mouseDown(juce::MouseEvent const& e)
    {
        m_mouse_handler.handleMouseDown(e);
    }
    
    // ================================================================================ //
    //                                     MOUSE DRAG                                   //
    // ================================================================================ //
    
    void PatcherView::mouseDrag(juce::MouseEvent const& e)
    {
        m_mouse_handler.handleMouseDrag(e);
    }
    
    // ================================================================================ //
    //                                      MOUSE UP                                    //
    // ================================================================================ //
    
    void PatcherView::mouseUp(juce::MouseEvent const& e)
    {
        m_mouse_handler.handleMouseUp(e);
    }
    
    // ================================================================================ //
    //                                     MOUSE MOVE                                   //
    // ================================================================================ //
    
    void PatcherView::mouseMove(juce::MouseEvent const& event)
    {
        m_mouse_handler.handleMouseMove(event);
    }
    
    // ================================================================================ //
    //                                  MOUSE DOUBLE CLICK                              //
    // ================================================================================ //
    
    void PatcherView::mouseDoubleClick(const juce::MouseEvent& e)
    {
        m_mouse_handler.handleMouseDoubleClick(e);
    }
    
    void PatcherView::showPatcherPopupMenu(juce::Point<int> const& position)
    {
        juce::PopupMenu m;
        auto* cm = &KiwiApp::getCommandManager();
        
        m.addCommandItem(cm, CommandIDs::editModeSwitch);
        m.addSeparator();
        
        if(!isLocked())
        {
            m.addCommandItem(cm, juce::StandardApplicationCommandIDs::paste);
            m.addSeparator();
            
            m.addCommandItem(cm, juce::StandardApplicationCommandIDs::selectAll);
            m.addSeparator();
        }
        
        m.show();
    }
    
    void PatcherView::showObjectPopupMenu(ObjectFrame const& object, juce::Point<int> const& position)
    {
        if(!isLocked())
        {
            juce::PopupMenu m;
            auto* cm = &KiwiApp::getCommandManager();
            
            m.addCommandItem(cm, juce::StandardApplicationCommandIDs::cut);
            m.addCommandItem(cm, juce::StandardApplicationCommandIDs::copy);
            m.addCommandItem(cm, juce::StandardApplicationCommandIDs::paste);
            m.addSeparator();
            
            m.addCommandItem(cm, CommandIDs::pasteReplace);
            m.addCommandItem(cm, juce::StandardApplicationCommandIDs::del);
            m.addSeparator();

            m.addSeparator();
            
            m.show();
        }
    }
    
    void PatcherView::showLinkPopupMenu(LinkView const& link, juce::Point<int> const& position)
    {
        if(!isLocked())
        {
            juce::PopupMenu m;
            auto* cm = &KiwiApp::getCommandManager();
            
            m.addCommandItem(cm, juce::StandardApplicationCommandIDs::del);
            m.addSeparator();
            
            m.show();
        }
    }
    
    // ================================================================================ //
    //                                     SELECTION                                    //
    // ================================================================================ //
    
    void PatcherView::moveSelectedObjects(juce::Point<int> const& delta, bool commit, bool commit_gesture)
    {
        for(auto* object : m_view_model.getSelectedObjects())
        {
            if(object && !object->removed())
            {
                object->setPosition(object->getX() + delta.x, object->getY() + delta.y);
            }
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
    
    std::set<flip::Ref> const& PatcherView::getSelectedObjects() const
    {
        return m_local_objects_selection;
    }
    
    std::set<flip::Ref> const& PatcherView::getSelectedLinks() const
    {
        return m_local_links_selection;
    }
    
    void PatcherView::copySelectionToClipboard()
    {
        auto& document = m_patcher_model.entity().use<DocumentManager>();
        
        auto& clipboard = m_instance.getPatcherClipboardData();
        clipboard.clear();
        flip::StreamBinOut sbo(clipboard);
        
        std::set<flip::Ref> const& selected_objects = getSelectedObjects();
        
        sbo << static_cast<uint32_t>(selected_objects.size());
        
        for(flip::Ref const& object_ref : selected_objects)
        {
            model::Object const* object_ptr = document.get<model::Object>(object_ref);
            if(object_ptr)
            {
                flip::Mold mold(model::DataModel::use(), sbo);
                model::Object const& object = *object_ptr;
                
                model::Factory::copy(object, mold);
                mold.cure();

                // store object name to restore it later from the model::Factory.
                sbo << object.getName();
                
                // store object ref to find links boundaries.
                sbo << object_ptr->ref();
            }
        }
        
        uint32_t number_of_links = 0;
        
        for(model::Link& link : m_patcher_model.getLinks())
        {
            if(!link.removed())
            {
                flip::Ref const& sender_ref = link.getSenderObject().ref();
                flip::Ref const& receiver_ref = link.getReceiverObject().ref();
                
                bool sender_selected = m_local_objects_selection.find(sender_ref) != m_local_objects_selection.end();
                
                bool receiver_selected = m_local_objects_selection.find(receiver_ref) != m_local_objects_selection.end();
                
                if(sender_selected && receiver_selected)
                {
                    number_of_links++;
                }
            }
        }
        
        // Store the number of links
        sbo << number_of_links;
        
        for(model::Link& link : m_patcher_model.getLinks())
        {
            if(!link.removed())
            {
                flip::Ref const& sender_ref = link.getSenderObject().ref();
                flip::Ref const& receiver_ref = link.getReceiverObject().ref();
                
                bool sender_selected = m_local_objects_selection.find(sender_ref) != m_local_objects_selection.end();
                
                bool receiver_selected = m_local_objects_selection.find(receiver_ref) != m_local_objects_selection.end();
                
                if(sender_selected && receiver_selected)
                {
                    uint32_t sender_index = static_cast<uint32_t>(link.getSenderIndex());
                    uint32_t receiver_index = static_cast<uint32_t>(link.getReceiverIndex());
                    
                    sbo << sender_ref;
                    sbo << receiver_ref;
                    sbo << sender_index;
                    sbo << receiver_index;
                }
            }
        }
        
        KiwiApp::commandStatusChanged();
    }
    
    void PatcherView::pasteFromClipboard(juce::Point<int> const& delta)
    {
        auto& clipboard = m_instance.getPatcherClipboardData();
        if(!clipboard.empty())
        {
            std::vector<uint8_t> data(clipboard.begin(), clipboard.end());
            flip::StreamBinIn sbi(data);
            
            unselectAll();
            
            std::map<flip::Ref, model::Object const*> molded_objects;
            
            // paste objects:
            uint32_t number_of_objects;
            sbi >> number_of_objects;
            
            for(uint32_t i = 0; i < number_of_objects; i++)
            {
                const flip::Mold mold(model::DataModel::use(), sbi);
                
                std::string object_name;
                sbi >> object_name;
                
                flip::Ref old_object_ref;
                sbi >> old_object_ref;
                
                model::Object& new_object = m_patcher_model.addObject(object_name, mold);
                new_object.setPosition(new_object.getX() + delta.x, new_object.getY() + delta.y);
                m_view_model.selectObject(new_object);
                
                molded_objects.insert({old_object_ref, &new_object});
            }
            
            // paste links:
            uint32_t number_of_links;
            sbi >> number_of_links;
            
            for(uint32_t i = 0; i < number_of_links; i++)
            {
                flip::Ref old_sender_ref, old_receiver_ref;
                sbi >> old_sender_ref;
                sbi >> old_receiver_ref;
                
                uint32_t outlet, inlet;
                sbi >> outlet;
                sbi >> inlet;
                
                const auto from_it = molded_objects.find(old_sender_ref);
                const auto to_it = molded_objects.find(old_receiver_ref);
                
                model::Object const* from = (from_it != molded_objects.cend()) ? from_it->second : nullptr;
                model::Object const* to = (to_it != molded_objects.cend()) ? to_it->second : nullptr;
                
                if(from && to)
                {
                    m_patcher_model.addLink(*from, outlet, *to, inlet);
                }
            }
            
            DocumentManager::commit(m_patcher_model, "paste objects");
        }
    }
    
    void PatcherView::duplicateSelection()
    {
        copySelectionToClipboard();
        pasteFromClipboard({m_grid_size, m_grid_size});
    }
    
    void PatcherView::cut()
    {
        copySelectionToClipboard();
        deleteSelection();
    }
    
    model::Object& PatcherView::replaceObjectWith(model::Object& object_to_remove, model::Object& new_object)
    {
        new_object.setPosition(object_to_remove.getX(), object_to_remove.getY());
        
        // re-link object
        const size_t new_inlets = new_object.getNumberOfInlets();
        const size_t new_outlets = new_object.getNumberOfOutlets();
        
        for(model::Link& link : m_patcher_model.getLinks())
        {
            if(!link.removed())
            {
                const model::Object& from = link.getSenderObject();
                const size_t outlet_index = link.getSenderIndex();
                const model::Object& to = link.getReceiverObject();
                const size_t inlet_index = link.getReceiverIndex();
                
                if(&from == &object_to_remove)
                {
                    if(outlet_index < new_outlets)
                    {
                        m_patcher_model.addLink(new_object, outlet_index, to, inlet_index);
                    }
                }
                
                if(&to == &object_to_remove)
                {
                    if(inlet_index < new_inlets)
                    {
                        m_patcher_model.addLink(from, outlet_index, new_object, inlet_index);
                    }
                }
            }
        }
        
        m_view_model.unselectObject(object_to_remove);
        m_patcher_model.removeObject(object_to_remove);
        
        m_view_model.selectObject(new_object);
        
        return new_object;
    }
    
    void PatcherView::pasteReplace()
    {
        if(isAnyObjectSelected())
        {
            auto& clipboard = m_instance.getPatcherClipboardData();
            if(!clipboard.empty())
            {
                std::vector<uint8_t> data(clipboard.begin(), clipboard.end());
                flip::StreamBinIn sbi(data);
                
                uint32_t number_of_objects_in_clipboard;
                sbi >> number_of_objects_in_clipboard;
                
                std::set<flip::Ref> const& selected_objects = getSelectedObjects();
                
                if(number_of_objects_in_clipboard == 1)
                {
                    flip::Mold mold(model::DataModel::use(), sbi);
                    
                    auto& document = m_patcher_model.entity().use<DocumentManager>();
                    
                    std::string new_object_name;
                    sbi >> new_object_name;
                    
                    flip::Ref old_object_ref;
                    sbi >> old_object_ref;
                    
                    for(auto const& obj_ref : selected_objects)
                    {
                        model::Object* selected_object = document.get<model::Object>(obj_ref);
                        if(selected_object != nullptr && !selected_object->removed())
                        {
                            try
                            {
                                model::Object& new_object = m_patcher_model.addObject(new_object_name,
                                                                                      mold);
                                replaceObjectWith(*selected_object, new_object);
                            }
                            catch(...)
                            {
                                KiwiApp::error("replace object failed");
                            }
                        }
                    }
                    
                    DocumentManager::commit(m_patcher_model, "paste-replace objects");
                }
            }
        }
    }
    
    bool PatcherView::isSelected(ObjectFrame const& object) const
    {
        return m_view_model.isSelected(object.getModel());
    }
    
    std::set<uint64_t> PatcherView::getDistantSelection(ObjectFrame const& object) const
    {
        std::set<uint64_t> distant_selection;
        
        if (m_distant_objects_selection.find(object.getModel().ref()) != m_distant_objects_selection.end())
        {
            distant_selection = m_distant_objects_selection.at(object.getModel().ref());
        }
        
        return distant_selection;
    }
    
    bool PatcherView::isSelected(LinkView const& link) const
    {
        return m_view_model.isSelected(link.getModel());
    }
    
    void PatcherView::addToSelectionBasedOnModifiers(ObjectFrame& object, bool select_only)
    {
        if(select_only)
        {
            selectObjectOnly(object);
        }
        else if(isSelected(object))
        {
            unselectObject(object);
        }
        else
        {
            selectObject(object);
        }
    }
    
    void PatcherView::addToSelectionBasedOnModifiers(LinkView& link, bool select_only)
    {
        if(select_only)
        {
            selectLinkOnly(link);
        }
        else if(isSelected(link))
        {
            unselectLink(link);
        }
        else
        {
            selectLink(link);
        }
    }
    
    bool PatcherView::selectOnMouseDown(ObjectFrame& object, bool select_only)
    {
        if(isSelected(object))
        {
            return true;
        }
        
        addToSelectionBasedOnModifiers(object, select_only);
        return false;
    }
    
    bool PatcherView::selectOnMouseDown(LinkView& link, bool select_only)
    {
        if(isSelected(link))
        {
            return true;
        }
        
        addToSelectionBasedOnModifiers(link, select_only);
        return false;
    }
    
    void PatcherView::selectOnMouseUp(ObjectFrame& object, bool select_only,
                                   const bool box_was_dragged, const bool result_of_mouse_down_select_method)
    {
        if(result_of_mouse_down_select_method && ! box_was_dragged)
        {
            addToSelectionBasedOnModifiers(object, select_only);
        }
    }
    
    
    void PatcherView::selectOnMouseUp(LinkView& link, bool select_only,
                                   const bool box_was_dragged, const bool result_of_mouse_down_select_method)
    {
        if(result_of_mouse_down_select_method && ! box_was_dragged)
        {
            addToSelectionBasedOnModifiers(link, select_only);
        }
    }
    
    void PatcherView::bringsLinksToFront()
    {
        for(auto& link_uptr : m_links)
        {
            link_uptr->toFront(false);
        }
    }
    
    void PatcherView::bringsObjectsToFront()
    {
        for(auto& object_uptr : m_objects)
        {
            object_uptr->toFront(false);
        }
    }
    
    bool PatcherView::keyPressed(const juce::KeyPress& key)
    {
        if(m_mouse_handler.getCurrentAction() == MouseHandler::Action::MoveObject ||
           m_mouse_handler.getCurrentAction() == MouseHandler::Action::ResizeObject)
            return false; // abort
        
        if(key.isKeyCode(juce::KeyPress::deleteKey) || key.isKeyCode(juce::KeyPress::backspaceKey))
        {
            deleteSelection();
            return true;
        }
        else if(key.isKeyCode(juce::KeyPress::returnKey))
        {
            if(m_local_objects_selection.size() == 1)
            {
                auto& doc = m_patcher_model.entity().use<DocumentManager>();
                
                model::Object* object_m = doc.get<model::Object>(*m_local_objects_selection.begin());
                if(object_m)
                {
                    const auto it = findObject(*object_m);
                    if(it != m_objects.cend())
                    {
                        editObject(**it);
                        return true;
                    }
                }
            }
        }
        else
        {
            const bool snap = key.getModifiers().isShiftDown();
            const int amount = snap ? m_grid_size : 1;
            
            if(key.isKeyCode(juce::KeyPress::rightKey))
            {
                moveSelectedObjects({amount, 0});
                return true;
            }
            else if(key.isKeyCode(juce::KeyPress::downKey))
            {
                moveSelectedObjects({0, amount});
                return true;
            }
            else if(key.isKeyCode(juce::KeyPress::leftKey))
            {
                moveSelectedObjects({-amount, 0});
                return true;
            }
            else if(key.isKeyCode(juce::KeyPress::upKey))
            {
                moveSelectedObjects({0, -amount});
                return true;
            }
        }
        
        return false;
    }
    
    void PatcherView::loadPatcher()
    {
        // create resident objects
        for(auto& object : m_patcher_model.getObjects())
        {
            if(object.resident()) { addObjectView(object); }
        }
        
        // create resident links
        for(auto& link : m_patcher_model.getLinks())
        {
            if(link.resident()) { addLinkView(link); }
        }
    }
    
    void PatcherView::setLock(bool locked)
    {
        if(locked)
        {
            m_view_model.unselectAll();
            m_io_highlighter.hide();
            grabKeyboardFocus();
        }
        
        m_view_model.setLock(locked);
        DocumentManager::commit(m_patcher_model, "Edit mode switch");
    }
    
    bool PatcherView::isLocked() const
    {
        return m_is_locked;
    }
    
    bool PatcherView::canConnect(model::Object const& from, const size_t outlet,
                              model::Object const& to, const size_t inlet) const
    {
        if((from.getNumberOfOutlets() > outlet) && (to.getNumberOfInlets() > inlet))
        {
            // Check if link does not exists.
            const auto find_link = [&from, &outlet, &to, &inlet](std::unique_ptr<LinkView> const& link_view_uptr)
            {
                model::Link& link_m = link_view_uptr->getModel();
                
                return (link_m.getSenderObject().ref()      == from.ref() &&
                        link_m.getReceiverObject().ref()    == to.ref() &&
                        link_m.getSenderIndex()             == outlet &&
                        link_m.getReceiverIndex()           == inlet);
            };
            
            // Check if inlets and outlets types are compatible
            
            if(std::find_if(m_links.begin(), m_links.end(), find_link) == m_links.cend())
            {
                return to.getInlet(inlet).hasType(from.getOutlet(outlet).getType());
            }
        }
        
        return false;
    }
    
    std::pair<ObjectFrame*, size_t> PatcherView::getLinkCreatorNearestEndingIolet()
    {
        ObjectFrame* result_object = nullptr;
        size_t result_index = 0;
        
        if(m_link_creator)
        {
            const ObjectFrame& binded_object = m_link_creator->getBindedObject();
            const juce::Point<int> end_pos = m_link_creator->getEndPosition();
            
            const int max_distance = 50;
            int min_distance = max_distance;
            
            for(auto& object_frame_uptr : m_objects)
            {
                if(object_frame_uptr.get() != &binded_object)
                {
                    model::Object const& object_m = object_frame_uptr->getModel();
                    
                    const bool sender = m_link_creator->isBindedToSender();
                    
                    const size_t io_size = sender ? object_m.getNumberOfInlets() : object_m.getNumberOfOutlets();
                    
                    for(size_t i = 0; i < io_size; ++i)
                    {
                        const juce::Point<int> io_pos = sender ? object_frame_uptr->getInletPatcherPosition(i) : object_frame_uptr->getOutletPatcherPosition(i);
                        
                        const int distance = end_pos.getDistanceFrom(io_pos);
                        
                        if(min_distance > distance)
                        {
                            model::Object const& binded_object_m = binded_object.getModel();
                            model::Object const& ending_object_m = object_m;
                            
                            model::Object const& from = sender ? binded_object_m : ending_object_m;
                            model::Object const& to = sender ? ending_object_m : binded_object_m;
                            
                            const size_t outlet = sender ? m_link_creator->getBindedIndex() : i;
                            const size_t inlet = sender ? i : m_link_creator->getBindedIndex();
                            
                            //if(m_patcher_model.canConnect(from, outlet, to, inlet))
                            if(canConnect(from, outlet, to, inlet))
                            {
                                min_distance = distance;
                                result_object = object_frame_uptr.get();
                                result_index = i;
                            }
                        }
                    }
                }
            }
        }
        
        return std::make_pair(result_object, result_index);
    }
    
    juce::Rectangle<int> PatcherView::getCurrentObjectsArea()
    {
        juce::Rectangle<int> area;
        
        for(auto& object_m : m_patcher_model.getObjects())
        {
            if(! object_m.removed())
            {
                juce::Rectangle<int> object_bounds(object_m.getX(),
                                                   object_m.getY(),
                                                   object_m.getWidth(),
                                                   object_m.getHeight());
                
                if(object_bounds.getX() <= area.getX())
                {
                    area.setLeft(object_bounds.getX());
                }
                
                if(object_bounds.getY() <= area.getY())
                {
                    area.setTop(object_bounds.getY());
                }
                
                if(object_bounds.getBottom() >= area.getBottom())
                {
                    area.setBottom(object_bounds.getBottom());
                }
                
                if(object_bounds.getRight() >= area.getRight())
                {
                    area.setRight(object_bounds.getRight());
                }
            }
        }
        
        return area;
    }
    
    juce::Rectangle<int> PatcherView::getSelectionBounds()
    {
        juce::Rectangle<int> area;
        
        int counter = 0;
        for(auto* object_m : m_view_model.getSelectedObjects())
        {
            counter++;
            if(object_m && !object_m->removed())
            {
                juce::Rectangle<int> object_bounds(object_m->getX(),
                                                   object_m->getY(),
                                                   object_m->getWidth(),
                                                   object_m->getHeight());
                
                if(counter == 1)
                {
                    area = object_bounds;
                }
                else
                {
                    if(object_bounds.getX() < area.getX())
                    {
                        area.setLeft(object_bounds.getX());
                    }
                    
                    if(object_bounds.getY() < area.getY())
                    {
                        area.setTop(object_bounds.getY());
                    }
                    
                    if(object_bounds.getBottom() > area.getBottom())
                    {
                        area.setBottom(object_bounds.getBottom());
                    }
                    
                    if(object_bounds.getRight() > area.getRight())
                    {
                        area.setRight(object_bounds.getRight());
                    }
                }
            }
        }
        
        return area;
    }
    
    juce::Point<int> PatcherView::getOriginPosition() const
    {
        return m_viewport.getOriginPosition();
    }
    
    void PatcherView::originPositionChanged()
    {
        for(auto& jbox_uptr : m_objects)
        {
            jbox_uptr->patcherViewOriginPositionChanged();
        }
        
        repaint();
    }
    
    void PatcherView::zoomIn()
    {
        m_view_model.setZoomFactor(m_view_model.getZoomFactor() + 0.25);
        DocumentManager::commit(m_patcher_model);
        
        if(isAnyObjectSelected())
        {
            m_viewport.bringRectToCentre(getSelectionBounds());
        }
    }
    
    void PatcherView::zoomNormal()
    {
        m_view_model.setZoomFactor(1.);
        DocumentManager::commit(m_patcher_model);
        
        if(isAnyObjectSelected())
        {
            m_viewport.bringRectToCentre(getSelectionBounds());
        }
    }
    
    void PatcherView::zoomOut()
    {
        const double zoom = m_view_model.getZoomFactor();
        if(zoom > 0.25)
        {
            m_view_model.setZoomFactor(zoom - 0.25);
            DocumentManager::commit(m_patcher_model);
            
            if(isAnyObjectSelected())
            {
                m_viewport.bringRectToCentre(getSelectionBounds());
            }
        }
    }
    
    void PatcherView::connectedUserChanged(PatcherManager& manager)
    {
        if(&m_manager == &manager)
        {
            auto& patcher = manager.getPatcher();
            checkObjectsSelectionChanges(patcher);
            checkLinksSelectionChanges(patcher);
        }
    }
    
    // ================================================================================ //
    //                                     OBSERVER                                     //
    // ================================================================================ //
    
    void PatcherView::patcherChanged(model::Patcher& patcher, model::Patcher::View& view)
    {
        if(! patcher.changed()) return; // abort
        
        if(view.added()) {}
        
        // create ObjectView for each newly added objects
        int object_zorder = -1;
        for(auto& object : patcher.getObjects())
        {
            object_zorder++;
            if(object.added()) { addObjectView(object, object_zorder); }
        }
        
        // create LinkView for each newly added links
        for(auto& link : patcher.getLinks())
        {
            if(link.added()) { addLinkView(link); }
        }
        
        bool patcher_area_uptodate = false;
        
        // send ObjectView change notification
        for(auto& object : patcher.getObjects())
        {
            if(object.changed())
            {
                if(object.boundsChanged()
                   && !patcher_area_uptodate
                   && !view.removed()
                   && m_mouse_handler.getCurrentAction() != MouseHandler::Action::MoveObject
                   && m_mouse_handler.getCurrentAction() != MouseHandler::Action::ResizeObject)
                {
                    m_viewport.updatePatcherArea(true);
                    patcher_area_uptodate = true;
                }
                
                objectChanged(view, object);
            }
        }
        
        // send LinkView change notification
        for(auto& link : patcher.getLinks())
        {
            if(link.changed()) { linkChanged(link); }
            
            // send to LinkView ObjectView change notification
            if(patcher.objectsChanged())
            {
                for(auto& object : patcher.getObjects())
                {
                    if(object.changed())
                    {
                        LinkView* link_view = getLink(link);
                        
                        if(link_view) { link_view->objectChanged(object); }
                    }
                }
            }
        }
        
        if(!view.removed() && patcher.nameChanged())
        {
            updateWindowTitle();
        }
        
        if(!view.removed() && &view == &m_view_model)
        {
            checkViewInfos(view);
            checkObjectsSelectionChanges(patcher);
            checkLinksSelectionChanges(patcher);
        }
        
        // delete LinkView for each removed links
        for(auto& link : patcher.getLinks())
        {
            if(link.removed()) { removeLinkView(link); }
        }
        
        // delete ObjectView for each removed objects
        for(auto& object : patcher.getObjects())
        {
            if(object.removed())
            {
                // remove link_creator if binded object has been removed.
                if(m_link_creator)
                {
                    model::Object const& binded_object = m_link_creator->getBindedObject().getModel();
                    if(&binded_object == &object)
                    {
                        removeChildComponent(m_link_creator.get());
                        m_link_creator.reset();
                        m_io_highlighter.hide();
                    }
                }
                
                removeObjectView(object);
            }
        }
        
        if(view.removed()) {}
        
        if(patcher.resident() && (patcher.objectsChanged() || patcher.linksChanged()))
        {
            updateWindowTitle();
        }
    }
    
    void PatcherView::updateWindowTitle() const
    {
        PatcherViewWindow* window = findParentComponentOfClass<PatcherViewWindow>();
        if(window)
        {
            juce::String title;
            
            if(!m_manager.isRemote())
            {
                title = m_patcher_model.getName();
                const bool edited = m_manager.needsSaving();
                
                juce::File kiwi_file = DocumentManager::getSelectedFile(m_patcher_model);
                
                if(juce::ComponentPeer* peer = window->getPeer())
                {
                    if (!peer->setDocumentEditedStatus(edited))
                        if (edited)
                            title << "*";
                    
                    peer->setRepresentedFile(kiwi_file);
                }
            }
            else
            {
                title = "[Remote] " + m_manager.getDocumentName();
            }
            
            std::string new_name = title.toStdString() + (isLocked() ? "" : " (edit) ");
            window->setName(new_name);
        }
    }
    
    void PatcherView::checkViewInfos(model::Patcher::View& view)
    {
        if(&view == &m_view_model && !view.removed())
        {
            const bool was_locked = m_is_locked;
            m_is_locked = view.getLock();
            if(was_locked != m_is_locked)
            {
                m_is_locked ? bringsObjectsToFront() : bringsLinksToFront();
                
                for(auto& object : m_objects)
                {
                    object->lockStatusChanged();
                }
                
                if(m_is_locked)
                {
                    m_viewport.resetObjectsArea();
                }
                
                updateWindowTitle();
                
                repaint();
                KiwiApp::commandStatusChanged();
            }
            
            if(m_view_model.zoomFactorChanged())
            {
                const double zoom = m_view_model.getZoomFactor();
                m_viewport.setZoomFactor(zoom);
            }
        }
    }
    
    void PatcherView::checkObjectsSelectionChanges(model::Patcher& patcher)
    {
        const auto connected_users = m_manager.getConnectedUsers();
        
        std::set<flip::Ref>                     new_local_objects_selection;
        std::map<flip::Ref, std::set<uint64_t>> new_distant_objects_selection;
        
        for(auto& object_m : patcher.getObjects())
        {
            if(!object_m.removed())
            {
                std::set<uint64_t> selected_for_users;
                
                for(auto& user : patcher.getUsers())
                {
                    bool selected_for_local_view = false;
                    bool selected_for_other_view = false;
                    
                    const uint64_t user_id = user.getId();
                    const bool is_distant_user = user_id != m_instance.getUserId();
                    const bool is_connected = (connected_users.find(user_id) != connected_users.end());
                    
                    if(is_distant_user && !is_connected)
                    {
                        continue;
                    }
                    
                    for(auto& view : user.getViews())
                    {
                        const bool is_local_view = (&m_view_model == &view);
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
                        new_local_objects_selection.emplace(object_m.ref());
                    }
                    
                    if(selected_for_other_view)
                    {
                        selected_for_users.emplace(user_id);
                    }
                }
                
                new_distant_objects_selection.insert({object_m.ref(), selected_for_users});
            }
        }
        
        std::set<ObjectFrame*> updated_objects;
        
        // check diff between old and new distant selection
        // and notify objects if their selection state changed
        for(auto& local_object_uptr : m_objects)
        {
            model::Object const& local_object_m = local_object_uptr->getModel();
            
            // local diff
            const bool old_local_selected_state = m_local_objects_selection.find(local_object_m.ref()) != m_local_objects_selection.end();
            
            bool new_local_selected_state = new_local_objects_selection.find(local_object_m.ref()) != new_local_objects_selection.end();
            
            if(old_local_selected_state != new_local_selected_state)
            {
                updated_objects.insert(local_object_uptr.get());
                selectionChanged();
            }
            
            // distant diff
            bool distant_selection_changed_for_object = false;
            for(auto distant_it : new_distant_objects_selection)
            {
                flip::Ref const& distant_object_lookup_ref = distant_it.first;
                
                if(distant_object_lookup_ref == local_object_uptr->getModel().ref())
                {
                    distant_selection_changed_for_object =
                    m_distant_objects_selection[distant_object_lookup_ref] != distant_it.second;
                    
                    // notify object
                    if(distant_selection_changed_for_object)
                    {
                        updated_objects.insert(local_object_uptr.get());
                        selectionChanged();
                    }
                }
            }
        }
        
        // cache new selection state
        std::swap(m_distant_objects_selection, new_distant_objects_selection);
        std::swap(m_local_objects_selection, new_local_objects_selection);
        
        // call objects reaction.
        
        for(auto object_frame : updated_objects)
        {
            object_frame->selectionChanged();
        }
    }
    
    void PatcherView::checkLinksSelectionChanges(model::Patcher& patcher)
    {
        const auto connected_users = m_manager.getConnectedUsers();
        
        std::set<flip::Ref>                     new_local_links_selection;
        std::map<flip::Ref, std::set<uint64_t>> new_distant_links_selection;
        
        for(auto& link_m : patcher.getLinks())
        {
            if(link_m.removed()) break;
            
            std::set<uint64_t> selected_for_users;
            
            for(auto& user : patcher.getUsers())
            {
                bool selected_for_local_view = false;
                bool selected_for_other_view = false;
                
                const uint64_t user_id = user.getId();
                const bool is_distant_user = user_id != m_instance.getUserId();
                const bool is_connected = (connected_users.find(user_id) != connected_users.end());
                
                if(is_distant_user && !is_connected)
                {
                    continue;
                }
                
                for(auto& view : user.getViews())
                {
                    const bool is_local_view = (&m_view_model == &view);
                    const bool is_selected = view.isSelected(link_m);
                    
                    if(is_selected)
                    {
                        if(is_distant_user)
                        {
                            selected_for_other_view = true;
                            
                            // a link is considered selected for a given user
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
                    new_local_links_selection.emplace(link_m.ref());
                }
                
                if(selected_for_other_view)
                {
                    selected_for_users.emplace(user_id);
                }
            }
            
            new_distant_links_selection.insert({link_m.ref(), selected_for_users});
        }
        
        // check diff between old and new distant selection
        // and notify links if their selection state changed
        for(auto& local_link_uptr : m_links)
        {
            model::Link const& local_link_m = local_link_uptr->getModel();
            
            // local diff
            const bool old_local_selected_state = m_local_links_selection.find(local_link_m.ref()) != m_local_links_selection.end();
            
            bool new_local_selected_state = new_local_links_selection.find(local_link_m.ref()) != new_local_links_selection.end();
            
            if(old_local_selected_state != new_local_selected_state)
            {
                local_link_uptr->localSelectionChanged(new_local_selected_state);
                selectionChanged();
            }
            
            // distant diff
            bool distant_selection_changed_for_link = false;
            for(auto distant_it : new_distant_links_selection)
            {
                flip::Ref const& distant_link_lookup_ref = distant_it.first;
                
                if(distant_link_lookup_ref == local_link_uptr->getModel().ref())
                {
                    distant_selection_changed_for_link =
                    m_distant_links_selection[distant_link_lookup_ref] != distant_it.second;
                    
                    // notify link
                    if(distant_selection_changed_for_link)
                    {
                        local_link_uptr->distantSelectionChanged(distant_it.second);
                        selectionChanged();
                    }
                }
            }
        }
        
        // cache new selection state
        std::swap(m_distant_links_selection, new_distant_links_selection);
        std::swap(m_local_links_selection, new_local_links_selection);
    }
    
    void PatcherView::selectionChanged()
    {
        KiwiApp::commandStatusChanged();
    }
    
    void PatcherView::addObjectView(model::Object& object, int zorder)
    {
        const auto it = findObject(object);
        
        if(it == m_objects.cend())
        {
            const auto it = (zorder > 0) ? m_objects.begin() + zorder : m_objects.end();
            
            std::unique_ptr<ObjectView> object_view = Factory::createObjectView(object);
            
            ObjectFrame& object_frame = **(m_objects.emplace(it, new ObjectFrame(*this, std::move(object_view))));
            
            addAndMakeVisible(object_frame, zorder);
        }
    }
    
    void PatcherView::objectChanged(model::Patcher::View& view, model::Object& object)
    {
        const auto it = findObject(object);
        
        if(it != m_objects.cend())
        {
            ObjectFrame& object_view = *it->get();
            object_view.objectChanged(view, object);
        }
    }
    
    void PatcherView::removeObjectView(model::Object& object)
    {
        const auto it = findObject(object);
        
        if(it != m_objects.cend())
        {
            ObjectFrame* object_view = it->get();
            
            if(m_hittester.getObject() == object_view)
            {
                // reset hittester to prevent functions like mouseUp() to get an invalid ptr.
                m_hittester.reset();
            }
            
            juce::ComponentAnimator& animator = juce::Desktop::getInstance().getAnimator();
            animator.animateComponent(object_view, object_view->getBounds(), 0., 200., true, 0.8, 1.);
            
            removeChildComponent(object_view);
            m_objects.erase(it);
        }
    }
    
    void PatcherView::addLinkView(model::Link& link)
    {
        const auto it = findLink(link);
        
        if(it == m_links.cend())
        {
            auto result = m_links.emplace(m_links.end(), new LinkView(*this, link));
            
            LinkView& link_view = *result->get();
            addAndMakeVisible(link_view);
        }
    }
    
    void PatcherView::linkChanged(model::Link& link)
    {
        const auto it = findLink(link);
        
        if(it != m_links.cend())
        {
            LinkView& link_view = *it->get();
            link_view.linkChanged(link);
        }
    }
    
    void PatcherView::removeLinkView(model::Link& link)
    {
        const auto it = findLink(link);
        
        if(it != m_links.cend())
        {
            removeChildComponent(it->get());
            m_links.erase(it);
        }
    }
    
    PatcherView::ObjectFrames::iterator PatcherView::findObject(model::Object const& object)
    {
        const auto find_jobj = [&object](std::unique_ptr<ObjectFrame> const& jobj)
        {
            return (&object == &jobj->getModel());
        };
        
        return std::find_if(m_objects.begin(), m_objects.end(), find_jobj);
    }
    
    PatcherView::LinkViews::iterator PatcherView::findLink(model::Link const& link)
    {
        const auto find_link_view = [&link](std::unique_ptr<LinkView> const& link_view)
        {
            return (&link == &link_view->getModel());
        };
        
        return std::find_if(m_links.begin(), m_links.end(), find_link_view);
    }
    
    model::Patcher::View& PatcherView::getPatcherViewModel()
    {
        return m_view_model;
    }
    
    PatcherView::ObjectFrames const& PatcherView::getObjects() const
    {
        return m_objects;
    }
    
    PatcherView::LinkViews const& PatcherView::getLinks() const
    {
        return m_links;
    }
    
    ObjectFrame* PatcherView::getObject(model::Object const& object)
    {
        const auto it = findObject(object);
        return (it != m_objects.cend()) ? it->get() : nullptr;
    }
    
    LinkView* PatcherView::getLink(model::Link const& link)
    {
        const auto it = findLink(link);
        return (it != m_links.cend()) ? it->get() : nullptr;
    }
    
    // ================================================================================ //
    //                                  COMMANDS ACTIONS                                //
    // ================================================================================ //
    
    bool PatcherView::isEditingObject() const
    {
        return m_box_being_edited != nullptr;
    }

    void PatcherView::editObject(ObjectFrame & object_frame)
    {
        assert(!isEditingObject() && "Editing two objects simultaneously");
        
        object_frame.editObject();
    }
    
    void PatcherView::objectEditorShown(ObjectFrame const& object_frame)
    {
        m_box_being_edited = &object_frame;
        
        KiwiApp::commandStatusChanged();
    }
    
    void PatcherView::objectEditorHidden(ObjectFrame const& object_frame)
    {
        assert(m_box_being_edited == &object_frame && "Calling object editor shown outside text edition");
        
        m_box_being_edited = nullptr;
        
        KiwiApp::commandStatusChanged();
    }
    
    void PatcherView::objectTextChanged(ObjectFrame const& object_frame, std::string const& new_text)
    {
        model::Object & old_model = object_frame.getModel();
        
        std::vector<Atom> atoms = AtomHelper::parse(new_text);
        
        std::unique_ptr<model::Object> object_model = model::Factory::create(atoms);
        
        juce::Point<int> origin = getOriginPosition();
        juce::Rectangle<int> box_bounds = object_frame.getObjectBounds();
        
        object_model->setPosition(box_bounds.getX() - origin.x, box_bounds.getY() - origin.y);
        
        // handle error box case
        if(object_model->getName() == "errorbox")
        {
            model::ErrorBox& error_box = dynamic_cast<model::ErrorBox&>(*object_model);
            error_box.setInlets(old_model.getInlets());
            error_box.setOutlets(old_model.getOutlets());
            KiwiApp::error(error_box.getError());
        }
        
        if (!object_model->hasFlag(model::Flag::IFlag::DefinedSize))
        {
            const int text_width = juce::Font().getStringWidth(new_text) + 12;
            const int max_io = std::max(object_model->getNumberOfInlets(),
                                        object_model->getNumberOfOutlets()) * 14;
            
            object_model->setWidth(std::max(text_width, max_io));
            object_model->setHeight(box_bounds.getHeight());
        }

        model::Object & new_object = m_patcher_model.replaceObject(old_model, std::move(object_model));
        
        m_view_model.unselectObject(old_model);
        
        if(!isLocked())
        {
            m_view_model.selectObject(new_object);
        }
        
        DocumentManager::commit(m_patcher_model, "Edit Object");
    }
    
    void PatcherView::createObjectModel(std::string const& text, bool give_focus)
    {
        if(! DocumentManager::isInCommitGesture(m_patcher_model))
        {
            bool linked_newbox = m_local_objects_selection.size() == 1;
            
            std::unique_ptr<model::Object> new_object = model::Factory::create(AtomHelper::parse(text));
            
            juce::Point<int> pos = getMouseXYRelative() - getOriginPosition();
            
            auto& doc = m_patcher_model.entity().use<DocumentManager>();
            
            if(linked_newbox)
            {
                model::Object* obj = doc.get<model::Object>(*m_local_objects_selection.begin());
                
                if(obj)
                {
                    pos.setX(obj->getX());
                    pos.setY(obj->getY() + obj->getHeight() + m_grid_size);
                    
                    if(obj->getNumberOfInlets() >= 1)
                    {
                        m_patcher_model.addLink(*obj, 0, *new_object, 0);
                    }
                }
            }
            
            new_object->setPosition(pos.x, pos.y);
            
            m_view_model.unselectAll();
            
            m_view_model.selectObject(m_patcher_model.addObject(std::move(new_object)));
            
            DocumentManager::commit(m_patcher_model, "Insert New Empty Box");

            if(give_focus && m_local_objects_selection.size() == 1)
            {
                model::Object* object_m = doc.get<model::Object>(*m_local_objects_selection.begin());
                if(object_m)
                {
                    const auto it = findObject(*object_m);
                    if(it != m_objects.cend())
                    {
                        editObject(**it);
                    }
                }
            }
        }
    }
    
    // ================================================================================ //
    //                                     UNDO/REDO                                    //
    // ================================================================================ //
    
    void PatcherView::undo()
    {
        auto& doc = m_patcher_model.entity().use<DocumentManager>();
        if(doc.canUndo())
        {
            doc.undo();
            doc.commit(m_patcher_model);
        }
    }
    
    bool PatcherView::canUndo()
    {
        return m_patcher_model.entity().use<DocumentManager>().canUndo();
    }
    
    std::string PatcherView::getUndoLabel()
    {
        auto& doc = m_patcher_model.entity().use<DocumentManager>();
        return doc.canUndo() ? doc.getUndoLabel() : "";
    }

    void PatcherView::redo()
    {
        auto& doc = m_patcher_model.entity().use<DocumentManager>();
        if(doc.canRedo())
        {
            doc.redo();
            doc.commit(m_patcher_model);
        }
    }
    
    bool PatcherView::canRedo()
    {
        return m_patcher_model.entity().use<DocumentManager>().canRedo();
    }
    
    std::string PatcherView::getRedoLabel()
    {
        auto& doc = m_patcher_model.entity().use<DocumentManager>();
        return doc.canRedo() ? doc.getRedoLabel() : "";
    }
    
    // ================================================================================ //
    //                                     SELECTION                                    //
    // ================================================================================ //
    
    bool PatcherView::isAnythingSelected()
    {
        return isAnyObjectSelected() || isAnyLinksSelected();
    }
    
    bool PatcherView::isAnyObjectSelected()
    {
        return !m_local_objects_selection.empty();
    }
    
    bool PatcherView::isAnyLinksSelected()
    {
        return !m_local_links_selection.empty();
    }
    
    void PatcherView::selectObject(ObjectFrame& object)
    {
        m_view_model.selectObject(object.getModel());
        DocumentManager::commit(m_patcher_model);
    }
    
    void PatcherView::selectObjects(std::vector<ObjectFrame*> const& objects)
    {
        bool should_commit = false;
        
        for(ObjectFrame* object : objects)
        {
            if(object != nullptr)
            {
                m_view_model.selectObject(object->getModel());
                should_commit = true;
            }
        }
        
        if(should_commit)
        {
            DocumentManager::commit(m_patcher_model);
        }
    }
    
    void PatcherView::selectLink(LinkView& link)
    {
        m_view_model.selectLink(link.getModel());
        DocumentManager::commit(m_patcher_model);
    }
    
    void PatcherView::selectLinks(std::vector<LinkView*> const& links)
    {
        bool should_commit = false;
        
        for(LinkView* link : links)
        {
            if(link != nullptr)
            {
                m_view_model.selectLink(link->getModel());
                should_commit = true;
            }
        }
        
        if(should_commit)
        {
            DocumentManager::commit(m_patcher_model);
        }
    }
    
    void PatcherView::unselectObject(ObjectFrame& object)
    {
        m_view_model.unselectObject(object.getModel());
        DocumentManager::commit(m_patcher_model);
    }
    
    void PatcherView::unselectLink(LinkView& link)
    {
        m_view_model.unselectLink(link.getModel());
        DocumentManager::commit(m_patcher_model);
    }
    
    void PatcherView::selectObjectOnly(ObjectFrame& object)
    {
        unselectAll();
        selectObject(object);
        DocumentManager::commit(m_patcher_model);
    }

    void PatcherView::selectLinkOnly(LinkView& link)
    {
        unselectAll();
        selectLink(link);
        DocumentManager::commit(m_patcher_model);
    }
    
    void PatcherView::selectAllObjects()
    {
        m_view_model.selectAll();
        DocumentManager::commit(m_patcher_model);
    }
    
    void PatcherView::unselectAll()
    {
        if(!DocumentManager::isInCommitGesture(m_patcher_model))
        {
            m_view_model.unselectAll();
            DocumentManager::commit(m_patcher_model);
        }
    }
    
    void PatcherView::deleteSelection()
    {
        for(model::Link* link : m_view_model.getSelectedLinks())
        {
            if(link)
            {
                m_patcher_model.removeLink(*link, &m_view_model);
            }
        }
        
        for(model::Object* object : m_view_model.getSelectedObjects())
        {
            if(object)
            {
                m_patcher_model.removeObject(*object, &m_view_model);
            }
        }
        
        DocumentManager::commit(m_patcher_model, "Delete objects and links");
        
        m_viewport.updatePatcherArea(false);
    }
    
    // ================================================================================ //
    //                              APPLICATION COMMAND TARGET                          //
    // ================================================================================ //
    
    juce::ApplicationCommandTarget* PatcherView::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void PatcherView::getAllCommands(juce::Array<juce::CommandID>& commands)
    {
        commands.add(CommandIDs::save);
        
        commands.add(CommandIDs::newPatcherView);
        
        commands.add(juce::StandardApplicationCommandIDs::undo);
        commands.add(juce::StandardApplicationCommandIDs::redo);
        commands.add(juce::StandardApplicationCommandIDs::cut);
        commands.add(juce::StandardApplicationCommandIDs::copy);
        commands.add(juce::StandardApplicationCommandIDs::paste);
        commands.add(CommandIDs::pasteReplace);
        commands.add(CommandIDs::duplicate);
        commands.add(juce::StandardApplicationCommandIDs::del);
        commands.add(juce::StandardApplicationCommandIDs::selectAll);
        
        commands.add(CommandIDs::newBox);
        commands.add(CommandIDs::newBang);
        commands.add(CommandIDs::newToggle);
        
        commands.add(CommandIDs::zoomIn);
        commands.add(CommandIDs::zoomOut);
        commands.add(CommandIDs::zoomNormal);
        commands.add(CommandIDs::editModeSwitch);
        commands.add(CommandIDs::gridModeSwitch);
        commands.add(CommandIDs::enableSnapToGrid);
        
        commands.add(CommandIDs::showPatcherInspector);
        commands.add(CommandIDs::showObjectInspector);
    }
    
    void PatcherView::getCommandInfo(const juce::CommandID commandID, juce::ApplicationCommandInfo& result)
    {
        const bool is_not_in_gesture = !DocumentManager::isInCommitGesture(m_patcher_model);
        
        switch(commandID)
        {
            case CommandIDs::save:
            {
                result.setInfo(TRANS("Save"), TRANS("Save document"), CommandCategories::general, 0);
                result.addDefaultKeypress('s', juce::ModifierKeys::commandModifier);
                result.setActive(!m_manager.isRemote());
                break;
            }
            case CommandIDs::newPatcherView:
            {
                result.setInfo(TRANS("New View"), TRANS("New Patcher View"), CommandCategories::view, 0);
                result.addDefaultKeypress('n', juce::ModifierKeys::commandModifier | juce::ModifierKeys::shiftModifier);
                break;
            }
            case juce::StandardApplicationCommandIDs::undo:
            {
                juce::String label = TRANS("Undo");
                const bool hasUndo = canUndo();
                if(hasUndo) { label += ' ' + getUndoLabel(); }
                
                result.setInfo(label, TRANS("Undo last action"), CommandCategories::general, 0);
                result.addDefaultKeypress('z',  juce::ModifierKeys::commandModifier);
                result.setActive(!isLocked() && hasUndo);
                break;
            }
            case juce::StandardApplicationCommandIDs::redo:
            {
                juce::String label = TRANS("Redo");
                const bool hasRedo = canRedo();
                if(hasRedo) { label += ' ' + getRedoLabel(); }
                
                result.setInfo(label, TRANS("Redo action"), CommandCategories::general, 0);
                result.addDefaultKeypress('z', juce::ModifierKeys::commandModifier | juce::ModifierKeys::shiftModifier);
                result.setActive(!isLocked() && hasRedo);
                break;
            }
            case juce::StandardApplicationCommandIDs::cut:
            {
                result.setInfo(TRANS("Cut"), TRANS("Cut"), CommandCategories::editing, 0);
                result.addDefaultKeypress('x', juce::ModifierKeys::commandModifier);
                result.setActive(!isLocked()
                                 && isAnyObjectSelected());
                break;
            }
            case juce::StandardApplicationCommandIDs::copy:
            {
                result.setInfo(TRANS("Copy"), TRANS("Copy"), CommandCategories::editing, 0);
                result.addDefaultKeypress('c', juce::ModifierKeys::commandModifier);
                result.setActive(!isLocked()
                                 && isAnyObjectSelected());
                break;
            }
            case juce::StandardApplicationCommandIDs::paste:
            {
                result.setInfo(TRANS("Paste"), TRANS("Paste"), CommandCategories::editing, 0);
                result.addDefaultKeypress('v', juce::ModifierKeys::commandModifier);
                result.setActive(!isLocked()
                                 && !m_instance.getPatcherClipboardData().empty());
                break;
            }
            case CommandIDs::pasteReplace:
            {
                result.setInfo(TRANS("Paste replace"),
                               TRANS("Replace selected objects with the object on the clipboard"),
                               CommandCategories::editing, 0);
                
                result.addDefaultKeypress('v', juce::ModifierKeys::commandModifier | juce::ModifierKeys::altModifier);
                result.setActive(!isLocked()
                                 && isAnyObjectSelected()
                                 && !m_instance.getPatcherClipboardData().empty());
                break;
            }
            case CommandIDs::duplicate:
            {
                result.setInfo(TRANS("Duplicate"), TRANS("Duplicate the selection"),
                               CommandCategories::editing, 0);
                
                result.addDefaultKeypress('d', juce::ModifierKeys::commandModifier);
                result.setActive(!isLocked()
                                 && isAnyObjectSelected());
                break;
            }
            case juce::StandardApplicationCommandIDs::del:
            {
                result.setInfo(TRANS("Delete"), TRANS("Delete all selected objects and links"),
                               CommandCategories::editing, 0);
                
                result.addDefaultKeypress(juce::KeyPress::backspaceKey, juce::ModifierKeys::noModifiers);
                result.setActive(!isLocked()
                                 && isAnythingSelected());
                break;
            }
            case juce::StandardApplicationCommandIDs::selectAll:
            {
                result.setInfo(TRANS("Select All"), TRANS("Select all boxes and links"), CommandCategories::editing, 0);
                result.addDefaultKeypress('a', juce::ModifierKeys::commandModifier);
                result.setActive(!isLocked());
                break;
            }
            case CommandIDs::newBox:
            {
                result.setInfo(TRANS("New Object Box"), TRANS("Add a new object box"), CommandCategories::editing, 0);
                result.addDefaultKeypress('n', juce::ModifierKeys::noModifiers);
                result.setActive(!isLocked());
                break;
            }
            case CommandIDs::newBang:
            {
                result.setInfo(TRANS("New Bang Box"), TRANS("Add a new bang"), CommandCategories::editing, 0);
                result.addDefaultKeypress('b', juce::ModifierKeys::noModifiers);
                result.setActive(!isLocked());
                break;
            }
            case CommandIDs::newToggle:
            {
                result.setInfo(TRANS("New Toggle Box"), TRANS("Add a new toggle"), CommandCategories::editing, 0);
                result.addDefaultKeypress('t', juce::ModifierKeys::noModifiers);
                result.setActive(!isLocked());
                break;
            }
            case CommandIDs::zoomIn:
            {
                result.setInfo(TRANS("Zoom in"), TRANS("Zoom in"), CommandCategories::view, 0);
                result.addDefaultKeypress('=', juce::ModifierKeys::commandModifier);
                break;
            }
            case CommandIDs::zoomOut:
            {
                result.setInfo(TRANS("Zoom out"), TRANS("Zoom out"), CommandCategories::view, 0);
                result.addDefaultKeypress('-', juce::ModifierKeys::commandModifier);
                break;
            }
            case CommandIDs::zoomNormal:
            {
                result.setInfo(TRANS("Zoom at 100%"), TRANS("Zoom reset"), CommandCategories::view, 0);
                result.addDefaultKeypress('1', juce::ModifierKeys::commandModifier | juce::ModifierKeys::shiftModifier);
                break;
            }
            case CommandIDs::editModeSwitch:
            {
                result.setInfo(TRANS("Edit"),
                               TRANS("Switch between edit and perform mode"),
                               CommandCategories::view, 0);
                
                result.addDefaultKeypress ('e', juce::ModifierKeys::commandModifier);
                result.setTicked(!m_view_model.getLock());
                break;
            }
            default:
            {
                result.setInfo (TRANS("[unknown command]"), TRANS("[unknown command]"), CommandCategories::view, 0);
                break;
            }
        }
        
        result.setActive(!(result.flags & juce::ApplicationCommandInfo::isDisabled)
                         && is_not_in_gesture
                         && !isEditingObject());
    }
    
    bool PatcherView::perform(const InvocationInfo& info)
    {
        // most of the commands below generate conflicts when they are being executed
        // in a commit gesture or when a box is being edited, so simply not execute them.
        if(DocumentManager::isInCommitGesture(m_patcher_model) || isEditingObject())
        {
            return true;
        }
            
        switch (info.commandID)
        {
            case CommandIDs::save:                              { m_manager.saveDocument(); break; }
            case CommandIDs::newPatcherView:                    { m_manager.newView(); break; }
                
            case juce::StandardApplicationCommandIDs::undo:     { undo(); break; }
            case juce::StandardApplicationCommandIDs::redo:     { redo(); break; }
                
            case juce::StandardApplicationCommandIDs::cut:      { cut(); break; }
            case juce::StandardApplicationCommandIDs::copy:     { copySelectionToClipboard(); break; }
            case juce::StandardApplicationCommandIDs::paste:
            { pasteFromClipboard({m_grid_size , m_grid_size}); break; }
                
            case CommandIDs::pasteReplace:                      { pasteReplace(); break; }
            case CommandIDs::duplicate:                         { duplicateSelection(); break; }
            case juce::StandardApplicationCommandIDs::del:      { deleteSelection(); break; }
            case juce::StandardApplicationCommandIDs::selectAll:{ selectAllObjects(); break; }
            
            case CommandIDs::newBox:                            { createObjectModel("", true); break; }
            case CommandIDs::newBang:                           { createObjectModel("bang", true); break; }
            case CommandIDs::newToggle:                         { createObjectModel("toggle", true); break; }
                
            case CommandIDs::zoomIn:                            { zoomIn(); break; }
            case CommandIDs::zoomOut:                           { zoomOut(); break; }
            case CommandIDs::zoomNormal:                        { zoomNormal(); break; }
            case CommandIDs::editModeSwitch:                    { setLock(!isLocked()); break; }
                
            default: return false;
        }
        
        return true;
    }
}
