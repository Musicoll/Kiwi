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

#pragma once

#include <KiwiEngine/KiwiEngine_Listeners.h>

#include "../KiwiApp_Network/KiwiApp_DocumentBrowser.h"
#include "../KiwiApp_Components/KiwiApp_ImageButton.h"

namespace kiwi
{
    // ================================================================================ //
    //                               DOCUMENT BROWSER VIEW                              //
    // ================================================================================ //
    
    class DocumentBrowserView : public juce::Component, public DocumentBrowser::Listener
    {
    public: // methods
        
        //! @brief Constructor.
        DocumentBrowserView(DocumentBrowser& browser);
        
        //! @brief Destructor.
        ~DocumentBrowserView();
        
        //! @brief Called when the document list changed.
        void driveAdded(DocumentBrowser::Drive& drive) override;
        
        //! @brief Called when a drive changed.
        void driveChanged(DocumentBrowser::Drive const& drive) override;
        
        //! @brief Called when the document list changed.
        void driveRemoved(DocumentBrowser::Drive const& drive) override;
        
        //! @brief Called when resized.
        void resized() override;
        
        //! @brief juce::Component::paint
        void paint(juce::Graphics& g) override;
        
    private: // nested classes
        
        class DriveView;
        
    private: // members
        
        DocumentBrowser&                        m_browser;
        std::vector<std::unique_ptr<DriveView>> m_drives;
    };
    
    // ================================================================================ //
    //                                 BROWSER DRIVE VIEW                               //
    // ================================================================================ //
    
    //! @brief Listen to document browser changes.
    class DocumentBrowserView::DriveView :
    public juce::ListBox,
    public juce::ListBoxModel,
    public DocumentBrowser::Drive::Listener
    {
    public:
        
        //! @brief Constructor.
        DriveView(DocumentBrowser::Drive& drive);
        
        //! @brief Destructor.
        ~DriveView();
        
        //! @brief Returns the session host name.
        std::string getHostName() const;
        
        //! @brief Called by the DocumentBrowser::Drive changed.
        //! @details Called when one or more document has been changed / removed or added.
        void driveChanged() override;
        
        //! @brief Returns the number of items in the list.
        int getNumRows() override;
        
        //! @brief Draw a row of the list.
        //! @details Note that the rowNumber value may be greater than the number of rows in your
        //! list, so be careful that you don't assume it's less than getNumRows().
        void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool selected) override;
        
        //! @brief Used to create or update a custom component to go in a row of the list.
        juce::Component* refreshComponentForRow(int row, bool selected,
                                                juce::Component* component_to_update) override;
        
        //! @brief Called when the user clicking on a part of the list where there are no rows.
        void backgroundClicked(juce::MouseEvent const& e) override;
        
        //! @brief Called when the return key is pressed.
        void returnKeyPressed(int last_row_selected) override;
        
        //! @brief Called when the user double-clicking on a row.
        void listBoxItemDoubleClicked(int row, juce::MouseEvent const& e) override;
        
        //! @brief Returns true if the two drive view refer to the same drive.
        bool operator==(DocumentBrowser::Drive const& other_drive) const;
        
        //! @brief Opens document for the given row.
        void openDocument(int row);
        
        //! @brief Make an API call to rename the remote document
        void renameDocumentForRow(int row, std::string const& new_name);
        
    private: // classes
        
        class Header;
        class RowElem;
        
    private: // members
        
        DocumentBrowser::Drive& m_drive;
    };
    
    // ================================================================================ //
    //                             BROWSER DRIVE VIEW HEADER                            //
    // ================================================================================ //
    
    class DocumentBrowserView::DriveView::Header : public juce::Component
    {
    public: // methods
        
        //! @brief Constructor
        Header(juce::ListBox& listbox, DocumentBrowser::Drive& drive);
        
        //! @brief Destructor
        ~Header() = default;
        
        //! @brief juce::Component::paint
        void paint(juce::Graphics& g) override;
        
        //! @brief juce::Component::resized
        void resized() override;
        
        //! @brief juce::Component::mouseDown
        void mouseDown(juce::MouseEvent const& event) override;
        
    private: // members
        
        juce::ListBox&              m_listbox;
        DocumentBrowser::Drive&     m_drive;
        ImageButton                 m_refresh_btn;
        ImageButton                 m_create_document_btn;
        const juce::Image           m_folder_img;
    };
    
    // ================================================================================ //
    //                            BROWSER DRIVE VIEW ROW ELEM                           //
    // ================================================================================ //
    
    class DocumentBrowserView::DriveView::RowElem : public juce::Component, juce::Label::Listener
    {
    public: // methods
        
        //! @brief Constructor.
        RowElem(DriveView& drive_view, std::string const& name);
        
        //! @brief Destructor.
        ~RowElem();
        
        //! @brief Show the document name editor.
        void showEditor();
        
        //! @brief juce::Component::paint
        void paint(juce::Graphics& g) override;
        
        //! @brief Called when resized.
        void resized() override;
        
        //! @brief juce::Component::mouseEnter
        void mouseEnter(juce::MouseEvent const& event) override;
        
        //! @brief juce::Component::mouseExit
        void mouseExit(juce::MouseEvent const& event) override;
        
        //! @brief juce::Component::mouseDown
        void mouseDown(juce::MouseEvent const& event) override;
        
        //! @brief juce::Component::mouseUp
        void mouseUp(juce::MouseEvent const& event) override;
        
        //! @brief juce::Component::mouseDoubleClick
        void mouseDoubleClick(juce::MouseEvent const& event) override;
        
        //! @brief Called when a Label's text has changed.
        void labelTextChanged(juce::Label* label_text_that_has_changed) override;
        
        //! @brief Update the document session
        void update(std::string const& name, int row, bool now_selected);
        
    private: // variables
        
        DriveView&          m_drive_view;
        std::string         m_name;
        ImageButton         m_open_btn;
        juce::Label         m_name_label;
        
        const juce::Image   m_kiwi_filetype_img;
        
        int                 m_row;
        bool                m_selected;
        bool                m_mouseover = false;
        bool                m_select_row_on_mouse_up = false;
    };
}
