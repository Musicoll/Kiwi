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

#ifndef KIWI_ENGINE_CONCURRENT_QUEUE_HPP_INCLUDED
#define KIWI_ENGINE_CONCURRENT_QUEUE_HPP_INCLUDED

#include <cstddef>
#include <atomic>

#include <readerwriterqueue.h>

namespace kiwi
{
    namespace engine
    {
        // ==================================================================================== //
        //                                       CONCURRENTQUEUE                                //
        // ==================================================================================== //
        
        namespace mc = moodycamel;
        
        //! @brief A mono producer, mono consumer FIFO lock free queue.
        //! @details Wrapper around a thirdparty concurrent queue.
        //! @see https://github.com/cameron314/readerwriterqueue
        template <class T>
        class ConcurrentQueue final
        {
        public: // methods
         
            //! @brief Constructor.
            //! @details Reserves memory space for at least capcity elements.
            ConcurrentQueue(size_t capacity):
            m_queue(capacity),
            m_size(0)
            {
            }
            
            //! @brief Destructor.
            ~ConcurrentQueue() = default;
            
            //! @brief Pushes element at end of queue (by copy).
            //! @details If number of elements exceeds capacity allocation will occur.
            //! Increments element counter.
            void push(T const& value)
            {
                m_queue.enqueue(value);
                
                std::atomic_fetch_add(&m_size, 1);
            }
            
            //! @brief Pushes element at end of queue (by move).
            //! @details If number of elements exceeds capacity allocation will occur.
            //! Increments element counter.
            void push(T&& value)
            {
                m_queue.enqueue(std::forward<T>(value));
                
                std::atomic_fetch_add(&m_size, 1);
            }
            
            //! @brief Pops first element in the queue.
            //! @details Returns false if the queue was empty and pop failed, true otherwise.
            bool pop(T & value)
            {
                bool success = m_queue.try_dequeue(value);
                
                if (success) { std::atomic_fetch_add(&m_size, -1);}
                
                return success;
            }
            
            //! @brief Returns an approximative size for the queue.
            //! @details Since size is increased and decreased after elements are effectively pushed or poped,
            //! for the consumer the returned size is guaranteed to be lower or equal
            //! to the effective size of the queue, for the producer the returned size is guaranteed to be
            //! greater or equal than the effective size of the queue.
            size_t load_size() const
            {
                return m_size.load();
            }
            
        private: // members
            
            mc::ReaderWriterQueue<T, 1024>      m_queue;
            std::atomic<int>                    m_size;
            
        private: // deleted methods
            
            ConcurrentQueue() = delete;
            ConcurrentQueue(ConcurrentQueue const& other) = delete;
            ConcurrentQueue(ConcurrentQueue && other) = delete;
            ConcurrentQueue& operator=(ConcurrentQueue const& other) = delete;
            ConcurrentQueue& operator=(ConcurrentQueue && other) = delete;
            
        };
    }
}


#endif // KIWI_ENGINE_CONCURRENT_QUEUE_HPP_INCLUDED
