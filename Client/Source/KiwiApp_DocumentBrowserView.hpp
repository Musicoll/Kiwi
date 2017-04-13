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

#ifndef KIWI_APP_DOCUMENT_BROWSER_VIEW_HPP_INCLUDED
#define KIWI_APP_DOCUMENT_BROWSER_VIEW_HPP_INCLUDED

#include "flip/contrib/MulticastServiceExplorer.h"

#include <KiwiEngine/KiwiEngine_Listeners.hpp>

#include "KiwiApp_Network/KiwiApp_DocumentBrowser.hpp"
#include "KiwiApp_Window.hpp"

namespace kiwi
{
    class Instance;
    
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
    //                               DOCUMENT BROWSER PANEL                             //
    // ================================================================================ //
    
    //! @brief Listen to document browser changes.
    class DocumentBrowserView::DriveView :
    public juce::Component,
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
        
        //! @brief Called when resized.
        void resized() override;
        
        //! @brief juce::Component::paint
        void paint(juce::Graphics& g) override;
        
        // ========= DocumentBrowser::Drive::Listener
        
        //! @brief Called when a document session has been added.
        void documentAdded(DocumentBrowser::Drive::DocumentSession& doc) override {
            std::cout << "documentAdded" << '\n';};
        
        //! @brief Called when a document session changed.
        void documentChanged(DocumentBrowser::Drive::DocumentSession& doc) override {std::cout << "documentChanged" << '\n';};
        
        //! @brief Called when a document session has been removed.
        void documentRemoved(DocumentBrowser::Drive::DocumentSession& doc) override {std::cout << "documentRemoved" << '\n';};
        
        void driveChanged() override;
        
        // ========= juce::ListBoxModel
        
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
        
        //! @brief Returns true if the two drive view refer to the same drive.
        bool operator==(DocumentBrowser::Drive const& other_drive) const;
        
    private: // classes
        
        class Header;
        class DocumentSessionView;
        
    private: // methods
        
        //! @internal update layout.
        void updateLayout();
        
    private: // members
        
        DocumentBrowser::Drive&                                 m_drive;
        std::vector<juce::ScopedPointer<DocumentSessionView>>   m_documents = {};
        juce::ListBox                                       	m_document_list;
    };
    
    // ================================================================================ //
    //                            DOCUMENT BROWSER IMAGE BUTTON                         //
    // ================================================================================ //
    
    class ImageButton : public juce::DrawableButton
    {
    public: // methods
        
        //! @brief Constructor.
        ImageButton(juce::String const& button_text);
        
        //! @brief Destructor.
        ~ImageButton() = default;
        
        //! @brief This method is called when the button has been clicked.
        void clicked(juce::ModifierKeys const& modifiers) override;
        
        //! @brief Set the command to execute when the button has been clicked.
        void setCommand(std::function<void(void)> fn);
        
    private: // members
        
        std::function<void(void)> m_command;
    };
    
    
    // ================================================================================ //
    //                            DOCUMENT BROWSER PANEL HEADER                         //
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
        
        juce::ListBox&                  m_listbox;
        DocumentBrowser::Drive&         m_drive;
        std::unique_ptr<ImageButton>    m_refresh_btn;
        std::unique_ptr<ImageButton>    m_create_document_btn;
        const juce::Image               m_folder_img;
        std::unique_ptr<juce::Drawable> m_refresh_img;
        std::unique_ptr<juce::Drawable> m_create_img;
    };
    
    // ================================================================================ //
    //                          DOCUMENT BROWSER PANEL SESSION VIEW                     //
    // ================================================================================ //
    
    class DocumentBrowserView::DriveView::DocumentSessionView : public juce::Component, juce::Label::Listener
    {
    public: // methods
        
        //! @brief Constructor.
        DocumentSessionView(juce::ListBox& listbox, DocumentBrowser::Drive::DocumentSession& document);
        
        //! @brief Destructor.
        ~DocumentSessionView();
        
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
        void labelTextChanged(juce::Label* labelThatHasChanged) override;
        
        //! @brief Returns the document name.
        std::string getName() const;
        
        //! @brief Returns the session host.
        std::string getHost() const;
        
        //! @brief Returns the session port.
        uint16_t getSessionPort() const;
        
        //! @brief Returns the session id.
        uint64_t getSessionId() const;
        
        //! @brief Update the document session
        void update(DocumentBrowser::Drive::DocumentSession* doc, int row, bool now_selected);
        
        //! @brief Returns true if the two documents refer to the same session_id
        bool operator==(DocumentBrowser::Drive::DocumentSession const& other_document) const;
        
    private: // methods
        
        //! @internal Called by the DriveView when the document has changed.
        void documentSessionChanged();
        
    private: // variables
        
        juce::ListBox&                              m_listbox;
        DocumentBrowser::Drive::DocumentSession*    m_document = nullptr;
        std::unique_ptr<ImageButton>                m_open_btn;
        juce::Label                                 m_name_label;
        
        const juce::Image                           m_kiwi_filetype_img;
        std::unique_ptr<juce::Drawable>             m_open_img;
        
        int                                         m_row;
        bool                                        m_selected;
        bool                                        m_mouseover = false;
        bool                                        m_select_row_on_mouse_up = false;
        
        friend DriveView;
    };
}


#endif // KIWI_APP_DOCUMENT_BROWSER_VIEW_HPP_INCLUDED
