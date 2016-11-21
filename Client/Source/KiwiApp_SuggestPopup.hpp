/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_APP_SUGGESTPOPUP_HPP_INCLUDED
#define KIWI_APP_SUGGESTPOPUP_HPP_INCLUDED

#include <KiwiModel/KiwiModel_Factory.hpp>
#include <KiwiEngine/KiwiEngine_Listeners.hpp>

#include <juce_gui_basics/juce_gui_basics.h>

#include "KiwiApp_Utils/KiwiApp_SuggestList.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                   SUGGEST POPUP                                  //
    // ================================================================================ //
    
    //! @brief Suggestion list Popup Component.
    class SuggestPopup : public juce::Component, public juce::ListBoxModel
    {
    public: // methods
        
        using action_method_t = std::function<void(juce::String)>;
        
        //! @brief Constructor.
        SuggestPopup(SuggestList::entries_t entries = SuggestList::entries_t());
        
        //! @brief Destructor.
        ~SuggestPopup();
        
        //! @brief Populate the list with some entries.
        void populate(SuggestList::entries_t entries);
        
        //! @brief Apply a filter to the given pattern.
        void applyFilter(std::string const& filter_pattern);
        
        //! @brief Return the SuggestList.
        SuggestList const& useSuggestList() const;
        
        //! @brief Show the popup.
        void show(Component* target_comp);
        
        //! @brief Close the popup.
        void close();
        
        //! @brief Set the action to execute when an item has been clicked.
        void setItemClickedAction(action_method_t function);
        
        //! @brief Set the action to execute when an item has been double-clicked.
        void setItemDoubleClickedAction(action_method_t function);
        
        //! @brief Set the action to execute when an item has been selected.
        void setSelectedItemAction(action_method_t function);
        
        //! @brief Select the first item of the list
        void setFirstItemFocused();
        
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
        
        //! @brief User clicked on a part of the list where there are no rows.
        void backgroundClicked(juce::MouseEvent const& e) override;
        
        //! @brief Called when rows are selected or deselected.
        void selectedRowsChanged(int last_row_selected) override;
        
        //! @brief Called when the delete key is pressed.
        void deleteKeyPressed(int last_row_selected) override;
        
        //! @brief Called when the return key is pressed.
        void returnKeyPressed(int last_row_selected) override;
        
    private: // members
        
        SuggestList     m_suggest_list;
        juce::ListBox   m_suggest_list_box;
        action_method_t m_clicked_action;
        action_method_t m_double_clicked_action;
        action_method_t m_selected_action;
    };
    
    // ================================================================================ //
    //                              SUGGEST POPUP EDITOR                                //
    // ================================================================================ //
    
    //! @brief A text editor component that shows a pop-up menu/combo box below it.
    class SuggestPopupEditor
    : public juce::Component,
    public juce::TextEditor::Listener,
    public juce::KeyListener,
    public juce::Timer
    {
    public: // classes
        
        class Listener;
        
    public: // methods
        
        //! @brief Constructor.
        SuggestPopupEditor();
        
        //! @brief Destructor.
        ~SuggestPopupEditor();
        
        //! @brief Returns the TextEditor.
        juce::TextEditor& useEditor();
        
        //! @brief Adds a TextEditor' listener.
        void addListener(SuggestPopupEditor::Listener& listener);
        
        //! @brief Removes a TextEditor' listener.
        void removeListener(SuggestPopupEditor::Listener& listener);
        
        //! @brief juce::Component.
        void mouseDown(juce::MouseEvent const& event) override;
        
        //! @brief juce::Component.
        void resized() override;
        
        //! @brief juce::Component.
        void focusLost(FocusChangeType cause) override;
        
        //! @brief Shows the menu.
        void showMenu();
        
        //! @brief juce::Timer.
        void dismissMenu();
        
        //! @brief juce::TextEditor::Listener
        void textEditorTextChanged(juce::TextEditor&) override;
        
        //! @brief juce::KeyListener.
        bool keyPressed(juce::KeyPress const& key, Component* component) override;
        
        //! @brief juce::Timer.
        void timerCallback() override;

    private: // members
        
        std::unique_ptr<SuggestPopup>   m_popup = nullptr;
        juce::TextEditor                m_editor;
        juce::String                    m_typed_text;
        engine::Listeners<Listener>     m_listeners;
    };
    
    // ================================================================================ //
    //                          SUGGEST POPUP EDITOR LISTENER                           //
    // ================================================================================ //
    
    //! @brief Receives callbacks from a SuggestPopupEditor component.
    //! @see SuggestPopupEditor::addListener
    class SuggestPopupEditor::Listener
    {
    public: // methods
        
        //! @brief Destructor.
        virtual ~Listener() {}
        
        //! @brief Called when the user changes the text in some way.
        virtual void textEditorTextChanged(SuggestPopupEditor& ed) {}
        
        //! @brief Called when the user presses the return key.
        virtual void textEditorReturnKeyPressed(SuggestPopupEditor& ed) {}
        
        //! @brief Called when the user presses the escape key.
        virtual void textEditorEscapeKeyPressed(SuggestPopupEditor& ed) {}
        
        //! @brief Called when the text editor loses focus.
        virtual void textEditorFocusLost(SuggestPopupEditor& ed) {}
    };
}

#endif // KIWI_APP_SUGGESTPOPUP_HPP_INCLUDED
