/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright(c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2(or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#include "jConsole.hpp"
//#include "Application.h"

namespace kiwi
{
    //std::unique_ptr<Console::History> jConsole::m_history = Console::History::create();
    
    // ================================================================================ //
    //                                  CONSOLE COMPONENT                               //
    // ================================================================================ //
    
    jConsole::jConsole() : m_history(Console::History::create()), m_font(13.f)
    {
        m_history->addListener(*this);
        
        setSize(300, 500);
        
        TableHeaderComponent* header = new TableHeaderComponent();
        header->addColumn(String("ID"),        Column::Id, 30, 20, 50, TableHeaderComponent::defaultFlags, -1);
        header->addColumn(String("Type"),      Column::Type, 30, 20, 50, TableHeaderComponent::defaultFlags, -1);
        header->addColumn(String("Object"),    Column::Object, 60, 20, 10000, TableHeaderComponent::defaultFlags, -1);
        header->addColumn(String("Message"),   Column::Message, 100, 40, 10000, TableHeaderComponent::defaultFlags, -1);
        header->setStretchToFitActive(false);
        header->setColumnVisible(1, false);
        header->setColumnVisible(2, false);
        header->addListener(this);
        
        m_table.setWantsKeyboardFocus(true);
        m_table.setMultipleSelectionEnabled(true);
        m_table.setMouseMoveSelectsRows(false);
        m_table.setHeaderHeight(m_font.getHeight() + 15);
        m_table.setRowHeight(m_font.getHeight() + 5);
        m_table.setColour(ListBox::ColourIds::backgroundColourId, Colours::transparentWhite);
        m_table.getViewport()->setScrollBarsShown(true, false, true, false);
        m_table.getViewport()->setScrollBarThickness(10);
        m_table.setModel(this);
        m_table.setHeader(header);
        addAndMakeVisible(m_table);
    }
    
    jConsole::~jConsole()
    {
        m_history->removeListener(*this);
    }
    
    // ================================================================================ //
    //                                      COMMAND                                     //
    // ================================================================================ //
    
    void jConsole::copy()
    {
        String text;
        SparseSet<int> selection = m_table.getSelectedRows();
        for(size_t i = 0; i < selection.size(); i++)
        {
            auto msg = m_history->get(selection[i]);
            if(msg && !msg->getText().empty())
            {
                text += msg->getText() + "\n";
            }
        }
        
        SystemClipboard::copyTextToClipboard(text);
    }
    
    void jConsole::erase()
    {
        SparseSet<int> selection = m_table.getSelectedRows();
        std::vector<size_t> select;
        for(size_t i = 0; i < selection.size(); i++)
        {
            select.push_back(selection[i]);
        }
        m_history->erase(select);
        m_table.setVerticalPosition(0);
    }
    
    // ================================================================================ //
    //                                  HISTORY LISTENER                                //
    // ================================================================================ //
    
    void jConsole::consoleHistoryChanged(Console::History const&)
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
    
    void jConsole::paint(Graphics& g)
    {
        int width   = m_table.getVisibleContentWidth();
        int rowheight = m_table.getRowHeight();
        g.setColour(Colours::black.withAlpha(0.15f));
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
        //Application::commandStatusChanged();
    }
    
    void jConsole::deleteKeyPressed(int lastRowSelected)
    {
        erase();
    }
    
    void jConsole::backgroundClicked(const MouseEvent& mouse)
    {
        m_table.deselectAllRows();
    }
    
    int jConsole::getNumRows()
    {
        return m_history->size();
    }
    
    void jConsole::paintRowBackground(Graphics& g,
                                      int rowNumber, int width, int height, bool selected)
    {
        auto msg = m_history->get(rowNumber);
        if(msg)
        {
            if(selected)
            {
                g.fillAll(Colours::lightsteelblue);
            }
            else if(msg->getType() == Console::MessageType::Error)
            {
                g.fillAll(Colours::lightpink);
            }
            else if(msg->getType() == Console::MessageType::Warning)
            {
                g.fillAll(Colours::lightgoldenrodyellow);
            }
            else
            {
                g.fillAll(Colour(0xfffefefe));
            }
        }
        else
        {
            g.fillAll(Colour(0xfffefefe));
        }
    
        g.setColour(Colours::black.withAlpha(0.15f));
        g.drawLine(0, height, width, height);
    }
    
    void jConsole::paintOverChildren(Graphics &g)
    {
        int numColumns = m_table.getHeader().getNumColumns(true);
        float left = 0, width = 0;
        
        for(int i = 0; i < numColumns; i++)
        {
            width = m_table.getHeader().getColumnWidth(m_table.getHeader().getColumnIdOfIndex(i, true));
            
            if(m_table.getVisibleContentWidth() >= width + left)
            {
                g.setColour(Colours::black.withAlpha(0.15f));
                g.drawLine(left + width, m_table.getHeaderHeight(), left + width, getHeight());
            }
            
            left += width;
        }
    }
    
    void jConsole::paintCell(Graphics& g,
                             int rowNumber, int columnId, int width, int height,
                             bool rowIsSelected)
    {
        auto msg = m_history->get(rowNumber);
        if(msg)
        {
            g.setColour(Colours::black.brighter(0.4));
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
                    g.drawText(msg->getText(), 2, 0, width - 4, height,
                               Justification::centredLeft, true);
                    break;
                }
                    
                default: break;
            }
        }
    }
    
    void jConsole::sortOrderChanged(int newSortColumnId, bool isForwards)
    {
        m_history->sort(static_cast<Console::History::Sort>(newSortColumnId));
        m_table.updateContent();
    }
    
    void jConsole::cellDoubleClicked(int rowNumber, int columnId, const MouseEvent& mouse)
    {
        // TODO : hilight (if possible) object corresponding to the dblclicked row
    }
    
    Component* jConsole::refreshComponentForCell(int, int, bool,
                                                 Component* existingComponentToUpdate)
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
        
        // find the widest bit of text in this column..
        for(int i = getNumRows(); --i >= 0;)
        {
            auto msg = m_history->get(i);
            
            if(msg)
            {
                widest = std::max(widest, m_font.getStringWidth(msg->getText()));
            }
        }
        
        return widest + 8;
    }
    
    void jConsole::tableColumnsResized(TableHeaderComponent* tableHeader)
    {
        if(tableHeader == &m_table.getHeader())
        {
            updateRighmostColumnWidth(&m_table.getHeader());
        }
    }
    
    void jConsole::updateRighmostColumnWidth(TableHeaderComponent* header)
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
    
    jConsoleWindow::jConsoleWindow() : jWindow("Kiwi Console",
                                             Colours::white,
                                             minimiseButton | closeButton, true)
    {
        setContentOwned(new jConsole(), false);
        setResizable(true, false);
        setResizeLimits(50, 50, 32000, 32000);
        setTopLeftPosition(0, 0);
        setSize(300, 440);
        setVisible(true);
    }
    
    jConsoleWindow::~jConsoleWindow()
    {
        ;
    }
    
    void jConsoleWindow::closeButtonPressed()
    {
        setVisible(false);
    }
}