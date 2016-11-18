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

    SuggestPopup::SuggestPopup(SuggestList::entries_t entries) : m_suggest_list(std::move(entries))
    {
        setAlwaysOnTop(true);
        setOpaque(true);
        m_suggest_list_box.setColour(juce::ListBox::ColourIds::backgroundColourId,
                                     juce::Colours::transparentBlack);
        
        m_suggest_list_box.setModel(this);
        
        setSize(200, 150);
        m_suggest_list_box.setMultipleSelectionEnabled(false);
        m_suggest_list_box.setSize(getWidth(), getHeight());
        
        addAndMakeVisible(m_suggest_list_box);
        
        //setWantsKeyboardFocus(false);
        //setInterceptsMouseClicks(false, false);
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
        g.fillAll(selected ? juce::Colours::grey : juce::Colours::white.darker(is_odd ? 0 : 0.1));
        if(row_number < m_suggest_list.size())
        {
            std::string const& str = *(m_suggest_list.begin() + row_number);
            g.drawSingleLineText(str, 10, height - g.getCurrentFont().getHeight()*0.5, juce::Justification::left);
        }
    }
    
    void SuggestPopup::listBoxItemClicked(int row, juce::MouseEvent const& e)
    {
        std::cout << "listBoxItemClicked" << "\n";
        
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
        ;
    }
    
    void SuggestPopup::returnKeyPressed(int last_row_selected)
    {
        ;
    }
}
