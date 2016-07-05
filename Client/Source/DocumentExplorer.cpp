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

#include <KiwiCore/KiwiCoreConsole.hpp>

#include "DocumentExplorer.hpp"
#include "jInstance.hpp"

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
    
    DocumentExplorer::Panel::SessionItemButton::SessionItemButton(flip::MulticastServiceExplorer::Session && session) :
    juce::Button(""),
    m_session(session)
    {
        auto const& metadata = session.metadata;
        const auto it = metadata.find("name");
        const std::string name = (it != metadata.end()) ? it->second : "Name error";
        setButtonText(getMetadata("name"));
    }
    
    void DocumentExplorer::Panel::SessionItemButton::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
    {
        const auto bounds = getLocalBounds();
        
        const juce::Colour bgcolor = isMouseOverButton ? juce::Colour(0xFFE0CA3C) : juce::Colour(0xFF5A54C4);
        g.setColour(bgcolor);
        
        g.fillRoundedRectangle(bounds.reduced(1).toFloat(), 5);
        
        g.setColour(bgcolor.darker(0.8));
        g.drawRoundedRectangle(bounds.reduced(1).toFloat(), 5, 1);
        
        const juce::Colour text_color = isMouseOverButton ? juce::Colour(0xFF000000) : juce::Colour(0xFFFFFFFF);
        g.setColour(text_color);
        g.drawFittedText(getButtonText(), bounds, juce::Justification::centred, 2);
    }
    
    std::string DocumentExplorer::Panel::SessionItemButton::getMetadata(std::string const& key) const
    {
        assert(!key.empty());
        
        auto const& metadata = m_session.metadata;
        const auto it = metadata.find(key);
        return (it != metadata.end()) ? it->second : "";
    }
    
    uint16_t DocumentExplorer::Panel::SessionItemButton::getPort() const
    {
        return m_session.port;
    }
    
    uint32_t DocumentExplorer::Panel::SessionItemButton::getIp() const
    {
        return m_session.ip;
    }
    
    std::string DocumentExplorer::Panel::SessionItemButton::getHost() const
    {
        const uint32_t ip_address = getIp();
        char host[16];
        
        if(ip_address)
        {
            snprintf(host, sizeof host, "%u.%u.%u.%u",
                     (ip_address & 0xff000000) >> 24,
                     (ip_address & 0x00ff0000) >> 16,
                     (ip_address & 0x0000ff00) >> 8,
                     (ip_address & 0x000000ff));
        }
        
        return host;
    }
    
    DocumentExplorer::Panel::Panel(DocumentExplorer& explorer, jInstance& instance) :
    m_instance(instance),
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
        const auto bounds = getLocalBounds();
        
        const int padding = 10;
        const int button_width = bounds.getWidth() - padding * 2;
        const int button_height = 30;
        juce::Point<int> last_top_left_pos(0, 0);
        
        for(auto& button_uptr : m_buttons)
        {
            juce::Rectangle<int> new_bounds
            {
                last_top_left_pos.x + padding,
                last_top_left_pos.y + padding,
                button_width, button_height
            };
            
            button_uptr->setBounds(new_bounds);
            
            last_top_left_pos = new_bounds.getBottomLeft();
        }
    }
    
    void DocumentExplorer::Panel::paint(juce::Graphics& g)
    {
        g.setColour(juce::Colour(0xFF3F3B4E));
        g.fillAll();
    }
    
    void DocumentExplorer::Panel::buttonClicked(juce::Button* button)
    {        
        const auto* session_item = dynamic_cast<SessionItemButton*>(button);
        if(session_item)
        {
            uint16_t port = session_item->getPort();
            std::string host = session_item->getHost();
            m_instance.openRemotePatcher(host, port);
        }
    }
    
    void DocumentExplorer::Panel::documentListChanged()
    {
        // refresh all buttons
        for(auto& button_uptr : m_buttons)
        {
            button_uptr->removeListener(this);
        }
        
        removeAllChildren();
        m_buttons.clear();
        
        const auto bounds = getLocalBounds();
        
        int counter = 0;
        for(auto && session : m_explorer.getSessionList())
        {
            auto button_it = m_buttons.emplace(m_buttons.end(), std::make_unique<SessionItemButton>(std::move(session)));
            
            SessionItemButton& button = *button_it->get();
            button.addListener(this);
            
            button.setBounds(10, 10, bounds.getWidth() - 20, 30);
            addAndMakeVisible(button);
            
            counter++;
        }
    }
    
    // ================================================================================ //
    //                               DOCUMENT EXPLORER WINDOW                           //
    // ================================================================================ //
    
    DocumentExplorerWindow::DocumentExplorerWindow(DocumentExplorer& explorer, jInstance& instance) :
    jWindow("Document explorer panel", juce::Colours::white, minimiseButton | closeButton, true)
    {
        setContentOwned(new DocumentExplorer::Panel(explorer, instance), false);
        setResizable(true, false);
        setResizeLimits(50, 50, 32000, 32000);
        
        const juce::String windowState(getGlobalProperties().getValue("document_explorer_window"));
        
        if(windowState.isNotEmpty())
        {
            restoreWindowStateFromString(windowState);
        }
        else
        {
            setTopLeftPosition(10, 10);
            setSize(300, 440);
            setVisible(true);
        }
    }
    
    DocumentExplorerWindow::~DocumentExplorerWindow()
    {
        getGlobalProperties().setValue("document_explorer_window", getWindowStateAsString());
    }
    
    void DocumentExplorerWindow::closeButtonPressed()
    {
        setVisible(false);
    }
}