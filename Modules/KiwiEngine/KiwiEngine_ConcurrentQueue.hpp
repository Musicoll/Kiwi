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
#include <thread>

#include <concurrentqueue.h>

namespace kiwi
{
    namespace engine
    {
        // ==================================================================================== //
        //                                       CONCURRENTQUEUE                                //
        // ==================================================================================== //
        
        namespace mc = moodycamel;
        
        template <class T>
        class ConcurrentQueue final
        {
        private: // classes
            
            struct Traits : public mc::ConcurrentQueueDefaultTraits
            {
                static const size_t BLOCK_SIZE = 1024;
            };
            
        public: // methods
            
            ConcurrentQueue(size_t capacity, size_t limit):
            m_producer_queue(new mc::ConcurrentQueue<T, Traits>(capacity)),
            m_consumer_queue(m_producer_queue.load()),
            m_capacity(capacity),
            m_limit(limit),
            m_size(0),
            m_alloc_thread()
            {
            }
            
            ~ConcurrentQueue()
            {
                if (m_alloc_thread.joinable())
                {
                    m_alloc_thread.join();
                }
                
                delete m_producer_queue.load();
            }
            
            void push(T const& value)
            {
                while(!m_producer_queue.load()->try_enqueue(value)){}
                
                size_t new_size = std::atomic_fetch_add(&m_size, 1) + 1;
                
                if (m_alloc_thread.joinable()){m_alloc_thread.join();};
                
                if (new_size >= (m_capacity.load() - m_limit))
                {
                    m_alloc_thread = std::thread(std::bind(&ConcurrentQueue::reallocate, this));
                }
            }
            
            bool pop(T & value)
            {
                bool success = m_consumer_queue.load()->try_dequeue(value);
                
                std::atomic_fetch_add(&m_size, -1);
                
                return success;
            }
            
            size_t load_size() const
            {
                return m_size.load();
            }
            
        private: // methods
            
            void reallocate()
            {
                int capacity = m_capacity.load();
                
                mc::ConcurrentQueue<T, Traits>* new_queue = new mc::ConcurrentQueue<T, Traits>(2 * capacity);
                mc::ConcurrentQueue<T, Traits>* former_queue = m_producer_queue.exchange(new_queue);
                
                while(former_queue->size_approx() != 0)
                {
                }
                
                m_consumer_queue.exchange(new_queue);
                
                delete former_queue;
                
                m_capacity.store(2 * capacity);
            }
            
        private: // members
            
            std::atomic<mc::ConcurrentQueue<T, Traits> *>   m_producer_queue;
            std::atomic<mc::ConcurrentQueue<T, Traits> *>   m_consumer_queue;
            std::atomic<int>                                m_capacity;
            size_t                                          m_limit;
            std::atomic<int>                                m_size;
            std::thread                                     m_alloc_thread;
            
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
