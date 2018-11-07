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

#include <KiwiTool/KiwiTool_Listeners.h>

#include "../KiwiApp_Network/KiwiApp_DocumentBrowser.h"
#include "../KiwiApp_Components/KiwiApp_ImageButton.h"

namespace kiwi
{
    // ================================================================================ //
    //                               DOCUMENT BROWSER VIEW                              //
    // ================================================================================ //
    
    class DocumentBrowserView : public juce::Component
    {
    public: // methods
        
        //! @brief Constructor.
        DocumentBrowserView(DocumentBrowser& browser, bool enabled);
        
        //! @brief Destructor.
        ~DocumentBrowserView();
        
        //! @brief Called when resized.
        void resized() override;
        
        //! @brief juce::Component::paint
        void paint(juce::Graphics& g) override;
        
    private: // methods
        
        void enablementChanged() override final;
        
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
    class DocumentBrowserView::DriveView
    : public juce::ListBox
    , public juce::ListBoxModel
    , public DocumentBrowser::Drive::Listener
    {
    private: // classes
        
        enum SortBy
        {
            name,
            author,
            creationTime,
            openedTime
        };
        
        struct Comp
        {
            bool compare(DocumentBrowser::Drive::DocumentSession const& lhs,
                         DocumentBrowser::Drive::DocumentSession const& rhs) const;
            
            SortBy  m_type = SortBy::creationTime;
            bool    m_trashed_first = false;
        };
        
    public: // methods
        
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
        
        //! @brief Opens document for the given row.
        void openDocument(int row);
        
        //! @brief Make an API call to duplicate the document on server side.
        void duplicateDocumentForRow(int row);
        
        //! @brief Make an API call to rename the remote document
        void renameDocumentForRow(int row, std::string const& new_name);
        
        //! @brief Makes an API call to upload a document.
        void uploadDocument();
        
        //! @brief Makes an API call to download the remote document.
        void downloadDocumentForRow(int row);
        
        //! @brief Moves a document to trash.
        void deleteDocumentForRow(int row);
        
        //! @brief Restore a trashed document.
        void restoreDocumentForRow(int row);
        
    private: // methods
        
        //! @brief Hides document list and disable some interactions.
        void enablementChanged() override;
        
        //! @brief Resort content and call update content.
        void update();
        
        //! @brief Returns the drive name.
        std::string const& getDriveName() const;
        
        //! @brief Refresh document list.
        void refresh();
        
        //! @brief Creates a new document.
        void createDocument();
        
        //! @brief Returns the current sorting parameter.
        SortBy getSortType() const;
        
        //! @brief Changes the sort parameter and sorts.
        void setSortType(SortBy sort_type);
        
        // @brief Set the trash mode.
        void setTrashMode(bool trash_mode);
        
        //! @brief Get current mode trash or default.
        bool isShowingTrashedDocuments() const;
        
        //! @brief Creates document info tooltip.
        std::string createDocumentToolTip(DocumentBrowser::Drive::DocumentSession const& doc);
        
        //! @brief Save the state of the component
        void saveState();
        
        //! @brief Restore the state of the component
        void restoreState();
        
    private: // classes
        
        class Header;
        class RowElem;
        
    private: // members
        
        DocumentBrowser::Drive& m_drive;
        std::string             m_name;
        bool                    m_trash_mode;
        bool                    m_enabled;
        Comp                    m_sorter;
    };
    
    // ================================================================================ //
    //                             BROWSER DRIVE VIEW HEADER                            //
    // ================================================================================ //
    
    class DocumentBrowserView::DriveView::Header : public juce::Component
    {
    public: // methods
        
        //! @brief Constructor
        Header(DocumentBrowserView::DriveView& drive_view);
        
        //! @brief Destructor
        ~Header() = default;
        
        //! @brief juce::Component::paint
        void paint(juce::Graphics& g) override;
        
        //! @brief juce::Component::resized
        void resized() override;
        
        //! @brief juce::Component::mouseDown
        void mouseDown(juce::MouseEvent const& event) override;
        
        //! @brief Sets the text diaplyed by the header bar.
        void setText(std::string const& text);
        
    private: // methods
        
        void enablementChanged() override final;
        
    private: // members
        
        DocumentBrowserView::DriveView& m_drive_view;
        ImageButton                     m_refresh_btn;
        ImageButton                     m_create_document_btn;
        ImageButton                     m_trash_btn;
        juce::Rectangle<int>            m_folder_bounds;
        juce::Label                     m_label;
        juce::Image                     m_folder_img;
        juce::Image                     m_disable_folder_img;
    };
    
    // ================================================================================ //
    //                            BROWSER DRIVE VIEW ROW ELEM                           //
    // ================================================================================ //
    
    class DocumentBrowserView::DriveView::RowElem : public juce::Component,
                                                    public juce::SettableTooltipClient,
                                                    public juce::Label::Listener
    {
    public: // methods
        
        //! @brief Constructor.
        RowElem(DriveView& drive_view, std::string const& name, std::string const& tooltip);
        
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
        void update(std::string const& name, std::string const& tooltip, int row, bool now_selected);
        
    private: // methods
        
        void showPopup();
        
    private: // variables
        
        DriveView&          m_drive_view;
        std::string         m_name;
        ImageButton         m_open_btn;
        juce::Label         m_name_label;
        
        const juce::Image   m_kiwi_filetype_img;
        
        int                 m_row = -1;
        bool                m_selected;
        bool                m_mouseover = false;
    };
}
