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

#include "jConsole.hpp"
#include "KiwiApp.hpp"
#include "StoredSettings.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                  CONSOLE COMPONENT                               //
    // ================================================================================ //
    
    jConsole::jConsole(sConsoleHistory history) :
    m_history(history),
    m_font(13.f)
    {
        assert(history);

        history->addListener(*this);
        
        setSize(300, 500);
        
        juce::TableHeaderComponent* header = new juce::TableHeaderComponent();
        
        header->addColumn(juce::String("ID"),
                          Column::Id, 30, 20, 50,
                          juce::TableHeaderComponent::defaultFlags, -1);
        
        header->addColumn(juce::String("Type"),
                          Column::Type, 30, 20, 50,
                          juce::TableHeaderComponent::defaultFlags, -1);
        
        header->addColumn(juce::String("Object"),
                          Column::Object, 60, 20, 10000,
                          juce::TableHeaderComponent::defaultFlags, -1);
        
        header->addColumn(juce::String("Message"),
                          Column::Message, 100, 40, 10000,
                          juce::TableHeaderComponent::defaultFlags, -1);
        
        header->setStretchToFitActive(false);
        header->setColumnVisible(1, false);
        header->setColumnVisible(2, false);
        header->addListener(this);
        
        m_table.setWantsKeyboardFocus(true);
        m_table.setMultipleSelectionEnabled(true);
        m_table.setMouseMoveSelectsRows(false);
        m_table.setHeaderHeight(m_font.getHeight() + 15);
        m_table.setRowHeight(m_font.getHeight() + 5);
        m_table.setColour(juce::ListBox::ColourIds::backgroundColourId, juce::Colours::transparentWhite);
        m_table.getViewport()->setScrollBarsShown(true, false, true, false);
        m_table.getViewport()->setScrollBarThickness(10);
        m_table.setModel(this);
        m_table.setHeader(header);
        addAndMakeVisible(m_table);
    }
    
    jConsole::~jConsole()
    {
        sConsoleHistory history = getHistory();
        if(history)
        {
            history->removeListener(*this);
        }
    }
    
    sConsoleHistory jConsole::getHistory()
    {
        return m_history.lock();
    }
    
    // ================================================================================ //
    //                                      COMMAND                                     //
    // ================================================================================ //
    
    void jConsole::copy()
    {
        sConsoleHistory history = getHistory();
        if(history)
        {
            juce::String text;
            juce::SparseSet<int> selection = m_table.getSelectedRows();
            
            for(size_t i = 0; i < selection.size(); i++)
            {
                auto msg = history->get(selection[i]);
                if(msg && !msg->text.empty())
                {
                    text += msg->text + "\n";
                }
            }
            
            juce::SystemClipboard::copyTextToClipboard(text);
        }
    }
    
    void jConsole::erase()
    {
        sConsoleHistory history = getHistory();
        if(history)
        {
            juce::SparseSet<int> selection = m_table.getSelectedRows();
            std::vector<size_t> select;
            
            for(size_t i = 0; i < selection.size(); i++)
            {
                select.push_back(selection[i]);
            }
            
            history->erase(select);
            m_table.setVerticalPosition(0);
        }
    }
    
    // ================================================================================ //
    //                                  HISTORY LISTENER                                //
    // ================================================================================ //
    
    void jConsole::consoleHistoryChanged(ConsoleHistory const&)
    {
        m_table.updateContent();
    }
    
    // ================================================================================ //
    //                                      COMPONENT                                   //
    // ================================================================================ //
    
    void jConsole::resized()
    {
        m_table.setBounds(getLocalBounds());
        updateRighmostColumnWidth(&m_table.getHeader());
    }
    
    void jConsole::paint(juce::Graphics& g)
    {
        int width   = m_table.getVisibleContentWidth();
        int rowheight = m_table.getRowHeight();
        g.setColour(juce::Colours::black.withAlpha(0.15f));
        for(int i = m_table.getHeaderHeight() - 1; i < getHeight(); i+= rowheight)
        {
             g.drawHorizontalLine(i, 0, width);
        }
    }
    
    // ================================================================================ //
    //                              TABLE LIST BOX MODEL                                //
    // ================================================================================ //
    
    void jConsole::selectedRowsChanged(int row)
    {
        KiwiApp::commandStatusChanged();
    }
    
    void jConsole::deleteKeyPressed(int lastRowSelected)
    {
        erase();
    }
    
    void jConsole::backgroundClicked(const juce::MouseEvent& mouse)
    {
        m_table.deselectAllRows();
    }
    
    int jConsole::getNumRows()
    {
        sConsoleHistory history = getHistory();
        return history ? history->size() : 0;
    }
    
    void jConsole::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool selected)
    {
        sConsoleHistory history = getHistory();
        if(!history) return; //abort
        
        auto msg = history->get(rowNumber);
        if(msg)
        {
            if(selected)
            {
                g.fillAll(juce::Colours::lightsteelblue);
            }
            else if(msg->type == engine::Console::Message::Type::Error)
            {
                g.fillAll(juce::Colours::lightpink);
            }
            else if(msg->type == engine::Console::Message::Type::Warning)
            {
                g.fillAll(juce::Colours::lightgoldenrodyellow);
            }
            else
            {
                g.fillAll(juce::Colour(0xfffefefe));
            }
        }
        else
        {
            g.fillAll(juce::Colour(0xfffefefe));
        }
    
        g.setColour(juce::Colours::black.withAlpha(0.15f));
        g.drawLine(0, height, width, height);
    }
    
    void jConsole::paintOverChildren(juce::Graphics &g)
    {
        int numColumns = m_table.getHeader().getNumColumns(true);
        float left = 0, width = 0;
        
        for(int i = 0; i < numColumns; i++)
        {
            width = m_table.getHeader().getColumnWidth(m_table.getHeader().getColumnIdOfIndex(i, true));
            
            if(m_table.getVisibleContentWidth() >= width + left)
            {
                g.setColour(juce::Colours::black.withAlpha(0.15f));
                g.drawLine(left + width, m_table.getHeaderHeight(), left + width, getHeight());
            }
            
            left += width;
        }
    }
    
    void jConsole::paintCell(juce::Graphics& g,
                             int rowNumber, int columnId, int width, int height,
                             bool rowIsSelected)
    {
        sConsoleHistory history = getHistory();
        if(!history) return; //abort
        
        auto msg = history->get(rowNumber);
        
        if(msg)
        {
            g.setColour(juce::Colours::black.brighter(0.4));
            g.setFont(m_font);
            
            switch (columnId)
            {
                case Column::Object:
                {
                    // todo
                    break;
                }
                    
                case Column::Message:
                {
                    g.drawText(msg->text, 2, 0, width - 4, height,
                               juce::Justification::centredLeft, true);
                    break;
                }
                    
                default: break;
            }
        }
    }
    
    void jConsole::sortOrderChanged(int newSortColumnId, bool isForwards)
    {
        sConsoleHistory history = getHistory();
        if(!history) return; //abort
        
        history->sort(static_cast<ConsoleHistory::Sort>(newSortColumnId));
        m_table.updateContent();
    }
    
    void jConsole::cellDoubleClicked(int rowNumber, int columnId, const juce::MouseEvent& mouse)
    {
        // TODO : hilight (if possible) object corresponding to the dblclicked row
    }
    
    juce::Component* jConsole::refreshComponentForCell(int, int, bool, Component* existingComponentToUpdate)
    {
        // Just return 0, as we'll be painting these columns directly.
        jassert(existingComponentToUpdate == 0);
        return 0;
    }
    
    // This is overloaded from TableListBoxModel, and should choose the best width for the specified
    // column.
    int jConsole::getColumnAutoSizeWidth(int columnId)
    {
        if(columnId == Column::Id)
            return 30;
        
        int widest = 32;
        
        sConsoleHistory history = getHistory();
        if(history)
        {
            // find the widest bit of text in this column..
            for(int i = getNumRows(); --i >= 0;)
            {
                const auto msg = history->get(i);
                if(msg)
                {
                    widest = std::max(widest, m_font.getStringWidth(msg->text));
                }
            }
        }
        
        return widest + 8;
    }
    
    void jConsole::tableColumnsResized(juce::TableHeaderComponent* tableHeader)
    {
        if(tableHeader == &m_table.getHeader())
        {
            updateRighmostColumnWidth(&m_table.getHeader());
        }
    }
    
    void jConsole::updateRighmostColumnWidth(juce::TableHeaderComponent* header)
    {
        int rightmostColumnId   = 0;
        int rightmostColumnX    = 0;
        
        for(int i = 0; i <= header->getNumColumns(false) - 1; i++)
        {
            if(header->isColumnVisible(i) && rightmostColumnId <= header->getIndexOfColumnId(i, true))
            {
                rightmostColumnId = i;
            }
        }
        
        rightmostColumnId   = header->getColumnIdOfIndex(header->getNumColumns(true)-1, true);
        rightmostColumnX    = header->getTotalWidth() - header->getColumnWidth(rightmostColumnId);
        
        if(rightmostColumnX <= getWidth())
        {
            m_table.getHeader().setColumnWidth(rightmostColumnId, getWidth() - rightmostColumnX);
        }
    }
    
    // ================================================================================ //
    //                                  CONSOLE WINDOW                                  //
    // ================================================================================ //
    
    jConsoleWindow::jConsoleWindow(sConsoleHistory history) :
    jWindow("Kiwi Console", juce::Colours::white, minimiseButton | closeButton, true)
    {
        setContentOwned(new jConsole(history), false);
        setResizable(true, false);
        setResizeLimits(50, 50, 32000, 32000);
        
        const juce::String windowState(getGlobalProperties().getValue("console_window"));
        
        if(windowState.isNotEmpty())
        {
            restoreWindowStateFromString(windowState);
        }
        else
        {
            setTopLeftPosition(0, 0);
            setSize(300, 440);
            setVisible(true);
        }
    }
    
    jConsoleWindow::~jConsoleWindow()
    {
        getGlobalProperties().setValue("console_window", getWindowStateAsString());
    }
    
    void jConsoleWindow::closeButtonPressed()
    {
        setVisible(false);
    }
}