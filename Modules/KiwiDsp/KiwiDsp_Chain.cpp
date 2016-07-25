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

namespace kiwi
{
    namespace dsp
    {
        // ==================================================================================== //
        //                                          CHAIN                                       //
        // ==================================================================================== //
        
        Chain::Chain() noexcept :
        m_state(State::NotCompiled),
        m_sample_rate(0ul),
        m_vector_size(0ul)
        {
            ;
        }
        
        Chain::~Chain()
        {
            /*
            assert(m_state.load() != State::NotCompiled &&
                   "The chain must be compiled to be destroyed !");
            */
        }
        
        size_t Chain::getSampleRate() const noexcept
        {
            return m_sample_rate;
        }
        
        size_t Chain::getVectorSize() const noexcept
        {
            return m_vector_size;
        }
        
        Chain::State Chain::getState() const noexcept
        {
            return m_state.load();
        }
        
        void Chain::release()
        {
            if(m_state.load() != State::NotCompiled)
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                
                // clear nodes
                try
                {
                    m_nodes.clear();
                }
                catch(std::exception& e)
                {
                    throw Error("Clear nodes failed : " + std::string(e.what()));
                }
                
                m_sample_rate = 0ul;
                m_vector_size = 0ul;
                
                m_state.store(State::NotCompiled);
            }
        }
        
        void Chain::tick() const noexcept
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            
            // reset nodes (zeroes buffers and set ready-to-process flag)
            for(Node::uPtr const& node_uptr : m_nodes)
            {
                node_uptr->clean();
            }
            
            for(Node::uPtr const& node_uptr : m_nodes)
            {
                if(node_uptr->isTerminal())
                {
                    node_uptr->perform();
                }
            }
        }
        
        void Chain::compile(size_t const samplerate, size_t const vectorsize,
                            std::set<Processor*> const& processors,
                            std::set<Link*> const& links)
        {
            assert(isPowerOfTwo(vectorsize) && "The vectorsize must be a power of two.");
            
            release();
            
            std::lock_guard<std::mutex> guard(m_mutex);
            m_state.store(State::Compiling);
            m_sample_rate = samplerate;
            m_vector_size = vectorsize;
            
            createNodes(processors);
            connectNodes(links);
            
            prepareNodes();
            
            // look-for and store terminal nodes:
            // - the ones who needs their perform method to be called directly.
            
            m_state = State::Ready;
        }
        
        // ============================================================================ //
        //                             NODE AND LINK CREATION                           //
        // ============================================================================ //
        
        void Chain::createNodes(std::set<Processor*> const& processors)
        {
            m_nodes.reserve(processors.size());
            for(Processor* processor : processors)
            {
                assert(processor != nullptr && "A Processor pointer is nullptr.");
                
                if(processor->m_used)
                {
                    throw Error("The Processor object is already in a Chain object.");
                }
                
                try
                {
                    m_nodes.emplace_back(new Node(*processor));
                }
                catch(std::exception& e)
                {
                    processor->m_used = false;
                    throw Error("Node creation failed : " + std::string(e.what()));
                }
                
                processor->m_used = true;
            }
        }
        
        void Chain::connectNodes(std::set<Link*> const& links)
        {
            for(Link* const link : links)
            {
                Node* from = nullptr;
                Node* to = nullptr;
                
                for(Node::uPtr const& node : m_nodes)
                {
                    if(&node->getProcessor() == &link->getInputProcessor())
                    {
                        to = node.get();
                    }
                    else if(&node->getProcessor() == &link->getOutputProcessor())
                    {
                        from = node.get();
                    }
                    
                    if(from && to)
                    {
                        break;
                    }
                }
                
                assert((from && to) && "Invalid Link.");
                
                try
                {
                    Node::connect(*from, link->getOutputIndex(),
                                  *to, link->getInputIndex());
                }
                catch(std::exception& e)
                {
                    throw Error("Node connection failed : " + std::string(e.what()));
                }
            }
        }
        
        // ============================================================================ //
        //                              PREPARES THE NODES                              //
        // ============================================================================ //
        
        void Chain::prepareNodes()
        {
            for(Node::uPtr const& node : m_nodes)
            {
                if(node->isTerminal())
                {
                    node->prepare(m_sample_rate, m_vector_size);
                }
            }
        }
    }
}
