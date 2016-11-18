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
        void setFirstItemFocused()
        {
            toFront(true);
            m_suggest_list_box.selectRow(0);
        }
        
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
    
    //! @brief A text editor component that shows a pop-up menu/combo box below it.
    class SuggestPopupEditor
    : public juce::Component,
    public juce::TextEditor::Listener,
    public juce::KeyListener,
    public juce::Timer
    {
    public: // methods
        
        //! @brief Constructor.
        SuggestPopupEditor()
        {
            addAndMakeVisible(m_editor);
            
            m_editor.addListener(this);
            m_editor.addKeyListener(this);
        }
        
        void mouseDown(const juce::MouseEvent& event) override
        {
            if (!isParentOf(event.eventComponent))
                dismissMenu();
        }
        
        ~SuggestPopupEditor()
        {
            juce::Desktop::getInstance().removeGlobalMouseListener(static_cast<juce::MouseListener*>(this));
        }
        
        void showMenu()
        {
            m_popup.reset(new SuggestPopup());
            m_popup->addToDesktop(juce::ComponentPeer::StyleFlags::windowIsTemporary);
            m_popup->setVisible(true);
            m_popup->setBounds(getScreenBounds().translated(0, getHeight()).withHeight(100));
            
            m_popup->setSelectedItemAction([this](juce::String text)
                                          {
                                              m_editor.setText(text, juce::dontSendNotification);
                                              m_popup.reset();
                                          });
            
            m_popup->addKeyListener(this);
            
            juce::Desktop::getInstance().addGlobalMouseListener(this);
            
            startTimer(200);
        }
        
        void textEditorTextChanged(juce::TextEditor&) override
        {
            const auto text = m_editor.getText().toStdString();
            m_popup->applyFilter(text);
            
            if(m_popup->useSuggestList().empty())
            {
                dismissMenu();
            }
            else
            {
                if(!m_popup)
                    showMenu();
            }
        }
        
        void dismissMenu()
        {
            m_popup.reset();
            stopTimer();
            juce::Desktop::getInstance().removeGlobalMouseListener(this);
        }
        
        void timerCallback() override
        {
            if (!juce::Process::isForegroundProcess())
                dismissMenu();
            
            if(m_popup)
            {
                if (!hasKeyboardFocus(true) && !m_popup->hasKeyboardFocus(true))
                    dismissMenu();
            }
            else
            {
                if (!hasKeyboardFocus(true))
                    dismissMenu();
            }
        }
        
        bool keyPressed(juce::KeyPress const& key, Component* component) override
        {
            if(component == &m_editor)
            {
                if(key == juce::KeyPress::downKey)
                {
                    if(m_popup)
                        m_popup->setFirstItemFocused();
                    
                    return true;
                }
            }
            else if(component == m_popup.get())
            {
                // if the user tries to type into the menu lets move the focus back there and inject the keypress
                m_editor.toFront(true);
                return m_editor.keyPressed(key);
            }
            
            return false;
        }
        
        void focusLost(FocusChangeType cause) override
        {
            if (m_popup && !m_popup->hasKeyboardFocus(true))
                m_popup.reset();
        }
        
        void resized() override
        {
            m_editor.setBounds(getLocalBounds());
        }
        
    private: // members
        
        std::unique_ptr<SuggestPopup>   m_popup;
        juce::TextEditor                m_editor;
    };
}

#endif // KIWI_APP_SUGGESTPOPUP_HPP_INCLUDED
