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

#include "KiwiConsole.hpp"

namespace kiwi
{
    std::mutex                      Console::m_ostream_mutex;
    Listeners<Console::Listener>    Console::m_listeners;
    
    // ================================================================================ //
    //                                      CONSOLE                                     //
    // ================================================================================ //
    
    void Console::post(std:: string const& message)
    {
        output(MessageType::Post, flip::Ref::null, std::cout, message);
    }
    
    //! @brief Converts an Atom into string and print it in the console.
    void Console::postAtom(Atom const& atom)
    {
        output(MessageType::Post, flip::Ref::null, std::cout, AtomHelper::toString(atom));
    }
    
    //! @brief Converts a vector of Atom into string and print it in the console.
    void Console::postAtoms(std::vector<Atom> const& atoms)
    {
        output(MessageType::Post, flip::Ref::null, std::cout, AtomHelper::toString(atoms));
    }
    
    //! @brief Print a warning-type message in the console.
    void Console::warning(std:: string const& message)
    {
        output(MessageType::Warning, flip::Ref::null, std::cout, message);
    }
    
    //! @brief Print an error-type message in the console.
    void Console::error(std:: string const& message)
    {
        output(MessageType::Error, flip::Ref::null, std::cerr, message);
    }
    
    void Console::output(MessageType type, flip::Ref ref,
                         std::ostream& output_stream, std::string const& message)
    {
        Message msg { message, type, ref };
        
        m_listeners.call(&Listener::newConsoleMessage, msg);
        
        std::lock_guard<std::mutex> guard(m_ostream_mutex);
        output_stream << message << '\n';
    }
    
    void Console::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void Console::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    Console::Message::Message(std::string text, Console::MessageType type, flip::Ref ref) :
    m_text(text),
    m_type(type),
    m_ref(ref)
    {
        ;
    }
    
    Console::Message::Message(Message const& other) :
    m_text(other.m_text),
    m_type(other.m_type),
    m_ref(other.m_ref)
    {
        ;
    }
    
    // ================================================================================ //
    //                                  CONSOLE HISTORY                                 //
    // ================================================================================ //
    
    Console::History::History() :
    m_sort(Console::History::ByIndex)
    {
        ;
    }
    
    Console::History::~History()
    {
        m_messages.clear();
    }
    
    std::unique_ptr<Console::History> Console::History::create()
    {
        auto history = std::unique_ptr<Console::History>(new History());
        
        Console::addListener(*history.get());
        
        return std::move(history);
    }
    
    void Console::History::newConsoleMessage(Console::Message const& message)
    {
        {
            std::lock_guard<std::mutex> guard(m_message_mutex);
            size_t index = m_messages.size() + 1;
            m_messages.push_back({message, index});
        }
        
        m_listeners.call(&Listener::consoleHistoryChanged, *this);
    }
    
    void Console::History::clear()
    {
        {
            std::lock_guard<std::mutex> guard(m_message_mutex);
            m_messages.clear();
        }
        
        m_listeners.call(&Listener::consoleHistoryChanged, *this);
    }
    
    size_t Console::History::size()
    {
        return m_messages.size();
    }
    
    Console::Message const* Console::History::get(size_t index)
    {
        std::lock_guard<std::mutex> guard(m_message_mutex);
        if(index < m_messages.size())
        {
            return &m_messages[index].m_message;
        }
        else
        {
            return nullptr;
        }
    }
    
    void Console::History::erase(size_t index)
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
    
    void Console::History::erase(size_t begin, size_t last)
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
    
    void Console::History::erase(std::vector<size_t>& indices)
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
    
    bool Console::History::compareIndex(MessageHolder const& i, MessageHolder const& j)
    {
        return i.m_index < j.m_index;
    }
    
    bool Console::History::compareText(MessageHolder const& i, MessageHolder const& j)
    {
        std::string first = i.m_message.getText();
        std::string second = j.m_message.getText();
        
        return first.compare(second);
    }
    
    bool Console::History::compareType(MessageHolder const& i, MessageHolder const& j)
    {
        Console::MessageType first = i.m_message.getType();
        Console::MessageType second = j.m_message.getType();

        return first < second;
    }
    
    void Console::History::sort(Sort type)
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
    
    void Console::History::addListener(History::Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    
    void Console::History::removeListener(History::Listener& listener)
    {
        m_listeners.remove(listener);
    }
}
