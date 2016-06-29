/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_JCONSOLE_HPP_INCLUDED
#define KIWI_JCONSOLE_HPP_INCLUDED

#include <KiwiCore/KiwiConsole.hpp>

#include "../JuceLibraryCode/JuceHeader.h"

#include "jWindow.hpp"

namespace kiwi
{
    
    // ================================================================================ //
    //                                  JCONSOLE COMPONENT                              //
    // ================================================================================ //
    
    //! @brief The juce Console Component
    //! @details The juce Console Component maintain a Console::History and display Console messages to the user.
    //! The user can select a message to copy it to the system clipboard, delete a specific message or a range of messages, sort messages, double-click on a row to hilight the corresponding object...
    class jConsole :
    public Console::History::Listener,
    public Component,
    public TableListBoxModel,
    public TableHeaderComponent::Listener
    {
    public:
        
        //! @brief Constructor
        jConsole();
        
        //! @brief Destructor
        ~jConsole();
        
        //! @brief The function is called by an hisotry when it has changed.
        //! @param history The console history.
        void consoleHistoryChanged(Console::History const& history) final override;
        
        // ================================================================================ //
        //                                      COMPONENT                                   //
        // ================================================================================ //
        
        void resized() override;
        void paint(Graphics& g) override;
        
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
        void paintRowBackground(Graphics& g,
                                int rowNumber, int width, int height,
                                bool rowIsSelected) override;
        
        //! @brief Paint over cells.
        void paintOverChildren(Graphics &g) override;
        
        //! @brief Called when the console background has been clicked (clear row selection).
        void backgroundClicked(const MouseEvent& mouse) override;
        
        //! @brief This must paint any cells that aren't using custom components.
        void paintCell(Graphics& g,
                       int rowNumber, int columnId,
                       int width, int height,
                       bool rowIsSelected) override;
        
        //! @brief This is overloaded from TableListBoxModel,
        //! @details Called when the user has clicked a table header to change the sort order.
        void sortOrderChanged(int newSortColumnId, bool isForwards) override;
        
        //! @brief Called when a cell is double-clicked,
        void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent& mouse) override;
        
        //! @brief This is overloaded from TableListBoxModel
        //! @details Must update any custom components that we're using
        Component* refreshComponentForCell(int rowNumber,
                                           int columnId,
                                           bool isRowSelected,
                                           Component* existingComponentToUpdate) override;
        
        //! @brief This is overloaded from TableListBoxModel.
        //! @details Should choose and return the best width for the specified column.
        int getColumnAutoSizeWidth(int columnId) override;
        
        // ================================================================================ //
        //                          TABLE HEADER COMPONENT LISTENER                         //
        // ================================================================================ //
        
        //! @brief This is called when one or more of the table's columns are resized.
        void tableColumnsResized(TableHeaderComponent* tableHeader) override;
        
        //! @brief This is called when some of the table's columns are added, removed, hidden, or rearranged.
        void tableColumnsChanged(TableHeaderComponent* tableHeader) override {};
	
        //! @brief This is called when the column by which the table should be sorted is changed.
        void tableSortOrderChanged(TableHeaderComponent* tableHeader) override {};

        //! @brief This is called when the rightmost column width need to be updated.
        void updateRighmostColumnWidth(TableHeaderComponent* header);
        
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
        
        std::unique_ptr<Console::History>    m_history;
        juce::Font                           m_font;
        TableListBox                         m_table;
    };

    // ================================================================================ //
    //                                  CONSOLE WINDOW                                  //
    // ================================================================================ //
    
    //! @brief The jConsoleWindow is a jWindow that manages and display a jConsole Component.
    class jConsoleWindow : public jWindow
    {
    public:
        jConsoleWindow();
        ~jConsoleWindow();
        
        void closeButtonPressed() override;
    };
}


#endif // KIWI_JCONSOLE_HPP_INCLUDED
