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

#ifndef KIWI_CONSOLE_HPP_INCLUDED
#define KIWI_CONSOLE_HPP_INCLUDED

#include <iostream>
#include <sstream>
#include <mutex>
#include <string>
#include <set>
#include <algorithm>

#include "flip/Ref.h"

#include <KiwiCore/KiwiAtom.hpp>
#include <KiwiCore/KiwiListeners.hpp>

namespace kiwi
{
    // ================================================================================ //
    //                                      CONSOLE                                     //
    // ================================================================================ //
    
    //! @brief The console is an interface that let you print messages in a thread-safe way.
    class Console
    {
    public:

        //! @brief Type of message
        enum MessageType
        {
            Empty = 0,
            Post,
            PostRef,
            Error,
            ErrorRef,
            Warning,
            WarningRef
        };
        
        //! @brief The Console Message owns the informations of a message posted via a console.
        class Message
        {
        public:
            
            Message(std::string content, Console::MessageType type, flip::Ref ref);
            Message(Message const& other);
            
            std::string const& getText() const      { return m_text; };
            Console::MessageType getType() const    { return m_type; };
            flip::Ref getRef() const                { return m_ref; };
            
        private:
            std::string           m_text;
            Console::MessageType  m_type;
            flip::Ref             m_ref;
        };
        
        //! @brief Print a post-type message in the console.
        static void post(std:: string const& message);
        
        //! @brief Converts an Atom into string and print it in the console.
        static void postAtom(Atom const& atom);
        
        //! @brief Converts a vector of Atom into string and print it in the console.
        static void postAtoms(std::vector<Atom> const& atoms);
        
        //! @brief Print a warning-type message in the console.
        static void warning(std:: string const& message);
        
        //! @brief Print an error-type message in the console.
        static void error(std:: string const& message);
        
        class History;
        
    private:
        
        //! @internal send message to listeners and append it to the ostream parameter value.
        static void output(MessageType type, flip::Ref ref,
                           std::ostream& output_stream, std::string const& message);
        
        //! @brief You can inherit from this class to receive console changes.
        class Listener
        {
        public:
            virtual ~Listener() = default;
            
            //! Receive the Console messages.
            virtual void newConsoleMessage(Console::Message const& message) = 0;
        };
        
        //! @brief Add an history listener.
        void addListener(Listener& listener);
        
        //! @brief Remove an history listener.
        void removeListener(Listener& listener);
        
        Console() = delete;
        
    private:
        
        static std::mutex           m_ostream_mutex;
        static Listeners<Listener>  m_listeners;
    };
    
    // ================================================================================ //
    //                                  CONSOLE HISTORY                                 //
    // ================================================================================ //
    
    //! @brief The Console History listen to the Console and keep an history of the messages.
    //! @details It expose methods to fetch messages sorted by date, message or by type.
    class Console::History : public Console::Listener
    {
    public:
        class Listener;
        
        //! @brief Sorting method types
        enum Sort
        {
            ByIndex = 0,
            ByType,
            ByContent
        };
        
    public:
        
        //! @brief Constructor.
        History();
        
        //! @brief Destructor.
        ~History();
        
        //! @brief Clear the messages.
        void clear();
        
        //! @brief Retreives the number of messages in the history.
        size_t size();
        
        //! @brief Get a message from the history at a given index.
        Console::Message const* get(size_t index);
        
        //! @brief Erase a message from the history.
        //! @param index The index of the message.
        void erase(size_t index);
        
        //! @brief Erase a range of messages from the history.
        //! @param begin The index of the message of the first message.
        //! @param last The index of the message of the last message.
        void erase(size_t begin, size_t last);
        
        //! @brief Erase a set of messages from the history.
        //! @param indices The indices of the messages
        void erase(std::vector<size_t>& indices);
        
        //! @brief Sort the messages by index, type or text.
        //! @param sort The type of sorting method.
        //! @see History::Sort
        void sort(Sort type = ByIndex);
        
        //! @brief Add an history listener.
        void addListener(Listener& listener);
        
        //! @brief Remove an history listener.
        void removeListener(Listener& listener);
        
    private:
        
        struct MessageHolder
        {
            Console::Message m_message;
            size_t           m_index;
        };
        
        static bool compareIndex(MessageHolder const& i, MessageHolder const& j);
        static bool compareContent(MessageHolder const& i, MessageHolder const& j);
        static bool compareType(MessageHolder const& i, MessageHolder const& j);
        
        //! @internal Receive the messages from the Console and dispatch changes to listeners.
        void newConsoleMessage(Console::Message const& message) final override;
        
    private:
        
        std::mutex                  m_message_mutex;
        std::vector<MessageHolder>  m_messages;
        Sort                        m_sort;
        Listeners<Listener>         m_listeners;
    };
    
    // ================================================================================ //
    //                                  INSTANCE LISTENER                               //
    // ================================================================================ //
    
    //! The Console History Listener is a is a virtual class you can inherit from to receive console history change notifications.
    class Console::History::Listener
    {
    public:
        
        virtual ~Listener() = default;
        
        //! @brief The function is called by an hisotry when it has changed.
        //! @param history The console history.
        virtual void consoleHistoryChanged(History const& history) {}
    };
}

#endif // KIWI_CONSOLE_HPP_INCLUDED
