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

#ifndef KIWI_CONSOLE_HISTORY_HPP_INCLUDED
#define KIWI_CONSOLE_HISTORY_HPP_INCLUDED

#include <KiwiEngine/KiwiEngineConsole.hpp>

namespace kiwi
{
    // ================================================================================ //
    //                                  CONSOLE HISTORY                                 //
    // ================================================================================ //
    
    //! @brief The Console History listen to the Console and keep an history of the messages.
    //! @details It expose methods to fetch messages sorted by date, message or by type.
    class ConsoleHistory : public engine::Console::Listener
    {
    public:
        class Listener;
        
        //! @brief Sorting method types
        enum Sort
        {
            ByIndex = 0,    ///< Sort messages by Index
            ByType,         ///< Sort message by type
            ByText          ///< Sort message by text
        };
        
    public:
        
        //! @brief Constructor.
        ConsoleHistory();
        
        //! @brief Destructor.
        ~ConsoleHistory();
        
        //! @brief Clear the messages.
        void clear();
        
        //! @brief Retreives the number of messages in the history.
        size_t size();
        
        //! @brief Get a message from the history at a given index.
        engine::Console::Message const* get(size_t index);
        
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
            engine::Console::Message    m_message;
            size_t                      m_index;
        };
        
        static bool compareIndex(MessageHolder const& i, MessageHolder const& j);
        static bool compareText(MessageHolder const& i, MessageHolder const& j);
        static bool compareType(MessageHolder const& i, MessageHolder const& j);
        
        //! @internal Receive the messages from the Console and dispatch changes to listeners.
        void newConsoleMessage(engine::Console::Message const& message) final override;
        
    private:
        
        std::mutex                  m_message_mutex;
        std::vector<MessageHolder>  m_messages;
        Sort                        m_sort;
        engine::Listeners<Listener> m_listeners;
    };
    
    // ================================================================================ //
    //                                  INSTANCE LISTENER                               //
    // ================================================================================ //
    
    //! @brief The Console History Listener is a is a virtual class you can inherit from to receive console history change notifications.
    class ConsoleHistory::Listener
    {
    public:
        
        virtual ~Listener() = default;
        
        //! @brief The function is called by an hisotry when it has changed.
        //! @param history The console history.
        virtual void consoleHistoryChanged(ConsoleHistory const& history) = 0;
    };
}

#endif // KIWI_CONSOLE_HISTORY_HPP_INCLUDED
