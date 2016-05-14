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
        bool getLockStatus();
        
        // ================================================================================ //
        //                                    COMPONENT                                     //
        // ================================================================================ //
        
        void paint(juce::Graphics& g) override;
        void mouseDown(juce::MouseEvent const& event) override;
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
        
        //! @internal handle left click
        void leftClick(juce::MouseEvent const& event);
        
        //! @internal handle right click
        void rightClick(juce::MouseEvent const& event);
        
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
        
        //! @brief Returns true if any object or link is selected
        bool isAnythingSelected();
        
        //! @brief Returns true if there is at least one object selected
        bool isAnyObjectSelected();
        
        //! @brief Returns true if there is at least one link selected
        bool isAnyLinksSelected();
        
        //! @brief Add all objects to the patcher selection.
        void selectAllObjects();
        
        //! @brief Unselect all objects.
        void unselectAll();
        
        //! @brief Unselect all objects.
        void deleteSelection();
        
        //! @brief Move selected boxes by a given ammount of pixels.
        void moveSelectedObjects(juce::Point<int> const& delta);

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
        
        bool                                        m_is_locked;
        
        static bool m_command_manager_binded;
    };
}

#endif // KIWI_JPATCHER_HPP_INCLUDED
