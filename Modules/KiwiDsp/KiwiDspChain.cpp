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
        class Chain::Tie
        {
        public:
            std::weak_ptr< Node > node;
            size_t                index;
            Tie(std::shared_ptr< Node > n, size_t i) : node(n), index(i) {}
            Tie(Tie const& other) : node(other.node), index(other.index) {}
            bool operator<(Tie const& ) const noexcept {return false;}
        };
        
        // ==================================================================================== //
        //                                          NODE                                        //
        // ==================================================================================== //
        //! @brief The class manages a Processor objects.
        class Chain::Node
        {
        public:
            Node(Processor& processor, size_t const samplerate, size_t const vectorsize) :
            m_processor(processor), m_sample_rate(samplerate), m_vector_size(vectorsize),
            m_index(0ul), m_valid(false)
            {
                if(m_processor.m_running)
                {
                    throw Error("The Processor object is already in a Chain object.");
                }
                m_processor.m_running = true;
                
                try
                {
                    m_inputs.resize(processor.getNumberOfInputs());
                }
                catch(std::exception&)
                {
                    throw Error("The Processor object can't allocate its inputs.");
                }
                
                try
                {
                    m_outputs.resize(processor.getNumberOfOutputs());
                }
                catch(std::exception&)
                {
                    throw Error("The Processor object can't allocate its outputs");
                }
            }
            
            ~Node()
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
            
            void addInput(const size_t index, Tie&& tie)
            {
                if(index < static_cast<size_t>(m_inputs.size()))
                {
                    if(!m_inputs[index].emplace(std::forward<Tie>(tie)).second)
                    {
                        throw Error("The Link object already exists.");
                    }
                }
                else
                {
                    throw Error("The Link object has a wrong output index.");
                }
            }
            
            void addOutput(const size_t index, Tie&& tie)
            {
                if(index < static_cast<size_t>(m_outputs.size()))
                {
                    if(!m_outputs[index].emplace(std::forward<Tie>(tie)).second)
                    {
                        throw Error("The Link object already exists.");
                    }
                }
                else
                {
                    throw Error("The Link object has a wrong input index.");
                }
            }
            
            void prepare()
            {
                // Clears the invalid previous Node objects
                for(auto& set : m_inputs)
                {
                    auto it = set.cbegin();
                    while(it != set.cend())
                    {
                        if(it->node.expired())
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
            
            void perform() noexcept
            {
                ;
            }
            
            Processor&                          m_processor;
            Buffer                              m_buffer;
            size_t                              m_sample_rate;
            size_t                              m_vector_size;
            size_t                              m_index;
            bool                                m_valid;
            std::vector< std::set< Tie > >      m_inputs;
            std::vector< std::set< Tie > >      m_outputs;
        };
        
        
        
        
        
        Chain::Chain() :
        m_running(false), m_sample_rate(0ul), m_vector_size(0ul)
        {
            
        }
        
        Chain::~Chain()
        {
            stop();
        }
        
        void Chain::stop()
        {
            m_running = false;
            try
            {
                m_nodes.clear();
            }
            catch(std::exception&)
            {
                throw;
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
                            std::vector<Processor *> const& processors,
                            std::vector<Link *> const& links)
        {
            // ============================================================================ //
            //                              STOPS THE DSP                                   //
            // ============================================================================ //
            try
            {
                stop();
            }
            catch(std::exception&)
            {
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
                if(processor)
                {
                    try
                    {
                        m_nodes.push_back(std::make_shared<Node>(*processor, m_sample_rate, m_vector_size));
                    }
                    catch(std::exception& e)
                    {
                        throw;
                    }
                }
                else
                {
                    throw Error("A Processor object is not valid.");
                }
            }
            
            // ============================================================================ //
            //                              CONNECTS THE NODES                              //
            // ============================================================================ //
            for(auto link : links)
            {
                if(link)
                {
                    std::shared_ptr< Node > from, to;
                    for(auto const& node : m_nodes)
                    {
                        if(&node->m_processor == &link->getInputProcessor())
                        {
                            to = node;
                        }
                        else if(&node->m_processor == &link->getOutputProcessor())
                        {
                            from = node;
                        }
                        if(from && to)
                        {
                            break;
                        }
                    }
                    if(from && to)
                    {
                        try
                        {
                            to->addInput(link->getInputIndex(), {from, link->getOutputIndex()});
                        }
                        catch(std::exception& e)
                        {
                            throw;
                        }
                        
                        try
                        {
                            from->addOutput(link->getOutputIndex(), {to, link->getInputIndex()});
                        }
                        catch(std::exception& e)
                        {
                            throw;
                        }
                    }
                    else
                    {
                        throw Error("A Link object is not valid.");
                    }
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
            
            m_running = true;
        }
    }
}
