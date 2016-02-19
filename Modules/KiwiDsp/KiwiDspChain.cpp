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
        //! @brief The class wraps and manages a Processor objects.
        //! @details The class mamanges a Processor object to include it in a Chain object.
        //! @see Processor, Chain, Link and Signal.
        class Chain::Node
        {
        public:
            Node(Processor& processor, size_t const samplerate, size_t const vectorsize) :
            m_processor(processor), m_sample_rate(samplerate), m_vector_size(vectorsize),
            m_index(0ul), m_valid(false)
            {
                if(m_processor.m_running)
                {
                    class ErrorRunning : public Error
                    {
                    public:
                        const char* what() const noexcept final {return "Kiwi::Dsp::Chain : The processor is already in a Chain.";}
                    };
                    throw ErrorRunning();
                }
                m_processor.m_running = true;
                
                class ErrorResize : public Error
                {
                public:
                    const char* what() const noexcept final {return "Kiwi::Dsp::Chain : The Processor can't allocate its ioputs.";}
                };
                
                try
                {
                    m_inputs.resize(processor.getNumberOfInputs());
                }
                catch(std::exception& e)
                {
                    throw ErrorResize();
                }
                
                try
                {
                    m_outputs.resize(processor.getNumberOfOutputs());
                }
                catch(std::exception& e)
                {
                    throw ErrorResize();
                }
                
                try
                {
                    m_buffer_copy.resize(m_processor.getNumberOfInputs());
                }
                catch(std::exception& e)
                {
                    throw ErrorResize();
                }
            }
            
            ~Node()
            {
                try
                {
                    m_processor.m_running = false;
                    m_processor.release();
                }
                catch(std::exception& e)
                {
                    throw e;
                }
                m_inputs.clear();
                m_outputs.clear();
                m_buffer_copy.clear();
            }
            
            void addInput(const size_t index, Tie&& tie)
            {
                if(index < static_cast<size_t>(m_inputs.size()))
                {
                    if(m_inputs[index].emplace(std::forward<Tie>(tie)).second)
                    {
                        class ErrorDuplicate : public Error
                        {
                        public:
                            const char* what() const noexcept final {
                                return "Kiwi::Dsp::Processor : The input Processor is already connected.";}
                        };
                        throw ErrorDuplicate();
                    }
                }
                else
                {
                    class ErrorIndex : public Error
                    {
                    public:
                        const char* what() const noexcept final {
                            return "Kiwi::Dsp::Processor : The input Processor is connected to a wrong index.";}
                    };
                    throw ErrorIndex();
                }
            }
            
            void addOutput(const size_t index, Tie&& tie)
            {
                if(index < static_cast<size_t>(m_outputs.size()))
                {
                    if(m_outputs[index].emplace(std::forward<Tie>(tie)).second)
                    {
                        class ErrorDuplicate : public Error
                        {
                        public:
                            const char* what() const noexcept final {
                                return "Kiwi::Dsp::Processor : The output Processor is already connected.";}
                        };
                        throw ErrorDuplicate();
                    }
                }
                else
                {
                    class ErrorIndex : public Error
                    {
                    public:
                        const char* what() const noexcept final {
                            return "Kiwi::Dsp::Processor : The output Processor is connected to a wrong index.";}
                    };
                    throw ErrorIndex();
                }
            }
            
            void prepare()
            {
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
                
                if(0)
                {
                    
                }
                else
                {
                    class ErrorAlloc : public Error
                    {
                    public:
                        const char* what() const noexcept final
                        {return "Kiwi::Dsp::Node : Can't allocate the buffer.";}
                    };
                    throw ErrorAlloc();
                }
            }
            
            void perform() noexcept
            {
                /*
                typedef std::vector< std::vector< sample_t const* > >::size_type inc_type;
                for(inc_type i = 0; i < m_buffer_copy.size(); ++i)
                {
                    sample_t* buffer = m_buffer_in + i * m_vector_size;
                    if(!m_buffer_copy[i].empty())
                    {
                        Samples< sample_t >::copy(m_vector_size, m_buffer_copy[i][0], buffer);
                        for(std::vector< sample_t const* >::size_type j = 1; j < m_buffer_copy[i].size(); ++j)
                        {
                            Samples< sample_t >::add(m_vector_size, m_buffer_copy[i][j], buffer);
                        }
                    }
                    else
                    {
                        Samples< sample_t >::clear(m_vector_size, buffer);
                    }
                }
                 */
                Buffer zaza;
                m_processor.perform(zaza, m_buffer);
            }
            
            Processor&                          m_processor;
            Buffer                              m_buffer;
            size_t                              m_sample_rate;
            size_t                              m_vector_size;
            size_t                              m_index;
            bool                                m_valid;
            std::vector< std::vector< sample_t const* > > m_buffer_copy;
            std::vector< std::set< Tie > > m_inputs;
            std::vector< std::set< Tie > > m_outputs;
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
            catch(std::exception& e)
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
            stop();
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
                    class ErrorNode : public Error
                    {
                    public:
                        const char* what() const noexcept final {
                            return "Kiwi::Dsp::Chain : A Processor isn't valid.";}
                    };
                    throw ErrorNode();
                }
            }
            
            // ============================================================================ //
            //                              CONNECTS THE NODES                              //
            // ============================================================================ //
            class ErrorLink : public Error
            {
            public:
                const char* what() const noexcept final {return "Kiwi::Dsp::Chain : A link isn't valid.";}
            };
            
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
                        throw ErrorLink();
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
                                        class ErrorLoop : public Error
                                        {
                                        public:
                                            const char* what() const noexcept final {return "Kiwi::Dsp::Chain : A loop is detected.";}
                                        };
                                        
                                        throw ErrorLoop();
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
