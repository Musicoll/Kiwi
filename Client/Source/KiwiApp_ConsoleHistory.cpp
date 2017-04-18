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

#include "KiwiApp.hpp"
#include "KiwiApp_ConsoleHistory.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                  CONSOLE HISTORY                                 //
    // ================================================================================ //
    
    ConsoleHistory::ConsoleHistory(engine::Instance& instance) :
    m_instance(instance),
    m_sort(ConsoleHistory::ByIndex)
    {
        m_instance.addConsoleListener(*this);
    }
    
    ConsoleHistory::~ConsoleHistory()
    {
        m_instance.removeConsoleListener(*this);
        m_messages.clear();
    }
    
    void ConsoleHistory::newConsoleMessage(engine::Console::Message const& message)
    {
        {
            if(!message.text.empty())
            {
                bool changed = false;
                {
                    std::lock_guard<std::mutex> guard(m_message_mutex);
                    
                    if(!m_messages.empty())
                    {
                        auto& last = m_messages.back();
                        auto& last_message = last.m_message;
                        if(last_message.text == message.text
                           && last_message.type == message.type)
                        {
                            last.m_repeat_times++;
                            changed = true;
                        }
                    }
                    
                    if(!changed)
                    {
                        m_messages.push_back({message, m_messages.size() + 1});
                        changed = true;
                    }
                }
                
                if(changed)
                {
                    m_listeners.call(&Listener::consoleHistoryChanged, *this);
                }
            }
        }
    }
    
    void ConsoleHistory::clear()
    {
        {
            std::lock_guard<std::mutex> guard(m_message_mutex);
            m_messages.clear();
        }
        
        m_listeners.call(&Listener::consoleHistoryChanged, *this);
    }
    
    size_t ConsoleHistory::size()
    {
        return m_messages.size();
    }
    
    std::pair<engine::Console::Message const*, size_t> ConsoleHistory::get(size_t index)
    {
        std::lock_guard<std::mutex> guard(m_message_mutex);
        if(index < m_messages.size())
        {
            auto const& msg = m_messages[index];
            return {&msg.m_message, msg.m_repeat_times};
        }
        
        return {};
    }
    
    void ConsoleHistory::erase(size_t index)
    {
        if(index < m_messages.size())
        {
            {
                std::lock_guard<std::mutex> guard(m_message_mutex);
                m_messages.erase(m_messages.begin() + index);
                std::sort(m_messages.begin(), m_messages.end(), compareIndex);
                
                for(size_t i = 0; i < m_messages.size(); i++)
                {
                    m_messages[i-1].m_index = i;
                }
                
                this->sort(m_sort);
            }
            
            m_listeners.call(&Listener::consoleHistoryChanged, *this);
        }
    }
    
    void ConsoleHistory::erase(size_t begin, size_t last)
    {
        if(begin < last && last < m_messages.size())
        {
            {
                std::lock_guard<std::mutex> guard(m_message_mutex);
                m_messages.erase(m_messages.begin() + begin, m_messages.begin() + last);
                std::sort(m_messages.begin(), m_messages.end(), compareIndex);
                for(size_t i = 0; i < m_messages.size(); i++)
                {
                    m_messages[i].m_index = i+1;
                }
                
                sort(m_sort);
            }
            
            m_listeners.call(&Listener::consoleHistoryChanged, *this);
        }
    }
    
    void ConsoleHistory::erase(std::vector<size_t>& indices)
    {
        size_t max = m_messages.size();
        std::sort(indices.begin(), indices.end());
        std::reverse(indices.begin(), indices.end());
        
        for(size_t i = 0; i < indices.size(); i++)
        {
            if(indices[i] < max)
            {
                std::lock_guard<std::mutex> guard(m_message_mutex);
                m_messages.erase(m_messages.begin() + indices[i]);
                --max;
            }
        }
        
        {
            std::lock_guard<std::mutex> guard(m_message_mutex);
            std::sort(m_messages.begin(), m_messages.end(), compareIndex);
            for(size_t i = 0; i < m_messages.size(); i++)
            {
                m_messages[i].m_index = i+1;
            }
            sort(m_sort);
        }
        
        m_listeners.call(&Listener::consoleHistoryChanged, *this);
    }
    
    bool ConsoleHistory::compareIndex(MessageHolder const& i, MessageHolder const& j)
    {
        return i.m_index < j.m_index;
    }
    
    bool ConsoleHistory::compareText(MessageHolder const& i, MessageHolder const& j)
    {
        std::string first = i.m_message.text;
        std::string second = j.m_message.text;
        
        return first.compare(second);
    }
    
    bool ConsoleHistory::compareType(MessageHolder const& i, MessageHolder const& j)
    {
        engine::Console::Message::Type first = i.m_message.type;
        engine::Console::Message::Type second = j.m_message.type;
        
        return first < second;
    }
    
    void ConsoleHistory::sort(Sort type)
    {
        m_sort = type;
        switch(m_sort)
        {
            case ByIndex:
            { std::sort(m_messages.begin(), m_messages.end(), compareIndex); break; }
                
            case ByType:
            { std::sort(m_messages.begin(), m_messages.end(), compareType); break; }
                
            case ByText:
            { std::sort(m_messages.begin(), m_messages.end(), compareText); break; }
        }
    }
    
    void ConsoleHistory::addListener(ConsoleHistory::Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void ConsoleHistory::removeListener(ConsoleHistory::Listener& listener)
    {
        m_listeners.remove(listener);
    }
}
