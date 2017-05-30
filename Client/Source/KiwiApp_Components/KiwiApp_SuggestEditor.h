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

#include <juce_gui_basics/juce_gui_basics.h>

#include "../KiwiApp_Utils/KiwiApp_SuggestList.h"

namespace kiwi
{
    // ================================================================================ //
    //                                  SUGGEST EDITOR                                  //
    // ================================================================================ //
    
    //! @brief A text editor with auto-completion.
    //! @details This component shows a dropdown menu list below it.
    class SuggestEditor
    : public juce::TextEditor,
    public juce::TextEditor::Listener,
    public juce::Timer
    {
    public: // classes
        
        struct Listener;
        
    public: // methods
        
        //! @brief Constructor.
        //! @param entries Populate suggestion list with these entries.
        SuggestEditor(SuggestList::entries_t entries);
        
        //! @brief Destructor.
        ~SuggestEditor();
        
        //! @brief Adds a SuggestEditor::Listener.
        void addListener(SuggestEditor::Listener& listener);
        
        //! @brief Removes a SuggestEditor::Listener.
        void removeListener(SuggestEditor::Listener& listener);
        
        //! @brief Shows the menu.
        void showMenu();
        
        //! @brief Returns true if the menu is currently opened.
        bool isMenuOpened() const noexcept;
        
        //! @brief Close the menu.
        void closeMenu();
        
        //! @brief juce::TextEditor::Listener
        void textEditorTextChanged(juce::TextEditor& ed) override;
        
        //! @brief Called when the user presses the return key.
        void returnPressed() override;
        
        //! @brief Called when the user presses the escape key.
        void escapePressed() override;
        
        //! @brief Called when the text editor loses focus.
        void focusLost(juce::Component::FocusChangeType focus_change) override;
        
        //! @brief juce::TextEditor.
        bool keyPressed(juce::KeyPress const& key) override;
        
    private: // methods
        
        //! @internal Called every 200ms by a juce::Timer.
        //! @details This method will close the menu if its relative position changed
        //! or if editor lost the keyboard focus.
        void timerCallback() override;
        
        //! @brief Called when the menu selection has changed.
        void menuSelectionChanged(juce::String const& text);
        
        //! @brief Called when a menu item has been clicked.
        void menuItemClicked(juce::String const& text);
        
        //! @brief Called when a menu item has been double-clicked.
        void menuItemDoubleClicked(juce::String const& text);
        
    private: // classes
        
        class Menu;
    
    private: // members
        
        SuggestList                     m_suggest_list;
        std::unique_ptr<Menu>           m_menu = nullptr;
        juce::String                    m_typed_text;
        engine::Listeners<Listener>     m_listeners;
        
        bool                            m_need_complete = true;
    };
    
    // ================================================================================ //
    //                              SUGGEST EDITOR LISTENER                             //
    // ================================================================================ //
    
    //! @brief Receives callbacks from a SuggestEditor component.
    //! @see SuggestEditor::addListener
    struct SuggestEditor::Listener
    {
        //! @brief Destructor.
        virtual ~Listener() {}
        
        //! @brief Called when the user changes the text in some way.
        virtual void suggestEditorTextChanged(SuggestEditor& editor) {}
        
        //! @brief Called when the user presses the return key.
        virtual void suggestEditorReturnKeyPressed(SuggestEditor& editor) {}
        
        //! @brief Called when the user presses the escape key.
        virtual void suggestEditorEscapeKeyPressed(SuggestEditor& editor) {}
        
        //! @brief Called when the text editor loses focus.
        virtual void suggestEditorFocusLost(SuggestEditor& editor) {}
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
        Menu(SuggestList& list);
        
        //! @brief Destructor.
        ~Menu();
        
        //! @brief Set the action to execute when an item has been clicked.
        void setItemClickedAction(action_method_t function);
        
        //! @brief Set the action to execute when an item has been double-clicked.
        void setItemDoubleClickedAction(action_method_t function);
        
        //! @brief Set the action to execute when an item has been selected.
        void setSelectedItemAction(action_method_t function);
        
        //! @brief Select an item of the list.
        void selectRow(int idx);
        
        //! @brief Select the first item of the list.
        void selectFirstRow();
        
        //! @brief Select the previous item of the list.
        void selectPreviousRow();
        
        //! @brief Select the next item of the list.
        void selectNextRow();
        
        //! @brief Update the list.
        void update();
        
        // juce::Component
        void paint(juce::Graphics& g) override;
        
        // juce::Component
        void resized() override;
        
    private: // methods
        
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
        juce::ResizableCornerComponent      m_resizable_corner;
        juce::ComponentBoundsConstrainer    m_constrainer;
        
        action_method_t                     m_clicked_action;
        action_method_t                     m_double_clicked_action;
        action_method_t                     m_selected_action;
    };
}
