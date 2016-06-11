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

#include <KiwiModel/KiwiConsole.hpp>

#include "DocumentExplorer.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                  DOCUMENT EXPLORER                               //
    // ================================================================================ //
    
    DocumentExplorer::DocumentExplorer()
    {
        startTimer(20);
    }
    
    DocumentExplorer::~DocumentExplorer()
    {
        stopTimer();
    }
    
    DocumentExplorer::SessionList DocumentExplorer::getSessionList() const
    {
        return m_sessions;
    }
    
    void DocumentExplorer::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    //! @brief remove a listener.
    void DocumentExplorer::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    void DocumentExplorer::timerCallback()
    {
        process();
    }
    
    void DocumentExplorer::process()
    {
        //flip::MulticastServiceExplorer explorer;
        
        m_explorer.process();
        
        bool changed = false;
        
        SessionList new_sessions;
        
        for(auto && session : m_explorer)
        {
            new_sessions.push_back(session);
        }
        
        // check if we have a new session
        for(auto const& session : new_sessions)
        {
            const auto it = std::find_if(m_sessions.begin(), m_sessions.end(), [&session](flip::MulticastServiceExplorer::Session const& new_session){
                
                const bool equal = (session.ip == new_session.ip)
                && (session.port == new_session.port)
                && (session.version == new_session.version)
                && (session.metadata == new_session.metadata);
                
                return equal;
            });
            
            if(it == m_sessions.end())
            {
                changed = true;
                break;
            }
        }
        
        if(!changed)
        {
            // check if a session has expired
            for(auto const& session : m_sessions)
            {
                const auto it = std::find_if(new_sessions.begin(), new_sessions.end(), [&session](flip::MulticastServiceExplorer::Session const& old_session){
                    
                    const bool equal = (session.ip == old_session.ip)
                    && (session.port == old_session.port)
                    && (session.version == old_session.version)
                    && (session.metadata == old_session.metadata);
                    
                    return equal;
                });
                
                if(it == new_sessions.end())
                {
                    changed = true;
                    break;
                }
            }
        }
        
        if(changed)
        {
            std::swap(m_sessions, new_sessions);
            
            m_listeners.call(&Listener::documentListChanged);
        }
    }
    
    // ================================================================================ //
    //                               DOCUMENT EXPLORER PANEL                            //
    // ================================================================================ //
    
    DocumentExplorer::Panel::Panel(DocumentExplorer& explorer) :
    m_explorer(explorer)
    {
        m_explorer.addListener(*this);
    }
    
    DocumentExplorer::Panel::~Panel()
    {
        m_explorer.removeListener(*this);
    }
    
    void DocumentExplorer::Panel::resized()
    {
        
    }
    
    void DocumentExplorer::Panel::paint(Graphics& g)
    {
        g.setColour(juce::Colours::lightgrey);
        g.fillAll();
    }
    
    void DocumentExplorer::Panel::documentListChanged()
    {
        for(auto && session : m_explorer.getSessionList())
        {
            auto const& metadata = session.metadata;
            
            for(auto& meta : metadata)
            {
                Console::post("meta[\"" + meta.first + "\"] : " + meta.second);
            }
            
            Console::post("session.ip : " + std::to_string(session.ip));
            Console::post("session.port : " + std::to_string(session.port));
            Console::post("session.version : " + session.version);
        }
    }
    
    // ================================================================================ //
    //                               DOCUMENT EXPLORER WINDOW                           //
    // ================================================================================ //
    
    DocumentExplorerWindow::DocumentExplorerWindow(DocumentExplorer& explorer) :
    jWindow("Document explorer panel",
            Colours::white,
            minimiseButton | closeButton, true)
    {
        setContentOwned(new DocumentExplorer::Panel(explorer), false);
        setResizable(true, false);
        setResizeLimits(50, 50, 32000, 32000);
        setTopLeftPosition(0, 0);
        setSize(300, 440);
        setVisible(true);
    }
    
    DocumentExplorerWindow::~DocumentExplorerWindow()
    {
        ;
    }
    
    void DocumentExplorerWindow::closeButtonPressed()
    {
        setVisible(false);
    }
}