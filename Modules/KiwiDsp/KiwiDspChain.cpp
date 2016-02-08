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
                        m_nodes.push_back(std::make_shared<Node>(*this, *processor));
                    }
                    catch(std::exception& e)
                    {
                        throw;
                    }
                }
            }
            
            // ============================================================================ //
            //                              CONNECTS THE NODES                              //
            // ============================================================================ //
            for(auto link : links)
            {
                if(link)
                {
                    std::shared_ptr< Node > output, input;
                    for(auto const& node : m_nodes)
                    {
                        if(&node->m_processor == &link->getInputProcessor())
                        {
                            input = node;
                        }
                        else if(&node->m_processor == &link->getOutputProcessor())
                        {
                            output = node;
                        }
                        if(output && input)
                        {
                            break;
                        }
                    }
                    if(output && input)
                    {
                        try
                        {
                            output->addOutput(input, link->getInputIndex());
                        }
                        catch(std::exception& e)
                        {
                            throw;
                        }
                        
                        try
                        {
                            input->addInput(output, link->getOutputIndex());
                        }
                        catch(std::exception& e)
                        {
                            throw;
                        }
                    }
                    else
                    {
                        class ErrorLink : public Error
                        {
                        public:
                            const char* what() const noexcept final {return "Kiwi::Dsp::Chain : A link isn't valid.";}
                        };
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
                        for(auto input : node->m_inputs)
                        {
                            std::set< std::weak_ptr< Node >, std::owner_less< std::weak_ptr< Node > > >& set = input;
                            for(auto wnode : set)
                            {
                                std::shared_ptr< Node > snode = wnode.lock();
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
