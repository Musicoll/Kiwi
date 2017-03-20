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
        
        //! @brief Called when a document session has been added.
        void documentAdded(DocumentBrowser::Drive::DocumentSession const& doc) override;
        
        //! @brief Called when a document session changed.
        void documentChanged(DocumentBrowser::Drive::DocumentSession const& doc) override;
        
        //! @brief Called when a document session has been removed.
        void documentRemoved(DocumentBrowser::Drive::DocumentSession const& doc) override;
        
        //! @brief Returns true if the two drive view refer to the same drive.
        bool operator==(DocumentBrowser::Drive const& other_drive) const;
        
    private: // classes
        
        class BrowserButton : public juce::Button
        {
        public: // methods
            
            //! @brief Constructor.
            BrowserButton(juce::String const& button_text);
            
            //! @brief Destructor.
            ~BrowserButton() = default;
            
            //! @brief draw the button.
            void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
            
            //! @brief This method is called when the button has been clicked.
            void clicked(juce::ModifierKeys const& modifiers) override;
            
            //! @brief Set the command to execute when the button has been clicked.
            void setCommand(std::function<void(void)> fn);
            
        private: // members
            
            std::function<void(void)> m_command;
        };
        
        class DocumentSessionView : public juce::Component
        {
        public: // methods
            
            //! @brief Constructor.
            DocumentSessionView(DocumentBrowser::Drive::DocumentSession const& document);
            
            //! @brief Destructor.
            ~DocumentSessionView() = default;
            
            //! @brief juce::Component::paint
            void paint(juce::Graphics& g) override;
            
            //! @brief Called when resized.
            void resized() override;
            
            //! @brief Returns the metadata associated to the given key.
            std::string getMetadata(std::string const& key, std::string const& ifnotfound = "") const;
            
            //! @brief Returns the session port.
            uint16_t getPort() const;
            
            //! @brief Returns the session host.
            std::string getHost() const;
            
            //! @brief Returns the document name.
            std::string getName() const;
            
            //! @brief Returns the session id.
            uint64_t getSessionId() const;
            
            //! @brief Returns true if the two documents refer to the same session_id
            bool operator==(DocumentBrowser::Drive::DocumentSession const& other_document) const;
            
        private: // members
            
            DocumentBrowser::Drive::DocumentSession const&  m_document;
            BrowserButton                                   m_open_btn;
            juce::Label                                     m_name_label;
        };
        
    private: // methods
        
        //! @internal update layout.
        void updateLayout();
        
    private: // members
        
        DocumentBrowser::Drive&                             m_drive;
        std::vector<std::unique_ptr<DocumentSessionView>>   m_documents = {};
        std::unique_ptr<BrowserButton>                      m_create_document_btn = nullptr;
    };

    // ================================================================================ //
    //                               DOCUMENT BROWSER WINDOW                           //
    // ================================================================================ //
    
    //! @brief The DocumentBrowserWindow is a Window that manages and displays a Document browser.
    class DocumentBrowserWindow : public Window
    {
    public:
        DocumentBrowserWindow(DocumentBrowser& browser);
        ~DocumentBrowserWindow();
        
        void closeButtonPressed() override;
    };
}


#endif // KIWI_APP_DOCUMENT_BROWSER_VIEW_HPP_INCLUDED
