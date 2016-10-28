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

#include <KiwiEngine/KiwiEngine_Console.hpp>

#include "KiwiApp_DocumentExplorer.hpp"
#include "KiwiApp_Instance.hpp"

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
    
    void DocumentExplorer::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    std::string DocumentExplorer::getSessionMetadata(flip::MulticastServiceExplorer::Session const& session,
                                                     std::string const& key,
                                                     std::string const& notfound)
    {
        assert(!key.empty());
        
        auto const& metadata = session.metadata;
        const auto it = metadata.find(key);
        return (it != metadata.end()) ? it->second : notfound;
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
    
    DocumentExplorer::Panel::SessionItemButton::SessionItemButton(flip::MulticastServiceExplorer::Session session) :
    juce::Button(""),
    m_session(std::move(session))
    {
        setButtonText(getMetadata("name", "no name"));
    }
    
    void DocumentExplorer::Panel::SessionItemButton::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
    {
        const auto bounds = getLocalBounds();
        
        const juce::Colour bgcolor = isMouseOverButton ? juce::Colour(0xFFE0CA3C) : juce::Colour(0xFF5A54C4);
        g.setColour(bgcolor);
        
        g.fillRoundedRectangle(bounds.reduced(1).toFloat(), 0);
        
        g.setColour(bgcolor.darker(0.8));
        g.drawRoundedRectangle(bounds.reduced(1).toFloat(), 0, 1);
        
        const juce::Colour text_color = isMouseOverButton ? juce::Colour(0xFF000000) : juce::Colour(0xFFFFFFFF);
        g.setColour(text_color);
        g.drawFittedText(getButtonText(), bounds, juce::Justification::centred, 2);
    }
    
    std::string DocumentExplorer::Panel::SessionItemButton::getMetadata(std::string const& key,
                                                                        std::string const& notfound) const
    {
        return DocumentExplorer::getSessionMetadata(m_session, key, notfound);
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
    
    uint64_t DocumentExplorer::Panel::SessionItemButton::getSessionId() const
    {
        uint64_t session_id = 12345;
        
        try
        {
            session_id = std::stoull(getMetadata("session_id"));
        }
        catch(std::exception const& e)
        {
            std::cerr << "fails to read session_id metadata, returns default value" << '\n';
        }
        
        return session_id;
    }
    
    DocumentExplorer::Panel::Panel(DocumentExplorer& explorer, Instance& instance) :
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
        
        const juce::Point<int> padding(10, 5);
        const int button_width = bounds.getWidth() - padding.x * 2;
        const int button_height = 30;
        juce::Point<int> last_top_left_pos(padding.x, 30);
        
        for(auto& button_uptr : m_buttons)
        {
            juce::Rectangle<int> new_bounds
            {
                padding.x,
                last_top_left_pos.y + padding.y,
                button_width, button_height
            };
            
            button_uptr->setBounds(new_bounds);
            
            last_top_left_pos = new_bounds.getBottomLeft();
        }
    }
    
    void DocumentExplorer::Panel::paint(juce::Graphics& g)
    {
        const juce::Colour color(0xFF3F3B4E);
        
        g.setColour(color);
        g.fillAll();
        
        g.setColour(color.brighter(0.1));
        g.fillRoundedRectangle(getLocalBounds().reduced(8).toFloat(), 5);
        
        g.setColour(juce::Colours::whitesmoke);
        g.drawFittedText("Server Host : " + m_hostname,
                         getLocalBounds().reduced(12, 8).removeFromTop(22),
                         juce::Justification::centredLeft, 1);
        
        g.setColour(color);
        g.drawLine(0, 30, getWidth(), 30, 3);
    }
    
    void DocumentExplorer::Panel::buttonClicked(juce::Button* button)
    {        
        auto const* session_item = dynamic_cast<SessionItemButton*>(button);
        if(session_item)
        {
            m_instance.openRemotePatcher(session_item->getHost(),
                                         session_item->getPort(),
                                         session_item->getSessionId());
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
        m_hostname.clear();
        
        const auto bounds = getLocalBounds();
        
        int counter = 0;
        for(auto session : m_explorer.getSessionList())
        {
            session.metadata["name"] = "new document";
            session.metadata["session_id"] = getSessionMetadata(session, "new_session_id");
            
            auto button_it = m_buttons.emplace(m_buttons.end(), std::make_unique<SessionItemButton>(session));
            
            SessionItemButton& button = *button_it->get();
            button.addListener(this);
            
            button.setBounds(10, 10 + (counter*30), bounds.getWidth() - 20, 30);
            addAndMakeVisible(button);
            resized();
            counter++;
            
            juce::String files = getSessionMetadata(session, "backend_files_list");
            juce::var json_files;
            if(juce::JSON::parse(files, json_files).wasOk())
            {
                if(json_files.isArray())
                {
                    const auto files_array = json_files.getArray();
                    for(int i = 0; i < files_array->size(); ++i)
                    {
                        if(files_array->getReference(i).isObject())
                        {
                            auto file_obj = files_array->getReference(i).getDynamicObject();
                            
                            session.metadata["name"] = file_obj->getProperty("name").toString().toStdString();
                            session.metadata["session_id"] = file_obj->getProperty("session_id").toString().toStdString();;
                            
                            auto button_it = m_buttons.emplace(m_buttons.end(), std::make_unique<SessionItemButton>(session));
                            
                            SessionItemButton& button = *button_it->get();
                            button.addListener(this);
                            
                            button.setBounds(10, 10 + (counter*30), bounds.getWidth() - 20, 30);
                            addAndMakeVisible(button);
                            resized();
                            counter++;
                        }
                    }
                }
            }
            
            m_hostname = getSessionMetadata(session, "computer_name");
        }
    }
    
    // ================================================================================ //
    //                               DOCUMENT EXPLORER WINDOW                           //
    // ================================================================================ //
    
    DocumentExplorerWindow::DocumentExplorerWindow(DocumentExplorer& explorer, Instance& instance) :
    Window("Document explorer panel", juce::Colours::white, minimiseButton | closeButton, true)
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
