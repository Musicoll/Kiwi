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

#ifndef KIWI_LISTENERS_HPP_INCLUDED
#define KIWI_LISTENERS_HPP_INCLUDED

#include <memory>
#include <set>
#include <vector>
#include <mutex>

namespace kiwi
{
    // ================================================================================ //
    //                                      LISTENERS                                   //
    // ================================================================================ //
    
    //! @brief The listener set is a class that manages a list of listeners.
    //! @details Manages a list of listeners and allows to retrieve them easily and thread-safely.
    template <class ListenerClass>
    class Listeners
    {
    public:
        
        struct is_valid_listener : std::integral_constant<bool,
        !std::is_pointer<ListenerClass>::value &&
        !std::is_reference<ListenerClass>::value
        > {};
        
        using listener_t = typename std::remove_pointer<typename std::remove_reference<ListenerClass>::type>::type;
        using listener_ref_t = listener_t&;
        using listener_ptr_t = listener_t*;
        
        //! @brief Creates an empty listener set.
        Listeners()
        {
            static_assert(is_valid_listener::value,
                          "Template parameter must not be a pointer or a reference");
        }
        
        //! @brief Destructor.
        ~Listeners() noexcept { m_listeners.clear(); }
        
        //! @brief Add a listener.
        //! @details If the listener was allready present in the set, the function does nothing.
        //! @param listener The new listener to be added.
        //! @return True if success, otherwise false.
        bool add(listener_ref_t listener) noexcept
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            bool insert_success = m_listeners.insert(&listener).second;
            return insert_success;
        }
        
        //! @brief Remove a listener.
        //! @details If the listener wasn't in the set, the function does nothing.
        //! @param listener The listener to be removed.
        //! @return True if success, false otherwise.
        bool remove(listener_ref_t listener) noexcept
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            bool erase_success = m_listeners.erase(&listener);
            return erase_success;
        }
        
        //! @brief Returns the number of listeners.
        size_t size() const noexcept
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            return m_listeners.size();
        }
        
        //! @brief Returns true if there is no listener.
        bool empty() const noexcept
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            return m_listeners.empty();
        }
        
        //! @brief Remove all listeners.
        void clear() noexcept
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            m_listeners.clear();
        }
        
        //! @brief Returns true if the set contains a given listener.
        bool contains(listener_ref_t listener) const noexcept
        {
            return (m_listeners.find(&listener) != m_listeners.end());
        }
        
        //! @brief Get the listeners.
        std::vector<listener_ptr_t> getListeners()
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            return {m_listeners.begin(), m_listeners.end()};
        }
        
        //! @brief Retrieve the listeners.
        std::vector<listener_ptr_t> getListeners() const
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            return {m_listeners.begin(), m_listeners.end()};
        }
        
        //! @brief Calls a given method for each listener of the set.
        //! @param fun The listener's method to call.
        //! @param arguments optional arguments.
        template<class T, class ...Args>
        void call(T fun, Args&& ...arguments) const
        {
            for(auto* listener : getListeners())
            {
                (listener->*(fun))(arguments...);
            }
        }
        
    private:
        
        std::set<listener_ptr_t> m_listeners;
        mutable std::mutex       m_mutex;
    };
}

#endif // KIWI_LISTENERS_HPP_INCLUDED
