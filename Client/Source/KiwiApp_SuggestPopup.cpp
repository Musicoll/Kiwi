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

#include "KiwiApp_SuggestPopup.hpp"
#include "KiwiApp.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                   SUGGEST POPUP                                  //
    // ================================================================================ //

    SuggestPopup::SuggestPopup(SuggestList& list) : m_suggest_list(list)
    {
        setAlwaysOnTop(true);
        setOpaque(true);
        setSize(200, 150);
        
        m_suggest_list_box.setModel(this);
        m_suggest_list_box.setSize(getWidth(), getHeight());
        m_suggest_list_box.setRowHeight(18);
        m_suggest_list_box.setMultipleSelectionEnabled(false);
        m_suggest_list_box.setColour(juce::ListBox::ColourIds::backgroundColourId,
                                     juce::Colours::transparentBlack);
        
        addAndMakeVisible(m_suggest_list_box);
    }
    
    SuggestPopup::~SuggestPopup()
    {
        ;
    }
    
    void SuggestPopup::populate(SuggestList::entries_t entries)
    {
        m_suggest_list.clear();
        m_suggest_list.addEntries(std::move(entries));
        m_suggest_list_box.updateContent();
    }
    
    void SuggestPopup::applyFilter(std::string const& filter_pattern)
    {
        m_suggest_list.applyFilter(filter_pattern);
        m_suggest_list_box.updateContent();
    }
    
    void SuggestPopup::paint(juce::Graphics & g)
    {
        g.fillAll(juce::Colours::white);
    }
    
    void SuggestPopup::resized()
    {
        m_suggest_list_box.setBounds(getLocalBounds());
    }
    
    void SuggestPopup::setFirstItemFocused()
    {
        toFront(true);
        m_suggest_list_box.selectRow(0);
        return;
        int rows = getNumRows();
        auto new_selected_row = m_suggest_list_box.getSelectedRow() + 1;
        if(new_selected_row >= rows)
        {
            m_suggest_list_box.selectRow(0);
        }
        else
        {
            m_suggest_list_box.selectRow(new_selected_row);
        }
    }
    
    void SuggestPopup::setItemClickedAction(action_method_t function)
    {
        m_clicked_action = function;
    }
    
    void SuggestPopup::setItemDoubleClickedAction(action_method_t function)
    {
        m_double_clicked_action = function;
    }
    
    void SuggestPopup::setSelectedItemAction(action_method_t function)
    {
        m_selected_action = function;
    }
    
    void SuggestPopup::setDeleteKeyPressedAction(action_method_t function)
    {
        m_deletekey_pressed_action = function;
    }
    
    // ================================================================================ //
    //                               SUGGEST LISTBOX MODEL                              //
    // ================================================================================ //
    
    int SuggestPopup::getNumRows()
    {
        return m_suggest_list.size();
    }
    
    void SuggestPopup::paintListBoxItem(int row_number, juce::Graphics& g,
                                                      int width, int height, bool selected)
    {
        const bool is_odd = (row_number % 2) == 0;
        g.fillAll(selected ? juce::Colours::lightgrey : juce::Colours::white.darker(is_odd ? 0 : 0.05));
        if(row_number < m_suggest_list.size())
        {
            g.setFont(12);
            std::string const& str = *(m_suggest_list.begin() + row_number);
            g.drawSingleLineText(str, 10, height - g.getCurrentFont().getHeight()*0.5, juce::Justification::left);
        }
    }
    
    void SuggestPopup::listBoxItemClicked(int row, juce::MouseEvent const& e)
    {
        if(m_clicked_action && (row < m_suggest_list.size()))
        {
            std::string const& str = *(m_suggest_list.begin() + row);
            m_clicked_action(str);
        }
    }
    
    void SuggestPopup::listBoxItemDoubleClicked(int row, juce::MouseEvent const& e)
    {
        if(m_double_clicked_action && (row < m_suggest_list.size()))
        {
            std::string const& str = *(m_suggest_list.begin() + row);
            m_double_clicked_action(str);
        }
    }
    
    void SuggestPopup::backgroundClicked(juce::MouseEvent const& e)
    {
        ;
    }
    
    void SuggestPopup::selectedRowsChanged(int last_row_selected)
    {
        if(m_selected_action && (last_row_selected < m_suggest_list.size()))
        {
            std::string const& str = *(m_suggest_list.begin() + last_row_selected);
            m_selected_action(str);
        }
    }
    
    void SuggestPopup::deleteKeyPressed(int last_row_selected)
    {
        //std::cout << "deleteKeyPressed" << std::endl;
        if(m_deletekey_pressed_action && (last_row_selected < m_suggest_list.size()))
        {
            std::string const& str = *(m_suggest_list.begin() + last_row_selected);
            m_deletekey_pressed_action(str);
        }
    }
    
    void SuggestPopup::returnKeyPressed(int last_row_selected)
    {
        if(m_double_clicked_action && (last_row_selected < m_suggest_list.size()))
        {
            std::string const& str = *(m_suggest_list.begin() + last_row_selected);
            m_double_clicked_action(str);
        }
    }
    
    // ================================================================================ //
    //                              SUGGEST POPUP EDITOR                                //
    // ================================================================================ //
    
    SuggestPopupEditor::SuggestPopupEditor() : m_suggest_list(model::Factory::getNames())
    {
        setWantsKeyboardFocus(true);
        addAndMakeVisible(m_editor);
        
        m_editor.addListener(this);
        m_editor.addKeyListener(this);
    }
    
    SuggestPopupEditor::~SuggestPopupEditor()
    {
        juce::Desktop::getInstance().removeGlobalMouseListener(static_cast<juce::MouseListener*>(this));
    }
    
    juce::TextEditor& SuggestPopupEditor::useEditor()
    {
        return m_editor;
    }
    
    void SuggestPopupEditor::addListener(SuggestPopupEditor::Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void SuggestPopupEditor::removeListener(SuggestPopupEditor::Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    void SuggestPopupEditor::showMenu()
    {
        m_popup.reset(new SuggestPopup(m_suggest_list));
        
        const auto on_select_change_fn = [this](juce::String text)
        {
            juce::String old_text = m_editor.getText();
            m_editor.setText(text, juce::dontSendNotification);
            
            //m_editor.setHighlightedRegion({old_text.length(), text.length()});
            m_editor.grabKeyboardFocus();
            m_editor.setHighlightedRegion({0, static_cast<int>(m_editor.getText().length())});
        };
        
        const auto on_item_clicked_fn = [this](juce::String text)
        {
            juce::String old_text = m_editor.getText();
            m_editor.setText(text, juce::dontSendNotification);
            m_editor.setHighlightedRegion({old_text.length(), text.length()});
        };
        
        m_popup->setSelectedItemAction(on_select_change_fn);
        m_popup->setItemClickedAction(on_item_clicked_fn);
        m_popup->setItemDoubleClickedAction([this](juce::String text)
                                            {
                                                m_editor.setText(text, juce::dontSendNotification);
                                                dismissMenu();
                                                m_editor.grabKeyboardFocus();
                                                m_listeners.call(&Listener::textEditorReturnKeyPressed, *this);
                                            });
        
        m_popup->setDeleteKeyPressedAction([this](juce::String text)
                                           {
                                               m_editor.toFront(true);
                                               m_editor.deleteBackwards(false);
                                           });
        
        m_popup->addToDesktop(juce::ComponentPeer::StyleFlags::windowIsTemporary
                              | juce::ComponentPeer::windowHasDropShadow);

        m_popup->setBounds(getScreenBounds().translated(-2, getHeight() + 2).withSize(200, 150));
        m_popup->setVisible(true);
        
        m_popup->addKeyListener(this);
        
        juce::Desktop::getInstance().addGlobalMouseListener(this);
        
        m_popup->setFirstItemFocused();
        m_editor.grabKeyboardFocus();
        m_editor.setHighlightedRegion({0, static_cast<int>(m_editor.getText().length())});
        
        startTimer(200);
    }
    
    void SuggestPopupEditor::updateMenu()
    {
        assert(m_popup && "Call showMenu() before");
        assert(m_popup->isOnDesktop());
        
        m_popup->setVisible(true);
        m_popup->setBounds(getScreenBounds().translated(-2, getHeight() + 2).withSize(200, 150));
        
        m_popup->repaint();

        //m_popup->setFirstItemFocused();
        m_editor.grabKeyboardFocus();
    }
    
    void SuggestPopupEditor::mouseDown(juce::MouseEvent const& event)
    {
        if (!isParentOf(event.eventComponent))
        {
            dismissMenu();
        }
    }
    
    void SuggestPopupEditor::textEditorTextChanged(juce::TextEditor&)
    {
        const auto text = m_editor.getText().toStdString();
        m_suggest_list.applyFilter(text);
        
        if(m_popup && m_suggest_list.empty())
        {
            dismissMenu();
        }
        else if(!m_suggest_list.empty())
        {
            if(m_popup)
                updateMenu();
            else
                showMenu();
        }
        
        m_listeners.call(&Listener::textEditorTextChanged, *this);
    }
    
    void SuggestPopupEditor::textEditorReturnKeyPressed(juce::TextEditor& ed)
    {
        m_listeners.call(&Listener::textEditorReturnKeyPressed, *this);
    }
    
    void SuggestPopupEditor::textEditorEscapeKeyPressed(juce::TextEditor& ed)
    {
        m_listeners.call(&Listener::textEditorEscapeKeyPressed, *this);
    }
    
    void SuggestPopupEditor::textEditorFocusLost(juce::TextEditor& ed)
    {
        std::cout << "SuggestPopupEditor::textEditorFocusLost" << '\n';
        
        if (m_popup && !m_popup->hasKeyboardFocus(true))
        {
            std::cout << "m_popup && !m_popup->hasKeyboardFocus(true)" << '\n';
            
            //m_popup.reset();
            //m_listeners.call(&Listener::textEditorFocusLost, *this);
        }
        
        //m_listeners.call(&Listener::textEditorFocusLost, *this);
    }
    
    void SuggestPopupEditor::dismissMenu()
    {
        if(m_popup)
        {
            m_popup.reset();
            stopTimer();
            juce::Desktop::getInstance().removeGlobalMouseListener(this);
        }
    }
    
    void SuggestPopupEditor::timerCallback()
    {
        if(!juce::Process::isForegroundProcess())
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
    
    bool SuggestPopupEditor::keyPressed(juce::KeyPress const& key, Component* component)
    {
        if(component == &m_editor)
        {
            if(key == juce::KeyPress::downKey)
            {
                if(m_popup)
                {
                    m_popup->setFirstItemFocused();
                }
                
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
    
    void SuggestPopupEditor::focusLost(FocusChangeType cause)
    {
        std::cout << "SuggestPopupEditor::focusLost" << '\n';
        
        if (m_popup && !m_popup->hasKeyboardFocus(true))
        {
            //m_popup.reset();
            //m_listeners.call(&Listener::textEditorFocusLost, *this);
        }
    }
    
    void SuggestPopupEditor::resized()
    {
        m_editor.setBounds(getLocalBounds());
    }
}
