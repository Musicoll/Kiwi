//
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//

#include "KiwiDspChain.hpp"

namespace kiwi
{
    namespace dsp
    {
        // ==================================================================================== //
        //                                          TIE                                         //
        // ==================================================================================== //
        
        Chain::Tie::Tie(Link& link, std::vector< std::shared_ptr< Node> > const& nodes) :
        m_link(link)
        {
            std::shared_ptr< Node> from, to;
            for(auto const& node : nodes)
            {
                if(&node->getProcessor() == &link.getInputProcessor())
                {
                    to = node;
                }
                else if(&node->getProcessor() == &link.getOutputProcessor())
                {
                    from = node;
                }
                if(from && to)
                {
                    break;
                }
            }
            assert((!from || !to) && "A Link isn't valid.");
            m_from  = from;
            m_to    = to;
        }
        
        Link const& Chain::Tie::getLink() const noexcept
        {
            return m_link;
        }
        
        Link& Chain::Tie::getLink() noexcept
        {
            return m_link;
        }
        
        void Chain::Tie::connect() const
        {
            std::shared_ptr< Node> from = m_from.lock();
            std::shared_ptr< Node> to   = m_to.lock();
            try
            {
                to->addInput(shared_from_this());
            }
            catch(std::exception& e)
            {
                throw;
            }
            
            try
            {
                from->addOutput(shared_from_this());
            }
            catch(std::exception& e)
            {
                throw;
            }
        }
        
        std::shared_ptr< const Chain::Node > Chain::Tie::getOutputNode() const noexcept
        {
            return m_from.lock();
        }
        
        std::shared_ptr< const Chain::Node >  Chain::Tie::getInputNode() const noexcept
        {
            return m_to.lock();
        }
        
        size_t Chain::Tie::getOutputIndex() const noexcept
        {
            return m_from_index;
        }
        
        size_t Chain::Tie::getInputIndex() const noexcept
        {
            return m_to_index;
        }
        
        // ==================================================================================== //
        //                                          NODE                                        //
        // ==================================================================================== //
        
        Chain::Node::Node(Processor& processor, size_t const samplerate, size_t const vectorsize) :
        m_processor(processor), m_sample_rate(samplerate), m_vector_size(vectorsize),
        m_index(0ul), m_valid(false)
        {
            if(m_processor.m_running)
            {
                throw Error("The Processor object is already in a Chain object.");
            }
            m_processor.m_running = true;
            m_inputs.resize(processor.getNumberOfInputs());
            m_outputs.resize(processor.getNumberOfOutputs());
        }
        
        Chain::Node::~Node()
        {
            m_inputs.clear();
            m_outputs.clear();
            try
            {
                m_processor.m_running = false;
                m_processor.release();
            }
            catch(std::exception&)
            {
                throw;
            }
        }
        
        void Chain::Node::addInput(std::shared_ptr< const Chain::Tie > tie)
        {
            if(tie->getInputIndex() < static_cast<size_t>(m_inputs.size()))
            {
                if(!m_inputs[tie->getInputIndex()].insert(tie).second)
                {
                    throw Error("The Link object already exists.");
                }
            }
            else
            {
                throw Error("The Link object has a wrong output index.");
            }
        }
        
        void Chain::Node::addOutput(std::shared_ptr< const Chain::Tie > tie)
        {
            if(tie->getOutputIndex() < static_cast<size_t>(m_outputs.size()))
            {
                if(!m_outputs[tie->getOutputIndex()].insert(tie).second)
                {
                    throw Error("The Link object already exists.");
                }
            }
            else
            {
                throw Error("The Link object has a wrong input index.");
            }
        }
        
        void Chain::Node::prepare()
        {
            // Clears the invalid previous Node objects
            for(auto& set : m_inputs)
            {
                auto it = set.cbegin();
                while(it != set.cend())
                {
                    if(!it->lock()->getInputNode())
                    {
                        it = set.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
            
            std::vector<bool> inputs_states(m_inputs.size()), outputs_states(m_outputs.size());
            for(std::vector< std::set< Tie > >::size_type i = 0; i < m_inputs.size(); i++)
            {
                inputs_states[i] = !m_inputs[i].empty();
            }
            for(std::vector< std::set< Tie > >::size_type i = 0; i < m_outputs.size(); i++)
            {
                outputs_states[i] = !m_outputs[i].empty();
            }
            
            try
            {
                Infos infos(m_sample_rate, m_vector_size, inputs_states, outputs_states);
                m_valid = m_processor.m_running = m_processor.prepare(infos);
            }
            catch(std::exception& e)
            {
                throw;
            }
            
            /*
             if(0)
             {
             
             }
             else
             {
             throw Error("The Processor can't allocate its buffers.");
             }
             */
        }
        
        void Chain::Node::perform() noexcept
        {
            ;
        }
        
        // ==================================================================================== //
        //                                          CHAIN                                       //
        // ==================================================================================== //
        Chain::Chain() :
        m_state(State::NotCompiled), m_sample_rate(0ul), m_vector_size(0ul)
        {
            
        }
        
        Chain::~Chain()
        {
            release();
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
            return m_state;
        }
        
        void Chain::release()
        {
            if(m_state == State::Processing)
            {
                try
                {
                    m_nodes.clear();
                }
                catch(std::exception&)
                {
                    throw;
                }
                try
                {
                    m_ties.clear();
                }
                catch(std::exception&)
                {
                    throw;
                }
                m_state = State::NotCompiled;
            }
        }
        
        void Chain::tick() const noexcept
        {
            for(std::vector< Node >::size_type i = 0; i < m_nodes.size(); i++)
            {
                m_nodes[i]->perform();
            }
        }
        
        void Chain::compile(size_t const samplerate, size_t const vectorsize,
                            std::set<Processor *> const& processors,
                            std::set<Link *> const& links)
        {
            // ============================================================================ //
            //                              STOPS THE DSP                                   //
            // ============================================================================ //
            try
            {
                release();
            }
            catch(std::exception&)
            {
                // Attention on doit pouvoir vider les vecteurs sans appeler release des processors
                throw;
            }
            
            m_sample_rate = samplerate;
            m_vector_size = vectorsize;
            
            // ============================================================================ //
            //                              CREATES THE NODES                               //
            // ============================================================================ //
            m_nodes.reserve(processors.size());
            for(auto processor : processors)
            {
                assert(processor != nullptr && "A Processor pointer is nullptr.");
                try
                {
                    m_nodes.push_back(std::make_shared< Node >(*processor, m_sample_rate, m_vector_size));
                }
                catch(std::exception& e)
                {
                    throw;
                }
            }
            
            // ============================================================================ //
            //                              CONNECTS THE NODES                              //
            // ============================================================================ //
            m_ties.reserve(links.size());
            for(auto link : links)
            {
                assert(link != nullptr && "A Link pointer is nullptr.");
                try
                {
                    m_ties.push_back(std::make_shared< Tie >(*link, m_nodes));
                }
                catch(std::exception& e)
                {
                    throw;
                }
                
                try
                {
                    m_ties.back()->connect();
                }
                catch(std::exception& e)
                {
                    throw;
                }
            }
            
            // ============================================================================ //
            //                              NUMBERS THE NODES                               //
            // ============================================================================ //
            struct Sorter
            {
                size_t          m_index;
                std::set< std::weak_ptr< Node >, std::owner_less< std::weak_ptr< Node >  > > m_nodes;
                
                inline Sorter() noexcept : m_index(1ul) {}
                inline ~Sorter() noexcept {m_nodes.clear();}
                void operator()(std::shared_ptr< Node >& node)
                {
                    if(!static_cast<bool>(node->m_index))
                    {
                        m_nodes.insert(node);
                        for(auto& input : node->m_inputs)
                        {
                            for(auto& wnode : input)
                            {
                                std::shared_ptr< Node > snode = wnode.node.lock();
                                if(snode && !static_cast<bool>(snode->m_index))
                                {
                                    if(m_nodes.find(snode) != m_nodes.end())
                                    {
                                        throw Error("A loop is detected.");
                                    }
                                    else
                                    {
                                        this->operator()(snode);
                                    }
                                }
                            }
                        }
                        m_nodes.erase(node);
                        node->m_index = m_index++;
                    }
                }
            };
            
            try
            {
                for_each(m_nodes.begin(), m_nodes.end(), Sorter());
            }
            catch(Error& e)
            {
                throw;
            }
            
            // ============================================================================ //
            //                              SORTS THE NODES                                 //
            // ============================================================================ //
            sort(m_nodes.begin(), m_nodes.end(), [] (std::shared_ptr<Node> const& n1, std::shared_ptr<Node> const& n2)
                 {
                     return n1->m_index < n2->m_index;
                 });
            
            
            // ============================================================================ //
            //                              PREPARES THE NODES                              //
            // ============================================================================ //
            auto it = m_nodes.cbegin();
            while(it != m_nodes.cend())
            {
                if(static_cast<bool>((*it)->m_index))
                {
                    try
                    {
                        (*it)->prepare();
                    }
                    catch(std::exception& e)
                    {
                        throw;
                    }
                    if(!(*it)->m_valid)
                    {
                        it = m_nodes.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
            
            m_state = State::Processing;
        }
    }
}
