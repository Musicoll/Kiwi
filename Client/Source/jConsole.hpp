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

#include <KiwiModel/KiwiConsole.hpp>

#include "../JuceLibraryCode/JuceHeader.h"

#include "jWindow.hpp"

namespace kiwi
{
    
    // ================================================================================ //
    //                                  JCONSOLE COMPONENT                              //
    // ================================================================================ //
    
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
        
        //! Notify that the selection has changed.
        /** The function notifies that the selection has changed.
         */
        void selectedRowsChanged(int row) override;
        
        //! Notify that the delete key has been pressed.
        /** The function notifies that the delete key has been pressed.
         */
        void deleteKeyPressed(int lastRowSelected) override;
        
        //! Retreive the number of rows currently displayed by the console
        /** The function retreives the number of rows currently displayed by the console
         */
        int getNumRows() override;
        
        // This is overloaded from TableListBoxModel, and should fill in the background of the whole row
        void paintRowBackground (Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
        
        void paintOverChildren (Graphics &g) override;
        
        void backgroundClicked (const MouseEvent& mouse) override;
        
        // This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom components.
        void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
        
        // This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header to change the sort order.
        void sortOrderChanged(int newSortColumnId, bool isForwards) override;
        
        void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent& mouse) override;
        
        // This is overloaded from TableListBoxModel, and must update any custom components that we're using
        Component* refreshComponentForCell(int rowNumber,
                                           int columnId,
                                           bool isRowSelected,
                                           Component* existingComponentToUpdate) override;
        
        // This is overloaded from TableListBoxModel, and should choose the best width for the specified
        // column.
        int getColumnAutoSizeWidth(int columnId) override;
        
        // ================================================================================ //
        //                          TABLE HEADER COMPONENT LISTENER                         //
        // ================================================================================ //
        
        /** This is called when one or more of the table's columns are resized. */
        void tableColumnsResized (TableHeaderComponent* tableHeader) override;
        
        /** This is called when some of the table's columns are added, removed, hidden,
         or rearranged.
         */
        void tableColumnsChanged (TableHeaderComponent* tableHeader) override {};
	
        /** This is called when the column by which the table should be sorted is changed. */
        void tableSortOrderChanged (TableHeaderComponent* tableHeader) override {};

        void updateRighmostColumnWidth (TableHeaderComponent* header);
        
    private:
        
        enum Column
        {
            Id      = 1,
            Type    = 2,
            Object  = 3,
            Message = 4
        };
        
        void copy();
        
        void erase();
        
    private:
        
        std::unique_ptr<Console::History>    m_history;
        juce::Font                                  m_font;
        TableListBox                                m_table;
    };

    // ================================================================================ //
    //                                  CONSOLE WINDOW                                  //
    // ================================================================================ //
    
    class jConsoleWindow : public jWindow
    {
    public:
        jConsoleWindow();
        ~jConsoleWindow();
        
        void closeButtonPressed() override;
    };
}


#endif // KIWI_JCONSOLE_HPP_INCLUDED
