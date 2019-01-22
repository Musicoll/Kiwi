/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
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

    SuggestEditor::Menu::Menu(SuggestList& list, SuggestEditor & creator) :
    m_suggest_list(list),
    m_suggest_list_box(),
    m_creator(creator),
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
    
    void SuggestEditor::Menu::unselectRow()
    {
        m_suggest_list_box.selectRow(-1);
    }
    
    void SuggestEditor::Menu::selectPreviousRow()
    {
        const auto rows = getNumRows();
        const auto row_sel = m_suggest_list_box.getSelectedRow();
        const auto new_row_sel = (row_sel == -1) ? rows - 1 : ((row_sel <= 0) ? - 1 : row_sel - 1);
        m_suggest_list_box.selectRow(new_row_sel);
    }
    
    void SuggestEditor::Menu::selectNextRow()
    {
        const auto rows = getNumRows();
        const auto row_sel = m_suggest_list_box.getSelectedRow();
        const auto new_row_sel = (row_sel == -1) ? 0 : ((row_sel >= rows) ? -1 : row_sel + 1);
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
    
    void SuggestEditor::Menu::setUnselectedItemAction(std::function<void(void)> function)
    {
        m_unselected_action = function;
    }
    
    int SuggestEditor::Menu::getSelectedRow() const
    {
        return m_suggest_list_box.getSelectedRow();
    }
    
    bool SuggestEditor::Menu::canModalEventBeSentToComponent(juce::Component const* target_component)
    {
        return target_component == &m_creator;
    }
    
    void SuggestEditor::Menu::inputAttemptWhenModal()
    {
        if (juce::Desktop::getInstance().getMainMouseSource().getComponentUnderMouse() != &m_creator)
        {
            exitModalState(0);
            m_creator.closeMenu();
        }
        else
        {
            m_creator.setCaretVisible(true);
        }
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
        if (last_row_selected == -1 && m_unselected_action)
        {
            m_unselected_action();
        }
        else if((last_row_selected < m_suggest_list.size()) && m_selected_action)
        {
            std::string const& suggestion = *(m_suggest_list.begin() + last_row_selected);;
            m_selected_action(suggestion);
        }
    }
    
    // ================================================================================ //
    //                                  SUGGEST EDITOR                                  //
    // ================================================================================ //
    
    SuggestEditor::SuggestEditor(SuggestList::entries_t entries) :
    m_suggest_list(std::move(entries)),
    m_split_text(),
    m_menu(),
    m_update_enabled(true)
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
        m_menu.reset(new Menu(m_suggest_list, *this));
        
        using namespace std::placeholders; // for _1, _2 etc.
        m_menu->setSelectedItemAction(std::bind(&SuggestEditor::menuItemSelected, this, _1));
        m_menu->setItemValidatedAction(std::bind(&SuggestEditor::menuItemValidated, this, _1));
        m_menu->setUnselectedItemAction(std::bind(&SuggestEditor::menuItemUnselected, this));
        
        m_menu->addToDesktop(juce::ComponentPeer::windowIsTemporary
                              | juce::ComponentPeer::windowHasDropShadow
                              | juce::ComponentPeer::windowIgnoresKeyPresses);
        
        m_menu->enterModalState(false);
        
        const auto sb = getScreenBounds();
        m_menu->setTopLeftPosition(sb.getX() - 2, sb.getBottom() + 2);
        m_menu->setVisible(true);
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
        }
    }
    
    void SuggestEditor::disableUpdate()
    {
        m_update_enabled = false;
    }
    
    void SuggestEditor::enableUpdate()
    {
        m_update_enabled = true;
    }
    
    void SuggestEditor::textEditorFocusLost(juce::TextEditor & editor)
    {
        if(isMenuOpened())
        {
            m_menu->exitModalState(0);
            closeMenu();
        }
    }
    
    void SuggestEditor::textEditorTextChanged(juce::TextEditor&)
    {
        if (m_update_enabled)
        {
            setCaretVisible(true);
            
            m_split_text.clear();
            m_split_text.addTokens(getText(), " ");
            
            if (m_split_text.size() == 1)
            {
                m_suggest_list.applyFilter(m_split_text[0].toStdString());
                
                if(isMenuOpened() && m_suggest_list.empty())
                {
                    closeMenu();
                }
                else if(!m_suggest_list.empty())
                {
                    if(isMenuOpened())
                    {
                        if (m_menu->getSelectedRow()  != -1)
                        {
                            m_menu->unselectRow();
                        }
                        
                        m_menu->update();
                    }
                    else
                    {
                        showMenu();
                    }
                }
            }
            else if (isMenuOpened())
            {
                closeMenu();
            }
        }
    }
    
    void SuggestEditor::menuItemSelected(juce::String const& text)
    {
        juce::String editor_text = getText();
        
        int last_word_pos = editor_text.lastIndexOf(" ") + 1;
        
        juce::String replace_text = editor_text.replaceSection(last_word_pos,
                                                               editor_text.length() - last_word_pos,
                                                               text);
        
        disableUpdate();
        
        setText(replace_text);
        
        int highlight_start = std::min(last_word_pos + m_split_text[m_split_text.size() - 1].length(),
                                       replace_text.length());
        
        int highlight_end = replace_text.length();
        
        setHighlightedRegion({highlight_start, highlight_end});
        
        setCaretVisible(false);
    }
    
    void SuggestEditor::menuItemUnselected()
    {
        juce::String editor_text = getText();
        
        juce::String replace_text = editor_text.replaceSection(editor_text.lastIndexOf(" ") + 1,
                                                               editor_text.length() - editor_text.lastIndexOf(" ") - 1,
                                                               m_split_text[m_split_text.size() - 1]);
        
        disableUpdate();
        
        setText(replace_text);
    }
    
    void SuggestEditor::menuItemValidated(juce::String const& text)
    {
        juce::String editor_text = getText();
        
        juce::String replace_text = editor_text.replaceSection(editor_text.lastIndexOf(" ") + 1,
                                                               editor_text.length() - editor_text.lastIndexOf(" ") - 1,
                                                               text);
        
        setText(replace_text);
        
        closeMenu();
        
        moveCaretToEnd();
        setCaretVisible(true);
    }
    
    bool SuggestEditor::keyStateChanged(bool isKeyDown)
    {
        enableUpdate();
        
        return TextEditor::keyStateChanged(isKeyDown);
    }
    
    bool SuggestEditor::keyPressed(juce::KeyPress const& key)
    {
        setCaretVisible(true);
        
        bool result = false;
        
        if(key == juce::KeyPress::downKey && isMenuOpened())
        {
            m_menu->selectNextRow();
            result = true;
        }
        else if(key == juce::KeyPress::upKey && isMenuOpened())
        {
            m_menu->selectPreviousRow();
            result = true;
        }
        else if(key == juce::KeyPress::escapeKey)
        {
            if (isMenuOpened())
            {
                if (m_menu->getSelectedRow()  != -1)
                {
                    m_menu->unselectRow();
                }
                
                closeMenu();
            }
            else
            {
                m_split_text.clear();
                m_split_text.addTokens(getText(), " ");
                
                if (m_split_text.size() <= 1)
                {
                    m_suggest_list.applyFilter(m_split_text[0].toStdString());
                    
                    if (m_suggest_list.size() > 0)
                    {
                        showMenu();
                    }
                }
            }
            
            result = true;
        }
        
        if(!result)
        {
            result = juce::TextEditor::keyPressed(key);
        }
        
        return result;
    }
}
