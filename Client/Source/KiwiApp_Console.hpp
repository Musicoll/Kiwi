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

#ifndef KIWI_APP_CONSOLE_HPP_INCLUDED
#define KIWI_APP_CONSOLE_HPP_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "jWindow.hpp"
#include "KiwiConsoleHistory.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                  CONSOLE COMPONENT                               //
    // ================================================================================ //
    
    //! @brief The juce Console Component
    //! @details The juce Console Component maintain a ConsoleHistory and display Console messages to the user.
    //! The user can select a message to copy it to the system clipboard, delete a specific message or a range of messages, sort messages, double-click on a row to hilight the corresponding object...
    class Console :
    public ConsoleHistory::Listener,
    public juce::Component,
    public juce::TableListBoxModel,
    public juce::TableHeaderComponent::Listener
    {
    public:
        
        //! @brief Constructor
        Console(sConsoleHistory history);
        
        //! @brief Destructor
        ~Console();
        
        //! @brief Gets the ConsoleHistory.
        sConsoleHistory getHistory();
        
        //! @brief The function is called by an hisotry when it has changed.
        //! @param history The console history.
        void consoleHistoryChanged(ConsoleHistory const& history) final override;
        
        // ================================================================================ //
        //                                      COMPONENT                                   //
        // ================================================================================ //
        
        void resized() override;
        void paint(juce::Graphics& g) override;
        
        // ================================================================================ //
        //                              TABLE LIST BOX MODEL                                //
        // ================================================================================ //
        
        //! @brief Called when selected rows changed.
        void selectedRowsChanged(int row) override;
        
        //! @brief Called when the delete key has been pressed.
        void deleteKeyPressed(int lastRowSelected) override;
        
        //! @brief Get the number of rows currently displayed by the console
        int getNumRows() override;
        
        //! @brief This is overloaded from TableListBoxModel, and should fill in the background of the whole row
        void paintRowBackground(juce::Graphics& g,
                                int rowNumber, int width, int height,
                                bool rowIsSelected) override;
        
        //! @brief Paint over cells.
        void paintOverChildren(juce::Graphics &g) override;
        
        //! @brief Called when the console background has been clicked (clear row selection).
        void backgroundClicked(const juce::MouseEvent& mouse) override;
        
        //! @brief This must paint any cells that aren't using custom components.
        void paintCell(juce::Graphics& g,
                       int rowNumber, int columnId,
                       int width, int height,
                       bool rowIsSelected) override;
        
        //! @brief This is overloaded from TableListBoxModel,
        //! @details Called when the user has clicked a table header to change the sort order.
        void sortOrderChanged(int newSortColumnId, bool isForwards) override;
        
        //! @brief Called when a cell is double-clicked,
        void cellDoubleClicked(int rowNumber, int columnId, const juce::MouseEvent& mouse) override;
        
        //! @brief This is overloaded from TableListBoxModel
        //! @details Must update any custom components that we're using
        Component* refreshComponentForCell(int rowNumber,
                                           int columnId,
                                           bool isRowSelected,
                                           juce::Component* existingComponentToUpdate) override;
        
        //! @brief This is overloaded from TableListBoxModel.
        //! @details Should choose and return the best width for the specified column.
        int getColumnAutoSizeWidth(int columnId) override;
        
        // ================================================================================ //
        //                          TABLE HEADER COMPONENT LISTENER                         //
        // ================================================================================ //
        
        //! @brief This is called when one or more of the table's columns are resized.
        void tableColumnsResized(juce::TableHeaderComponent* tableHeader) override;
        
        //! @brief This is called when some of the table's columns are added, removed, hidden, or rearranged.
        void tableColumnsChanged(juce::TableHeaderComponent* tableHeader) override {};
	
        //! @brief This is called when the column by which the table should be sorted is changed.
        void tableSortOrderChanged(juce::TableHeaderComponent* tableHeader) override {};

        //! @brief This is called when the rightmost column width need to be updated.
        void updateRighmostColumnWidth(juce::TableHeaderComponent* header);
        
    private:
        
        //! @internal Column type
        enum Column
        {
            Id      = 1,
            Type    = 2,
            Object  = 3,
            Message = 4
        };
        
        //! @internal copy selected rows content.
        void copy();
        
        //! @internal erase selected rows.
        void erase();
        
    private:
        
        wConsoleHistory     m_history;
        juce::Font          m_font;
        juce::TableListBox  m_table;
    };

    // ================================================================================ //
    //                                  CONSOLE WINDOW                                  //
    // ================================================================================ //
    
    //! @brief The ConsoleWindow is a jWindow that manages and display a Console Component.
    class ConsoleWindow : public jWindow
    {
    public:
        ConsoleWindow(sConsoleHistory history);
        ~ConsoleWindow();
        
        void closeButtonPressed() override;
    };
}


#endif // KIWI_APP_CONSOLE_HPP_INCLUDED
