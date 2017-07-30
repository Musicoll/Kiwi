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
    public juce::TextEditor::Listener,
    public juce::Timer
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
        
        //! @internal Called every 200ms by a juce::Timer.
        //! @details This method will close the menu if its relative position changed
        //! or if editor lost the keyboard focus.
        void timerCallback() override;
        
        //! @brief juce::TextEditor::Listener
        void textEditorTextChanged(juce::TextEditor& ed) override;
        
        //! @brief Called when a menu item has been clicked.
        void menuItemSelected(juce::String const& text);
        
        //! @brief Called when a menu item has been double-clicked.
        void menuItemValidated(juce::String const& text);
        
    private: // classes
        
        class Menu;
    
    private: // members
        
        SuggestList                     m_suggest_list;
        std::vector<juce::String>       m_splited_text;
        std::unique_ptr<Menu>           m_menu = nullptr;
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
        
        //! @brief Set the action to execute when an item has been double-clicked.
        void setItemValidatedAction(action_method_t function);
        
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
        
        //! @brief Called to validate the current selected row.
        void validateSelectedRow();
        
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
        juce::ComponentBoundsConstrainer    m_constrainer;
        juce::ResizableCornerComponent      m_resizable_corner;
        
        action_method_t                     m_validated_action;
        action_method_t                     m_selected_action;
    };
    
    //! @brief Returns text splited using a specified divider.
    std::vector<juce::String> split(juce::String const& text, juce::String divider);
}
