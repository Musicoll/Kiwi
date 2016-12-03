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

#include "KiwiApp_SuggestEditor.hpp"
#include "KiwiApp.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                SUGGEST EDITOR MENU                               //
    // ================================================================================ //

    SuggestEditor::Menu::Menu(SuggestList& list) :
    m_suggest_list(list),
    m_resizable_corner(this, &m_constrainer)
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
        
        m_constrainer.setMinimumSize(200, 150);
        m_resizable_corner.setAlwaysOnTop(true);
        addAndMakeVisible(m_resizable_corner);
        
        const juce::String popup_size(getGlobalProperties().getValue("suggest_popup_size"));
        if(!popup_size.isEmpty())
        {
            juce::StringArray tokens;
            tokens.addTokens(popup_size, false);
            tokens.removeEmptyStrings();
            tokens.trim();
            
            if(tokens.size() == 2)
            {
                setSize(tokens[0].getIntValue(), tokens[1].getIntValue());
            }
        }
    }
    
    SuggestEditor::Menu::~Menu()
    {
        const juce::Point<int> size(getWidth(), getHeight());
        getGlobalProperties().setValue("suggest_popup_size", size.toString());
    }
    
    void SuggestEditor::Menu::paint(juce::Graphics & g)
    {
        g.fillAll(juce::Colours::white);
    }
    
    void SuggestEditor::Menu::resized()
    {
        m_suggest_list_box.setBounds(getLocalBounds());
        
        const int resizer_size = 18;
        m_resizable_corner.setBounds(getWidth() - resizer_size,
                                     getHeight() - resizer_size,
                                     resizer_size, resizer_size);
    }
    
    void SuggestEditor::Menu::selectRow(int row)
    {
        m_suggest_list_box.selectRow(row);
    }
 
    void SuggestEditor::Menu::selectFirstRow()
    {
        m_suggest_list_box.selectRow(0);
    }
    
    void SuggestEditor::Menu::selectPreviousRow()
    {
        const auto rows = getNumRows();
        const auto row_sel = m_suggest_list_box.getSelectedRow();
        const auto new_row_sel = (row_sel == -1) ? rows - 1 : ((row_sel <= 0) ? rows - 1 : row_sel - 1);
        m_suggest_list_box.selectRow(new_row_sel);
    }
    
    void SuggestEditor::Menu::selectNextRow()
    {
        const auto rows = getNumRows();
        const auto row_sel = m_suggest_list_box.getSelectedRow();
        const auto new_row_sel = (row_sel == -1) ? 0 : ((row_sel >= rows) ? 0 : row_sel + 1);
        m_suggest_list_box.selectRow(new_row_sel);
    }
    
    void SuggestEditor::Menu::update()
    {
        m_suggest_list_box.updateContent();
    }
    
    void SuggestEditor::Menu::setItemClickedAction(action_method_t function)
    {
        m_clicked_action = function;
    }
    
    void SuggestEditor::Menu::setItemDoubleClickedAction(action_method_t function)
    {
        m_double_clicked_action = function;
    }
    
    void SuggestEditor::Menu::setSelectedItemAction(action_method_t function)
    {
        m_selected_action = function;
    }
    
    // ================================================================================ //
    //                               SUGGEST LISTBOX MODEL                              //
    // ================================================================================ //
    
    int SuggestEditor::Menu::getNumRows()
    {
        return m_suggest_list.size();
    }
    
    void SuggestEditor::Menu::paintListBoxItem(int row_number, juce::Graphics& g,
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
    
    void SuggestEditor::Menu::listBoxItemClicked(int row, juce::MouseEvent const& e)
    {
        if(m_clicked_action && (row < m_suggest_list.size()))
        {
            std::string const& str = *(m_suggest_list.begin() + row);
            m_clicked_action(str);
        }
    }
    
    void SuggestEditor::Menu::listBoxItemDoubleClicked(int row, juce::MouseEvent const& e)
    {
        if(m_double_clicked_action && (row < m_suggest_list.size()))
        {
            std::string const& str = *(m_suggest_list.begin() + row);
            m_double_clicked_action(str);
        }
    }
    
    void SuggestEditor::Menu::selectedRowsChanged(int last_row_selected)
    {
        if(m_selected_action && (last_row_selected < m_suggest_list.size()))
        {
            std::string const& str = *(m_suggest_list.begin() + last_row_selected);
            m_selected_action(str);
        }
    }
    
    // ================================================================================ //
    //                                  SUGGEST EDITOR                                  //
    // ================================================================================ //
    
    SuggestEditor::SuggestEditor(SuggestList::entries_t entries)
    : m_suggest_list(std::move(entries))
    {
        setReturnKeyStartsNewLine(false);
        setTabKeyUsedAsCharacter(false);
        setPopupMenuEnabled(false);
        
        juce::TextEditor::addListener(this);
    }
    
    SuggestEditor::~SuggestEditor()
    {
        closeMenu();
    }
    
    void SuggestEditor::addListener(SuggestEditor::Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void SuggestEditor::removeListener(SuggestEditor::Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    void SuggestEditor::showMenu()
    {        
        m_menu.reset(new Menu(m_suggest_list));
        
        using namespace std::placeholders; // for _1, _2 etc.
        m_menu->setSelectedItemAction(std::bind(&SuggestEditor::menuSelectionChanged, this, _1));
        m_menu->setItemClickedAction(std::bind(&SuggestEditor::menuItemClicked, this, _1));
        m_menu->setItemDoubleClickedAction(std::bind(&SuggestEditor::menuItemDoubleClicked, this, _1));
        
        m_menu->addToDesktop(juce::ComponentPeer::windowIsTemporary
                              | juce::ComponentPeer::windowHasDropShadow
                              | juce::ComponentPeer::windowIgnoresKeyPresses);
        
        const auto sb = getScreenBounds();
        m_menu->setTopLeftPosition(sb.getX() - 2, sb.getBottom() + 2);
        m_menu->setVisible(true);
        
        m_need_complete = true;
        
        startTimer(200);
    }
    
    bool SuggestEditor::isMenuOpened() const noexcept
    {
        return static_cast<bool>(m_menu);
    }
    
    void SuggestEditor::closeMenu()
    {
        if(isMenuOpened())
        {
            m_menu.reset();
            stopTimer();
        }
    }
    
    void SuggestEditor::textEditorTextChanged(juce::TextEditor&)
    {
        m_typed_text = getText();
        m_suggest_list.applyFilter(m_typed_text.toStdString());
        
        setCaretVisible(true);
        
        if(isMenuOpened() && m_suggest_list.empty())
        {
            closeMenu();
        }
        else if(!m_suggest_list.empty())
        {
            if(isMenuOpened())
                m_menu->update();
            else if(!m_typed_text.isEmpty())
                showMenu();
            
            if(!m_typed_text.isEmpty() && m_need_complete)
            {
                const juce::String best_suggestion = *m_suggest_list.begin();
                const auto caret_pos = getCaretPosition();
                const int suggest_length = static_cast<int>(best_suggestion.length());
                if(caret_pos < suggest_length && best_suggestion.startsWith(m_typed_text))
                {
                    setText(best_suggestion, juce::dontSendNotification);
                    setHighlightedRegion({caret_pos, suggest_length});
                    setCaretVisible(false);
                }
            }
        }
        
        m_need_complete = true;
        m_listeners.call(&Listener::suggestEditorTextChanged, *this);
    }
    
    void SuggestEditor::returnPressed()
    {
        m_listeners.call(&Listener::suggestEditorReturnKeyPressed, *this);
    }
    
    void SuggestEditor::escapePressed()
    {
        m_listeners.call(&Listener::suggestEditorEscapeKeyPressed, *this);
    }
    
    void SuggestEditor::focusLost(juce::Component::FocusChangeType focus_change)
    {
        juce::TextEditor::focusLost(focus_change);
        m_listeners.call(&Listener::suggestEditorFocusLost, *this);
    }
    
    void SuggestEditor::menuSelectionChanged(juce::String const& text)
    {
        setText(text, juce::dontSendNotification);
        setHighlightedRegion({getCaretPosition(), text.length()});
        setCaretVisible(false);
        m_listeners.call(&Listener::suggestEditorTextChanged, *this);
    }
    
    void SuggestEditor::menuItemClicked(juce::String const& text)
    {
        setText(text, juce::dontSendNotification);
        setHighlightedRegion({getCaretPosition(), text.length()});
        setCaretVisible(false);
    }
    
    void SuggestEditor::menuItemDoubleClicked(juce::String const& text)
    {
        setText(text, juce::dontSendNotification);
        setCaretPosition(text.length());
        setCaretVisible(true);
        closeMenu();
    }
    
    void SuggestEditor::timerCallback()
    {
        if(!juce::Process::isForegroundProcess())
            closeMenu();
        
        if(isMenuOpened())
        {
            const auto sb = getScreenBounds();
            
            // check focus lost or menu position change
            if((!hasKeyboardFocus(true) && !m_menu->hasKeyboardFocus(true))
               || m_menu->getPosition() != juce::Point<int>(sb.getX() - 2, sb.getBottom() + 2))
            {
                closeMenu();
            }
        }
        else
        {
            if (!hasKeyboardFocus(true))
                closeMenu();
        }
    }
    
    bool SuggestEditor::keyPressed(juce::KeyPress const& key)
    {
        setCaretVisible(true);
        
        m_need_complete = !(key == juce::KeyPress::deleteKey || key == juce::KeyPress::backspaceKey);
        
        bool result = juce::TextEditor::keyPressed(key);
        
        if(isMenuOpened() && key == juce::KeyPress::downKey)
        {
            m_menu->selectNextRow();
            result = true;
        }
        else if(isMenuOpened() && key == juce::KeyPress::upKey)
        {
            m_menu->selectPreviousRow();
            result = true;
        }
        
        return result;
    }
}
