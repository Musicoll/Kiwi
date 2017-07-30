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

#include <KiwiApp_Components/KiwiApp_SuggestEditor.h>
#include <KiwiApp.h>

namespace kiwi
{
    // ================================================================================ //
    //                                SUGGEST EDITOR MENU                               //
    // ================================================================================ //

    SuggestEditor::Menu::Menu(SuggestList& list) :
    m_suggest_list(list),
    m_suggest_list_box(),
    m_constrainer(),
    m_resizable_corner(this, &m_constrainer),
    m_validated_action(),
    m_selected_action()
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
    
    void SuggestEditor::Menu::setItemValidatedAction(action_method_t function)
    {
        m_validated_action = function;
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
        if(m_selected_action && (row < m_suggest_list.size()))
        {
            std::string const& str = *(m_suggest_list.begin() + row);
            m_selected_action(str);
        }
    }
    
    void SuggestEditor::Menu::validateSelectedRow()
    {
        int selected_row = m_suggest_list_box.getSelectedRow();
        
        if (selected_row != -1 && m_validated_action)
        {
            std::string const& str = *(m_suggest_list.begin() + selected_row);
            m_validated_action(str);
        }
    }
    
    void SuggestEditor::Menu::listBoxItemDoubleClicked(int row, juce::MouseEvent const& e)
    {
        if (row < m_suggest_list.size())
        {
            selectRow(row);
            validateSelectedRow();
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
    
    SuggestEditor::SuggestEditor(SuggestList::entries_t entries) :
    m_suggest_list(std::move(entries)),
    m_splited_text(),
    m_menu()
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
    
    void SuggestEditor::showMenu()
    {        
        m_menu.reset(new Menu(m_suggest_list));
        
        using namespace std::placeholders; // for _1, _2 etc.
        m_menu->setSelectedItemAction(std::bind(&SuggestEditor::menuItemSelected, this, _1));
        m_menu->setItemValidatedAction(std::bind(&SuggestEditor::menuItemValidated, this, _1));
        
        m_menu->addToDesktop(juce::ComponentPeer::windowIsTemporary
                              | juce::ComponentPeer::windowHasDropShadow
                              | juce::ComponentPeer::windowIgnoresKeyPresses);
        
        const auto sb = getScreenBounds();
        m_menu->setTopLeftPosition(sb.getX() - 2, sb.getBottom() + 2);
        m_menu->setVisible(true);
        
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
    
    std::vector<juce::String> split(juce::String const& text, juce::String divider)
    {
        std::vector<juce::String> split;
        
        int current_index = 0;
        
        while(current_index < text.length())
        {
            if (text[current_index] != ' ')
            {
                int next_space_index = text.indexOfAnyOf(divider, current_index);
                int next_index = next_space_index != -1 ? next_space_index : text.length();
                
                split.push_back(text.substring(current_index, next_index));
                
                current_index = next_index;
            }
            else
            {
                ++current_index;
            }
        }
        
        return split;
    }
    
    void SuggestEditor::textEditorTextChanged(juce::TextEditor&)
    {
        m_splited_text = split(getText(), " ");
        
        if (m_splited_text.size() == 1)
        {
            m_suggest_list.applyFilter(m_splited_text[0].toStdString());
            
            setCaretVisible(true);
            
            if(isMenuOpened() && m_suggest_list.empty())
            {
                closeMenu();
            }
            else if(!m_suggest_list.empty())
            {
                if(isMenuOpened())
                {
                    m_menu->update();
                }
                else
                {
                    showMenu();
                    m_menu->selectFirstRow();
                }
            }
        }
        else if (isMenuOpened())
        {
            closeMenu();
        }
    }
    
    void SuggestEditor::menuItemSelected(juce::String const& text)
    {
    }
    
    void SuggestEditor::menuItemValidated(juce::String const& text)
    {
        setText(text);
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
        
        bool result = false;
        
        if (isMenuOpened())
        {
            if(key == juce::KeyPress::downKey)
            {
                m_menu->selectNextRow();
                result = true;
            }
            else if(key == juce::KeyPress::upKey)
            {
                m_menu->selectPreviousRow();
                result = true;
            }
            else if(key == juce::KeyPress::returnKey)
            {
                m_menu->validateSelectedRow();
                result = true;
            }
            else if(key == juce::KeyPress::escapeKey)
            {
                closeMenu();
                result = true;
            }
        }
        
        if(!result)
        {
            result = juce::TextEditor::keyPressed(key);
        }
        
        return result;
    }
}
