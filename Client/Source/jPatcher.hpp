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
        
        // juce::Component
        void paint(juce::Graphics& g) override;
        void mouseDown(juce::MouseEvent const& event) override;
        bool keyPressed(const KeyPress& key) override;
        
        //! @internal flip::DocumentObserver<model::Patcher>::document_changed
        void patcherChanged(model::Patcher& patcher, model::Patcher::View& view);
        
        //! @brief Returns the jObject corresponding to a given Object model.
        jObject* getjObject(model::Object const& object) const;
        
        //! @brief Returns the jLink corresponding to a given Link model.
        jLink* getjLink(model::Link const& link) const;
        
        // ================================================================================ //
        //                              APPLICATION COMMAND TARGET                          //
        // ================================================================================ //
        
        ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands(Array<CommandID>& commands) override;
        void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
        bool perform(const InvocationInfo& info) override;
        
    private: // methods
        
        std::set<std::unique_ptr<jObject>>::iterator findjObject(model::Object const& object) const;
        std::set<std::unique_ptr<jLink>>::iterator findjLink(model::Link const& link) const;
        
        //! @brief Load object and links.
        void loadPatcher();
        
        //! @internal handle left click
        void leftClick(juce::MouseEvent const& event);
        
        //! @internal handle right click
        void rightClick(juce::MouseEvent const& event);
        
        void selectionChanged();
        
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
        
        std::set<std::unique_ptr<jObject>>          m_objects;
        std::set<std::unique_ptr<jLink>>            m_links;
        
        std::set<model::Object*>                    m_local_objects_selection;
        std::set<model::Link*>                      m_local_links_selection;
        
        std::map<model::Object*, std::set<uint64_t>> m_distant_objects_selection;
        std::map<model::Link*, std::set<uint64_t>>   m_distant_links_selection;
        
        static bool m_command_manager_binded;
    };
}

#endif // KIWI_JPATCHER_HPP_INCLUDED
