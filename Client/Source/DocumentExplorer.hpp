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

#ifndef KIWI_DOCUMENT_EXPLORER_HPP_INCLUDED
#define KIWI_DOCUMENT_EXPLORER_HPP_INCLUDED

#include "flip/contrib/MulticastServiceExplorer.h"

#include <KiwiCore/KiwiListeners.hpp>

#include "jWindow.hpp"

namespace kiwi
{
    
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
        
        //! @brief Called when
        virtual void documentListChanged() = 0;
    };
    
    // ================================================================================ //
    //                               DOCUMENT EXPLORER PANEL                            //
    // ================================================================================ //
    
    //! @brief Listen to document explorer changes.
    class DocumentExplorer::Panel : public DocumentExplorer::Listener,
                                    public juce::Component
    {
    public:
        
        //! @brief Constructor.
        Panel(DocumentExplorer& explorer);
        
        //! @brief Destructor.
        ~Panel();
        
        void resized() override;
        void paint(Graphics& g) override;
        
        //! @brief Called when
        void documentListChanged() override;
        
    private: // members
        
        DocumentExplorer&           m_explorer;
        std::vector<juce::Button>   m_documents;
    };

    // ================================================================================ //
    //                               DOCUMENT EXPLORER WINDOW                           //
    // ================================================================================ //
    
    //! @brief The jConsoleWindow is a jWindow that manages and display a jConsole Component.
    class DocumentExplorerWindow : public jWindow
    {
    public:
        DocumentExplorerWindow(DocumentExplorer& explorer);
        ~DocumentExplorerWindow();
        
        void closeButtonPressed() override;
    };
}


#endif // KIWI_DOCUMENT_EXPLORER_HPP_INCLUDED
