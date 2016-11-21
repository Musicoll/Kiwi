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
        close();
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
    
    SuggestList const& SuggestPopup::useSuggestList() const
    {
        return m_suggest_list;
    }
    
    void SuggestPopup::show(Component* target_comp)
    {
        if(isOnDesktop()) return; // abort
        
        assert(target_comp != nullptr);
        
        setTopLeftPosition(target_comp->getX(), target_comp->getBottom());
        setSize(200, 150);
        m_suggest_list_box.setSize(getWidth(), getHeight());
        
        //Component* parent_comp = target_comp->getTopLevelComponent();
        target_comp->addAndMakeVisible(this);
        addToDesktop(juce::ComponentPeer::windowIsTemporary
                     | juce::ComponentPeer::windowHasDropShadow);
        
        enterModalState(false);
        
        setVisible(true);
    }
    
    void SuggestPopup::close()
    {
        exitModalState(0);
        removeFromDesktop();
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
        
        m_popup->addToDesktop(juce::ComponentPeer::StyleFlags::windowIsTemporary
                              | juce::ComponentPeer::windowHasDropShadow);
        
        m_popup->setVisible(true);
        m_popup->setBounds(getScreenBounds().translated(0, getHeight()).withSize(200, 150));
        
        const auto change_text_fn = [this](juce::String text)
        {
            juce::String old_text = m_editor.getText();
            m_editor.setText(text, juce::dontSendNotification);
            m_editor.setHighlightedRegion({old_text.length(), text.length()});
            //m_editor.grabKeyboardFocus();
        };
        
        m_popup->setSelectedItemAction(change_text_fn);
        m_popup->setItemClickedAction(change_text_fn);
        m_popup->setItemDoubleClickedAction([this](juce::String text)
                                            {
                                                m_editor.setText(text, juce::dontSendNotification);
                                                dismissMenu();
                                                m_editor.grabKeyboardFocus();
                                            });
        
        m_popup->addKeyListener(this);
        
        juce::Desktop::getInstance().addGlobalMouseListener(this);
        
        m_editor.grabKeyboardFocus();
        
        startTimer(200);
    }
    
    void SuggestPopupEditor::mouseDown(juce::MouseEvent const& event)
    {
        if (!isParentOf(event.eventComponent))
            dismissMenu();
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
            showMenu();
        }
        
        m_listeners.call(&Listener::textEditorTextChanged, *this);
    }
    
    void SuggestPopupEditor::dismissMenu()
    {
        m_popup.reset();
        stopTimer();
        juce::Desktop::getInstance().removeGlobalMouseListener(this);
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
        if (m_popup && !m_popup->hasKeyboardFocus(true))
            m_popup.reset();
    }
    
    void SuggestPopupEditor::resized()
    {
        m_editor.setBounds(getLocalBounds());
    }
}
