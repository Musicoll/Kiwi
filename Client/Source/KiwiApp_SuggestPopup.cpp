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

    SuggestPopup::SuggestPopup(SuggestList& list) :
    m_suggest_list(list),
    m_resizable_corner(this, nullptr)
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
        
        setWantsKeyboardFocus(false);
        setMouseClickGrabsKeyboardFocus(false);
        m_suggest_list_box.setMouseClickGrabsKeyboardFocus(false);
        m_suggest_list_box.setWantsKeyboardFocus(false);
        addAndMakeVisible(m_suggest_list_box);
        
        m_resizable_corner.setAlwaysOnTop(true);
        addAndMakeVisible(m_resizable_corner);
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
        
        const int resizer_size = 18;
        m_resizable_corner.setBounds(getWidth() - resizer_size,
                                     getHeight() - resizer_size,
                                     resizer_size, resizer_size);
    }
    
    void SuggestPopup::setFirstItemFocused()
    {
        //toFront(true);
        m_suggest_list_box.selectRow(0);
    }
    
    void SuggestPopup::selectPreviousRow()
    {
        int rows = getNumRows();
        const auto selected_row = m_suggest_list_box.getSelectedRow();
        auto new_selected_row = (selected_row == -1) ? -1 : (selected_row <= 0) ? rows - 1 : selected_row - 1;
        m_suggest_list_box.selectRow(new_selected_row);
    }
    
    void SuggestPopup::selectNextRow()
    {
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
    
    void SuggestPopup::update()
    {
        m_suggest_list_box.updateContent();
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
        
        juce::TextEditor::addListener(this);
        addKeyListener(this);
    }
    
    SuggestPopupEditor::~SuggestPopupEditor()
    {
        dismissMenu();
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
        assert(!isMenuOpened() && "Menu already opened");
        
        m_popup.reset(new SuggestPopup(m_suggest_list));
        
        const auto on_select_change_fn = [this](juce::String const& text)
        {
            setText(text, juce::dontSendNotification);
            setHighlightedRegion({m_typed_text.length(), text.length()});
            grabKeyboardFocus();
            m_listeners.call(&Listener::textEditorTextChanged, *this);
        };
        
        const auto on_item_clicked_fn = [this](juce::String const& text)
        {
            setText(text, juce::dontSendNotification);
            setHighlightedRegion({m_typed_text.length(), text.length()});
            grabKeyboardFocus();
        };
        
        const auto on_item_double_clicked_fn = [this](juce::String const& text)
        {
            setText(text, juce::dontSendNotification);
            setHighlightedRegion({m_typed_text.length(), text.length()});
            grabKeyboardFocus();
            dismissMenu();
            //m_listeners.call(&Listener::textEditorReturnKeyPressed, *this);
        };
        
        const auto on_delete_key_pressed_fn = [this](juce::String const& text)
        {
            toFront(true);
            deleteBackwards(false);
        };
        
        m_popup->setSelectedItemAction(on_select_change_fn);
        m_popup->setItemClickedAction(on_item_clicked_fn);
        m_popup->setItemDoubleClickedAction(on_item_double_clicked_fn);
        m_popup->setDeleteKeyPressedAction(on_delete_key_pressed_fn);
        
        m_popup->addToDesktop(juce::ComponentPeer::windowIsTemporary
                              | juce::ComponentPeer::windowHasDropShadow
                              | juce::ComponentPeer::windowIgnoresKeyPresses);

        m_popup->setBounds(getScreenBounds().translated(-2, getHeight() + 2).withSize(200, 150));
        m_popup->setVisible(true);
        
        grabKeyboardFocus();
        toFront(true);
        
        startTimer(200);
    }
    
    void SuggestPopupEditor::updateMenu()
    {
        assert(isMenuOpened() && "Call showMenu() before");
        assert(m_popup->isOnDesktop());
        
        //m_popup->setVisible(true);
        m_popup->setBounds(getScreenBounds().translated(-2, getHeight() + 2).withSize(200, 150));
        if(!m_suggest_list.empty() && m_typed_text == *m_suggest_list.begin())
        {
            m_popup->setFirstItemFocused();
        }
        
        m_popup->update();
        
        grabKeyboardFocus();
    }
    
    bool SuggestPopupEditor::isMenuOpened() const noexcept
    {
        return static_cast<bool>(m_popup);
    }
    
    void SuggestPopupEditor::textEditorTextChanged(juce::TextEditor&)
    {
        const auto old_text = m_typed_text;
        m_typed_text = getText();
        m_suggest_list.applyFilter(m_typed_text.toStdString());
        
        if(isMenuOpened() && m_suggest_list.empty())
        {
            dismissMenu();
        }
        else if(!m_suggest_list.empty())
        {
            if(isMenuOpened())
                updateMenu();
            else if(!m_typed_text.isEmpty())
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
        auto const* const focus_comp = getCurrentlyFocusedComponent();
        
        std::cout << "focus lost" << "\n";
        
        if(focus_comp
           && ((focus_comp != this)
               && (!isMenuOpened()
                   || (isMenuOpened()
                       && (focus_comp != m_popup.get()
                           || (!m_popup->isParentOf(focus_comp)))))))
        {
            m_listeners.call(&Listener::textEditorFocusLost, *this);
        }
    }
    
    void SuggestPopupEditor::dismissMenu()
    {
        if(isMenuOpened())
        {
            m_popup.reset();
            stopTimer();
        }
    }
    
    void SuggestPopupEditor::timerCallback()
    {
        if(!juce::Process::isForegroundProcess())
            dismissMenu();
        
        if(isMenuOpened())
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
        if(component == this)
        {
            if(key == juce::KeyPress::downKey)
            {
                if(isMenuOpened())
                {
                    m_popup->selectNextRow();
                }
                
                return true;
            }
            else if(key == juce::KeyPress::upKey)
            {
                if(isMenuOpened())
                {
                    m_popup->selectPreviousRow();
                }
                
                return true;
            }
            else if(key == juce::KeyPress::rightKey)
            {
                m_typed_text = getText();
                setText(m_typed_text, juce::dontSendNotification);
                return false;
            }
            //else return juce::TextEditor::keyPressed(key);
        }
        /*
        else if(component == m_popup.get())
        {
            // if the user tries to type into the menu lets move the focus back there and inject the keypress
            toFront(true);
            return juce::TextEditor::keyPressed(key);
        }
        */
        return false;
    }
}
