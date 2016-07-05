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

#ifndef KIWI_DOCUMENT_EXPLORER_HPP_INCLUDED
#define KIWI_DOCUMENT_EXPLORER_HPP_INCLUDED

#include "flip/contrib/MulticastServiceExplorer.h"

#include <KiwiCore/KiwiCoreListeners.hpp>

#include "jWindow.hpp"

namespace kiwi
{
    class jInstance;
    
    // ================================================================================ //
    //                                  DOCUMENT EXPLORER                               //
    // ================================================================================ //
    
    //! @brief Explore service running on the Local Area Network.
    class DocumentExplorer : public juce::Timer
    {
    public:
        
        using SessionList = std::list<flip::MulticastServiceExplorer::Session>;
        
        //! @brief Constructor
        DocumentExplorer();
        
        //! @brief Destructor
        ~DocumentExplorer();
        
        //! @brief juce::Timer callback.
        void timerCallback() override;
        
        //! @brief Returs a list of discovered sessions.
        SessionList getSessionList() const;
        
        class Listener;
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief remove a listener.
        void removeListener(Listener& listener);
        
        class Panel;
        
    private: // methods
        
        //! @brief Scan the LAN to find a service provider.
        void process();
        
    private: // members
        
        flip::MulticastServiceExplorer  m_explorer;
        SessionList                     m_sessions;
        kiwi::Listeners<Listener>       m_listeners;
    };
    
    // ================================================================================ //
    //                             DOCUMENT EXPLORER LISTENER                           //
    // ================================================================================ //
    
    //! @brief Listen to document explorer changes.
    class DocumentExplorer::Listener
    {
    public:
        
        //! @brief Destructor.
        virtual ~Listener() = default;
        
        //! @brief Called when the document list changed.
        virtual void documentListChanged() = 0;
    };
    
    // ================================================================================ //
    //                               DOCUMENT EXPLORER PANEL                            //
    // ================================================================================ //
    
    //! @brief Listen to document explorer changes.
    class DocumentExplorer::Panel : public juce::Component,
                                    public DocumentExplorer::Listener,
                                    public juce::Button::Listener
    {
    public:
        
        //! @brief Constructor.
        Panel(DocumentExplorer& explorer, jInstance& instance);
        
        //! @brief Destructor.
        ~Panel();
        
        void resized() override;
        void paint(juce::Graphics& g) override;
        
        //! @brief Called when a juce::Button is clicked.
        void buttonClicked(juce::Button* button) override;
        
        //! @brief Called when the document list changed.
        void documentListChanged() override;
        
    private: // classses
        
        class SessionItemButton : public juce::Button
        {
        public:
            SessionItemButton(flip::MulticastServiceExplorer::Session && session);
            void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
            
            //! @brief Returns the metadata associated to the given key.
            std::string getMetadata(std::string const& key) const;
            
            //! @brief Returns the session port.
            uint16_t getPort() const;
            
            //! @brief Returns the session ip.
            uint32_t getIp() const;
            
            //! @brief Returns the session host.
            std::string getHost() const;
            
        private:
            const flip::MulticastServiceExplorer::Session   m_session;
        };
        
    private: // members
        
        jInstance&                                      m_instance;
        DocumentExplorer&                               m_explorer;
        std::vector<std::unique_ptr<SessionItemButton>> m_buttons;
    };

    // ================================================================================ //
    //                               DOCUMENT EXPLORER WINDOW                           //
    // ================================================================================ //
    
    //! @brief The jConsoleWindow is a jWindow that manages and display a jConsole Component.
    class DocumentExplorerWindow : public jWindow
    {
    public:
        DocumentExplorerWindow(DocumentExplorer& explorer, jInstance& instance);
        ~DocumentExplorerWindow();
        
        void closeButtonPressed() override;
    };
}


#endif // KIWI_DOCUMENT_EXPLORER_HPP_INCLUDED
