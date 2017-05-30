/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include <cstdlib>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                  CIRCULAR BUFFER                                 //
        // ================================================================================ //
        
        //! @todo documentation inspired from boost circular buffer
        //! @see http://www.boost.org/doc/libs/1_59_0/doc/html/boost/circular_buffer.html#idp23975984-bb
        template<class T>
        class CircularBuffer final
        {
        public: // methods
            
            CircularBuffer(size_t capacity):
            m_buffer((T*) std::malloc(capacity * sizeof(T))),
            m_head(0),
            m_tail(0),
            m_size(0),
            m_capacity(capacity)
            {
            }
            
            CircularBuffer(size_t capacity, size_t size, T const& value):
            m_buffer((T*) std::malloc(capacity * sizeof(T))),
            m_head(0),
            m_tail(0),
            m_size(0),
            m_capacity(capacity)
            {
                assert(m_capacity >= m_size);
                
                for (int i = 0; i < size; ++i)
                {
                    push_back(value);
                }
            }
            
            ~CircularBuffer()
            {
                for (int i = std::min(m_head, m_tail); i < std::max(m_head, m_tail); ++i)
                {
                    (m_buffer + i)->~T();
                }
                
                std::free(m_buffer);
            }
            
            void assign(size_t nb_elements, const T& value)
            {
                clear();
                
                for(int i = 0; i < nb_elements; ++i)
                {
                    push_back(value);
                }
            }
            
            void clear()
            {
                size_t init_size = m_size;
                
                for (int i = 0; i < init_size; ++i)
                {
                    pop_front();
                }
            }
            
            size_t size() const
            {
                return m_size;
            }
            
            T const& operator[](size_t index) const noexcept
            {
                return *(m_buffer + ((m_head + index) % m_size));
            }
            
            T& operator[](size_t index) noexcept
            {
                return *(m_buffer + ((m_head + index) % m_size));
            }
            
            void push_back(T const& value)
            {
                if (m_size == m_capacity)
                {
                    (m_buffer + m_tail)->~T();
                    increment_head();
                }
                
                new (m_buffer + m_tail) T(value);
                
                increment_tail();
            }
            
            void pop_front()
            {
                if (m_size != 0)
                {
                    (m_buffer + m_head)->~T();
                    increment_head();
                }
            }
            
        private: // methods
            
            void increment_head()
            {
                m_head = (m_head + 1) == m_capacity ? 0 : m_head + 1;
                --m_size;
            }
            
            void increment_tail()
            {
                m_tail = (m_tail + 1) == m_capacity ? 0 : m_tail + 1;
                ++m_size;
            }
            
        private: // members
            
            T*             m_buffer;
            size_t         m_head;
            size_t         m_tail;
            size_t         m_size;
            size_t         m_capacity;
            
        private: // deleted methods
            
            CircularBuffer() = delete;
            CircularBuffer(CircularBuffer const& other) = delete;
            CircularBuffer(CircularBuffer && other) = delete;
            CircularBuffer& operator=(CircularBuffer const& other) = delete;
            CircularBuffer& operator=(CircularBuffer && other) = delete;
            
        };
    }
}
