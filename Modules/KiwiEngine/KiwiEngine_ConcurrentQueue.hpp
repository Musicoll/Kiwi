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

#include <readerwriterqueue.h>

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
            
        public: // methods
            
            ConcurrentQueue(size_t capacity):
            m_queue(new mc::ReaderWriterQueue<T, 1024>(capacity)),
            m_size(0)
            {
            }
            
            ~ConcurrentQueue()
            {
            }
            
            void push(T const& value)
            {
                m_queue->enqueue(value);
                
                std::atomic_fetch_add(&m_size, 1);
            }
            
            bool pop(T & value)
            {
                bool success = m_queue->try_dequeue(value);
                
                if (success) { std::atomic_fetch_add(&m_size, -1);}
                
                return success;
            }
            
            size_t load_size() const
            {
                return m_size.load();
            }
            
        private: // members
            
            std::unique_ptr<mc::ReaderWriterQueue<T, 1024>> m_queue;
            std::atomic<int>                                m_size;
            
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
