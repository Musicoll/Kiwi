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

#ifndef KIWI_JPATCHER_HPP_INCLUDED
#define KIWI_JPATCHER_HPP_INCLUDED

#include <KiwiModel/KiwiPatcher.hpp>

#include "flip/DocumentObserver.h"

#include "../JuceLibraryCode/JuceHeader.h"

namespace kiwi
{
    class jObject;
    class jLink;
    class jInstance;
    class HitTester;
    
    //! @brief The juce Patcher Component.
    class jPatcher : public juce::Component, public ApplicationCommandTarget
    {
    public:
        
        jPatcher(jInstance& instance, model::Patcher& patcher, model::Patcher::View& view);
        ~jPatcher();
        
        // ================================================================================ //
        //                                    jPATCHER                                      //
        // ================================================================================ //
        
        using jObjects = std::set<std::unique_ptr<jObject>>;
        using jLinks = std::set<std::unique_ptr<jLink>>;
        
        //! @internal flip::DocumentObserver<model::Patcher>::document_changed
        void patcherChanged(model::Patcher& patcher, model::Patcher::View& view);
        
        //! @brief Returns the jObjects.
        jObjects const& getObjects() const;
        
        //! @brief Returns the jLinks.
        jLinks const& getLinks() const;
        
        //! @brief Returns the jObject corresponding to a given Object model.
        jObject* getObject(model::Object const& object) const;
        
        //! @brief Returns the jLink corresponding to a given Link model.
        jLink* getLink(model::Link const& link) const;
        
        //! @brief Set the lock status of the patcher view.
        void setLock(bool locked);
        
        //! @brief Get the lock status of the patcher view.
        bool isLocked();
        
        // ================================================================================ //
        //                                    COMPONENT                                     //
        // ================================================================================ //
        
        void paint(juce::Graphics& g) override;
        void mouseDown(juce::MouseEvent const& event) override;
        void mouseDrag(MouseEvent const& e) override;
        void mouseUp(MouseEvent const& e) override;
        void mouseMove(juce::MouseEvent const& event) override;
        bool keyPressed(const KeyPress& key) override;
        
        // ================================================================================ //
        //                              APPLICATION COMMAND TARGET                          //
        // ================================================================================ //
        
        ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands(Array<CommandID>& commands) override;
        void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
        bool perform(const InvocationInfo& info) override;
        
    private: // methods
        
        jObjects::iterator findjObject(model::Object const& object) const;
        jLinks::iterator findjLink(model::Link const& link) const;
        
        //! @brief Load object and links.
        void loadPatcher();
        
        //! @internal handle right click
        void showPatcherPopupMenu(juce::Point<int> const& position);
        
        // ================================================================================ //
        //                                  MODEL OBSERVER                                  //
        // ================================================================================ //
        
        //! @brief Notify jPatcher that selection has changed.
        void selectionChanged();
        
        //! @brief Check patcher view information changes (lock_status...).
        void checkViewInfos(model::Patcher::View& view);
        
        //! @brief Check the patcher selection changes.
        void checkSelectionChanges(model::Patcher& patcher);
        
        //! @internal Object model has just been added to the document.
        void addjObject(model::Object& object);
        
        //! @internal Object model is resident and internal value changed.
        void objectChanged(model::Patcher::View& view, model::Object& object);
        
        //! @internal Object model will be removed from the document.
        void removejObject(model::Object& object);
        
        //! @internal Link model has just been added to the document.
        void addjLink(model::Link& link);
        
        //! @internal Link model is resident and internal value changed.
        void linkChanged(model::Link& link);
        
        //! @internal Link model will be removed from the document.
        void removejLink(model::Link& link);
        
        //! @brief Add a new Object to the model at a given position.
        void createObjectModel(std::string const& text, double pos_x, double pos_y);
        
        //! @brief Bring all link components in front of object ones.
        void bringsLinksToFront();
        
        //! @brief Bring all object components in front of link ones.
        void bringsObjectsToFront();
        
        // ================================================================================ //
        //                                     UNDO/REDO                                    //
        // ================================================================================ //
        
        //! @internal Try to undo last action.
        void undo();
        
        //! @internal Returns true if there is an action to undo.
        bool canUndo();
        
        //! @internal Returns the undo label if there is an action to undo or an empty string.
        std::string getUndoLabel();
        
        //! @internal Try to redo next action.
        void redo();
        
        //! @internal Returns true if there is an action to undo.
        bool canRedo();
        
        //! @internal Returns the redo label if there is an action to redo or an empty string.
        std::string getRedoLabel();
        
        // ================================================================================ //
        //                                     SELECTION                                    //
        // ================================================================================ //
        
        void addToSelectionBasedOnModifiers(jObject& object, bool select_only);
        
        void addToSelectionBasedOnModifiers(jLink& link, bool select_only);
        
        bool selectOnMouseDown(jObject& object, bool select_only);
        
        bool selectOnMouseDown(jLink& link, bool select_only);
        
        void selectOnMouseUp(jObject& box, bool select_only,
                             const bool box_was_dragged,
                             const bool result_of_mouse_down_select_method);
        
        void selectOnMouseUp(jLink& link, bool select_only,
                             const bool box_was_dragged,
                             const bool result_of_mouse_down_select_method);
        
        //! @brief Returns true if any object or link is selected
        bool isAnythingSelected();
        
        //! @brief Returns true if there is at least one object selected
        bool isAnyObjectSelected();
        
        //! @brief Returns true if there is at least one link selected
        bool isAnyLinksSelected();
        
        //! @brief Select an Object.
        void selectObject(jObject& object);
        
        //! @brief Unselect all and select an object.
        void selectObjectOnly(jObject& object);
        
        //! @brief Select a Link.
        void selectLink(jLink& link);
        
        //! @brief Unselect all and select a link.
        void selectLinkOnly(jLink& link);
        
        //! @brief Unselect an Object.
        void unselectObject(jObject& object);
        
        //! @brief Unselect a Link.
        void unselectLink(jLink& link);
        
        //! @brief Add all objects to the patcher selection.
        void selectAllObjects();
        
        //! @brief Unselect all objects.
        void unselectAll();
        
        //! @brief Unselect all objects.
        void deleteSelection();
        
        //! @brief Begins a move or resize gesture.
        void startMoveOrResizeObjects();
        
        //! @brief Ends a move or resize gesture.
        void endMoveOrResizeObjects();
        
        //! @brief Move selected objects by a given amount of pixels.
        //! @param delta The given amount of pixel.
        //! @param commit Pass false if you don't want to commit.
        //! @param gesture Pass true to commit a gesture (commit must be true).
        void moveSelectedObjects(juce::Point<int> const& delta,
                                 bool commit = true, bool gesture = false);

    private: // members
        
        jInstance&                                  m_instance;
        model::Patcher&                             m_patcher_model;
        model::Patcher::View&                       m_view_model;
        
        jObjects                                    m_objects;
        jLinks                                      m_links;
        
        std::set<model::Object*>                    m_local_objects_selection;
        std::set<model::Link*>                      m_local_links_selection;
        
        std::map<model::Object*, std::set<uint64_t>> m_distant_objects_selection;
        std::map<model::Link*, std::set<uint64_t>>   m_distant_links_selection;
        
        std::unique_ptr<HitTester> m_hittester;
        
        bool m_is_locked;
        
        // mouse interactions flags
        juce::Point<int> m_last_drag;
        bool m_object_received_down_event = false;
        bool m_copy_on_drag = false;
        bool m_is_dragging = false;
        bool m_is_dragging_links = false;
        bool m_mouse_has_just_been_clicked = false;
        bool m_select_on_mouse_down_status = false;
        bool m_link_downstatus = false;
        long m_last_border_downstatus;
        bool m_is_in_move_or_resize_gesture = false;
        
        // here to initialise jPatcher commands only one time.
        static bool m_command_manager_binded;
    };
}

#endif // KIWI_JPATCHER_HPP_INCLUDED
