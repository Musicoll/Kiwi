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

#include "KiwiDsp_Chain.hpp"
#include "KiwiDsp_Node.hpp"

namespace kiwi
{
    namespace dsp
    {
        // ==================================================================================== //
        //                                          CHAIN                                       //
        // ==================================================================================== //
        
        Chain::Chain() :
        m_nodes(),
        m_sample_rate(),
        m_vector_size(),
        m_prepare_state(PrepareState::NotPrepared),
        m_release_state(ReleaseState::Released),
        m_perform_state(PerformState::NotReady),
        m_tick_mutex()
        {
            ;
        }
        
        Chain::~Chain()
        {
            assert(m_release_state.load() == ReleaseState::Released && "Removing chain before releasing it :  ");
            
            m_nodes.clear();
        }
        
        void Chain::prepare()
        {
            prepare(getSampleRate(), getVectorSize());
        }
        
        void Chain::prepare(const size_t samplerate, const size_t vector_size)
        {
            if (m_prepare_state.load() == PrepareState::NotPrepared
                || m_release_state.load() == ReleaseState::Released)
            {
                std::lock_guard<std::mutex> lock(m_tick_mutex);
                
                m_prepare_state.store(PrepareState::Preparing);
                
                m_sample_rate = samplerate;
                m_vector_size = vector_size;
                
                // Removes all nodes that has been marked as deleted
                for(auto node = m_nodes.begin(); node != m_nodes.end();)
                {
                    if(node->second.isDeleted())
                    {
                        node->second.release();
                        m_nodes.erase(node++);
                    }
                    else
                    {
                        ++node;
                    }
                }
                
                try
                {
                    // Prepare all nodes that have been marked as notPrepared.
                    for(auto node = m_nodes.begin(); node != m_nodes.end(); ++node)
                    {
                        node->second.prepare(m_sample_rate, m_vector_size);
                    }
                }
                catch(Error & e)
                {
                    m_prepare_state.store(PrepareState::NotPrepared);
                    m_release_state.store(ReleaseState::NotReleased);
                    throw e;
                }
                
                // look-for and store terminal nodes:
                // - the ones who needs their perform method to be called directly.
                
                m_prepare_state.store(PrepareState::Prepared);
                m_release_state.store(ReleaseState::NotReleased);
                m_perform_state.store(PerformState::Ready);
            }
        }
        
        void Chain::release()
        {
            if (m_release_state.load() != ReleaseState::Released)
            {
                std::lock_guard<std::mutex> lock(m_tick_mutex);
                
                m_release_state.store(ReleaseState::Releasing);
                
                for(auto node = m_nodes.begin(); node != m_nodes.end(); ++node)
                {
                    node->second.release();
                }
                
                m_release_state.store(ReleaseState::Released);
                m_prepare_state.store(PrepareState::NotPrepared);
                m_perform_state.store(PerformState::NotReady);
            }
        }
    
        size_t Chain::getSampleRate() const noexcept
        {
            return m_sample_rate;
        }
        
        size_t Chain::getVectorSize() const noexcept
        {
            return m_vector_size;
        }
        
        void Chain::clean() noexcept
        {
            if(m_perform_state.load() == PerformState::Performed)
            {
                for(auto node = m_nodes.begin(); node != m_nodes.end(); ++node)
                {
                    node->second.clean();
                }
                
                m_perform_state.store(PerformState::Ready);
            }
        }
        
        void Chain::tick() noexcept
        {
            if (m_perform_state.load() == PerformState::Ready)
            {
                std::lock_guard<std::mutex> lock(m_tick_mutex);
                
                for(auto node = m_nodes.begin(); node != m_nodes.end(); ++node)
                {
                    if(node->second.isTerminal())
                    {
                        node->second.perform();
                    }
                }
                
                m_perform_state.store(PerformState::Performed);
                
                clean();
            }
        }
        
        // ============================================================================ //
        //                             NODE AND LINK MODIFICATION                       //
        // ============================================================================ //
        
        bool Chain::findProcessor(Processor const& processor) const
        {
            struct compare_proc
            {
                compare_proc(Processor const& proc): m_proc(proc){};
                
                bool operator()(std::pair<const uint64_t, Node> const& node)
                {
                    return &(*node.second.getProcessor()) == &m_proc;
                };
                
                Processor const& m_proc;
            };
            
            return std::find_if(m_nodes.cbegin(), m_nodes.cend(), compare_proc(processor)) != m_nodes.cend();
        }
        
        void Chain::addProcessor(const uint64_t id, std::unique_ptr<Processor> processor)
        {
            if (!findProcessor(*processor))
            {
                if (m_nodes.find(id) == m_nodes.end())
                {
                    std::lock_guard<std::mutex> lock(m_tick_mutex);
                    
                    m_nodes.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(id),
                                    std::forward_as_tuple(std::move(processor), id));
                    
                    m_prepare_state.store(PrepareState::NotPrepared);
                }
                else
                {
                    throw Error("Adding processor with same id :");
                }
            }
            else
            {
                processor.release();
                throw Error("Inserting twice the same processor :");
            }
        }
        
        void Chain::removeProcessor(const uint64_t id)
        {
            auto node = m_nodes.find(id);
            
            if (node != m_nodes.end())
            {
                std::lock_guard<std::mutex> lock(m_tick_mutex);
                
                node->second.setAsDeleted();
                
                m_prepare_state.store(PrepareState::NotPrepared);
            }
            else
            {
                throw Error("Removing a processor with non existing id :");
            }
        }
        
        bool Chain::connect(uint64_t source_node, size_t outlet_index, uint64_t dest_node, size_t inlet_index)
        {
            bool connected = false;
            
            if (m_nodes.find(source_node) != m_nodes.end() &&
                m_nodes.find(dest_node) != m_nodes.end())
            {
                std::lock_guard<std::mutex> lock(m_tick_mutex);
                
                connected = m_nodes.at(dest_node).connectInput(inlet_index, m_nodes.at(source_node), outlet_index);
                
                if (connected)
                {
                    m_prepare_state.store(PrepareState::NotPrepared);
                }
            }
            else
            {
                throw Error("Trying to connect two non existing nodes : ");
            }
            
            return connected;
        }
        
        bool Chain::discconnect(uint64_t source_node, size_t outlet_index, uint64_t dest_node, size_t inlet_index)
        {
            bool disconnected = false;
            
            if (m_nodes.find(source_node) != m_nodes.end() &&
                m_nodes.find(dest_node) != m_nodes.end())
            {
                std::lock_guard<std::mutex> lock(m_tick_mutex);
                
                disconnected = m_nodes.at(dest_node).disconnectInput(inlet_index, m_nodes.at(source_node), outlet_index);
                
                if (disconnected)
                {
                    m_prepare_state.store(PrepareState::NotPrepared);
                }
            }
            else
            {
                throw Error("Trying to discconnect two non existing nodes : ");
            }
            
            return disconnected;
        }
        
        std::shared_ptr<const Processor> Chain::getProcessor(uint64_t id) const
        {
            try
            {
                return m_nodes.at(id).getProcessor();
            }
            catch (const std::out_of_range& e)
            {
                throw Error("Trying to access out of range processor : " + std::string(e.what()));
            }
        };
        
        std::shared_ptr<Processor> Chain::getProcessor(uint64_t id)
        {
            try
            {
                return m_nodes.at(id).getProcessor();
            }
            catch (const std::out_of_range& e)
            {
                throw Error("Trying to access out of range processor : " + std::string(e.what()));
            }
        };
    }
}
