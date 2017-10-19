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

#pragma once

#include <KiwiEngine/KiwiEngine_Listeners.h>

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include <KiwiApp_Utils/KiwiApp_SuggestList.h>

namespace kiwi
{
    // ================================================================================ //
    //                                  SUGGEST EDITOR                                  //
    // ================================================================================ //
    
    //! @brief A text editor with auto-completion.
    //! @details This component shows a dropdown menu list below it.
    class SuggestEditor
    : public juce::TextEditor,
    public juce::TextEditor::Listener
    {
    public: // methods
        
        //! @brief Constructor.
        //! @param entries Populate suggestion list with these entries.
        SuggestEditor(SuggestList::entries_t entries);
        
        //! @brief Destructor.
        ~SuggestEditor();
        
    private: // methods
        
        //! @brief Shows the menu.
        void showMenu();
        
        //! @brief Returns true if the menu is currently opened.
        bool isMenuOpened() const noexcept;
        
        //! @brief Close the menu.
        void closeMenu();
        
        //! @brief juce::TextEditor.
        bool keyPressed(juce::KeyPress const& key) override;
        
        //! @brief Called when a key status changed.
        bool keyStateChanged(bool isKeyDown) override;
        
        //! @brief juce::TextEditor::Listener
        void textEditorTextChanged(juce::TextEditor& ed) override;
        
        //! @brief juce::TextEditor::Listener.
        //! @details Needs to hide menu when focus is lost on editor.
        void textEditorFocusLost(juce::TextEditor & editor) override;
        
        //! @brief Called when a menu item has been clicked.
        void menuItemSelected(juce::String const& text);
        
        //! @brief Called whenever a selected item is validated.
        void menuItemValidated(juce::String const& text);
        
        //! @brief Called whenever the selected item is unselected.
        //! @details Replace suggestion with what was fomerly typed.
        void menuItemUnselected();
        
        //! @brief Causes the sugesst list to stop updating.
        void disableUpdate();
        
        //! @brief Causes the suggest list to start updating.
        void enableUpdate();
        
    private: // classes
        
        class Menu;
    
    private: // members
        
        SuggestList                     m_suggest_list;
        juce::StringArray               m_split_text;
        std::unique_ptr<Menu>           m_menu = nullptr;
        bool                            m_update_enabled;
    };
    
    // ================================================================================ //
    //                                       MENU                                       //
    // ================================================================================ //
    
    //! @brief Suggestion menu.
    class SuggestEditor::Menu : public juce::Component, public juce::ListBoxModel
    {
    public: // methods
        
        using action_method_t = std::function<void(juce::String)>;
        
        //! @brief Constructor.
        Menu(SuggestList& list, SuggestEditor & creator);
        
        //! @brief Destructor.
        ~Menu();
        
        //! @brief Set the action to execute when an item has been double-clicked.
        void setItemValidatedAction(action_method_t function);
        
        //! @brief Set the action to execute when an item has been selected.
        void setSelectedItemAction(action_method_t function);
        
        //! @brief Set the action to execute when item are unselected.
        void setUnselectedItemAction(std::function<void(void)> function);
        
        //! @brief Unselect the currently selected row
        void unselectRow();
        
        //! @brief Select an item of the list.
        void selectRow(int idx);
        
        //! @brief Select the first item of the list.
        void selectFirstRow();
        
        //! @brief Select the previous item of the list.
        void selectPreviousRow();
        
        //! @brief Select the next item of the list.
        void selectNextRow();
        
        //! @brief Called to validate the current selected row.
        void validateSelectedRow();
        
        //! @brief Update the list.
        void update();
        
        // juce::Component
        void paint(juce::Graphics& g) override;
        
        // juce::Component
        void resized() override;
        
        //! @brief Returns the curretly selected row
        int getSelectedRow() const;
        
    private: // methods
        
        //! @brief Returns true if suggest editor creator is target component.
        //! @details Called only for keypressed events not for mouse events.
        bool canModalEventBeSentToComponent(juce::Component const* target_component) override final;
        
        //! @brief Exists modal state if click happens.
        //! @details If the creator is clicked it sets the caret visible enabling edition
        //! otherwise it exits modal state. Called only on mouse clicks events.
        void inputAttemptWhenModal() override final;
        
        // ================================================================================ //
        //                               SUGGEST LISTBOX MODEL                              //
        // ================================================================================ //
        
        //! @brief Returns the number of items in the list.
        int getNumRows() override;
        
        //! @brief Draw a row of the list.
        void paintListBoxItem(int row_number, juce::Graphics& g,
                              int width, int height, bool selected) override;
        
        //! @brief User clicked on a row.
        void listBoxItemClicked(int row, juce::MouseEvent const& e) override;
        
        //! @brief User double-clicked on a row.
        void listBoxItemDoubleClicked(int row, juce::MouseEvent const& e) override;
        
        //! @brief Called when rows are selected or deselected.
        void selectedRowsChanged(int last_row_selected) override;
        
    private: // members
        
        SuggestList&                        m_suggest_list;
        juce::ListBox                       m_suggest_list_box;
        SuggestEditor &                     m_creator;
        juce::ComponentBoundsConstrainer    m_constrainer;
        juce::ResizableCornerComponent      m_resizable_corner;
        
        action_method_t                     m_validated_action;
        action_method_t                     m_selected_action;
        std::function<void(void)>           m_unselected_action;
    };
}
