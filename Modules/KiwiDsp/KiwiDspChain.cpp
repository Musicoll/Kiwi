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
            if(m_running)
            {
                stop();
            }
            m_nodes.clear();
        }
        
        void Chain::stop()
        {
            if(m_running)
            {
                m_running = false;
                for(auto& node : m_nodes)
                {
                    try
                    {
                        node->prepare();
                    }
                    catch(Error& e)
                    {
                        throw e;
                    }
                }
            }
        }
        
        void Chain::tick() const noexcept
        {
            for(std::vector<Node>::size_type i = 0; i < m_nodes.size(); i++)
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
            if(m_running)
            {
                stop();
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
                        m_nodes.emplace_back(std::make_shared<Node>(*this, *processor));
                    }
                    catch(Error& e)
                    {
                        throw e;
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
                    std::shared_ptr< Node > output = *(std::find_if(m_nodes.begin(), m_nodes.end(),
                                                                  [=] (std::shared_ptr< Node > const& node)
                                                                  {
                                                                      return &node->m_processor == &link->getOutputProcessor();
                                                                  }));
                    
                    std::shared_ptr< Node > input = *(std::find_if(m_nodes.begin(), m_nodes.end(),
                                                                  [=] (std::shared_ptr< Node > const& node)
                                                                  {
                                                                      return &node->m_processor == &link->getInputProcessor();
                                                                  }));
                    if(output && input)
                    {
                        try
                        {
                            output->addOutput(input, link->getInputIndex());
                        }
                        catch(Error& e)
                        {
                            throw e;
                        }
                        
                        try
                        {
                            input->addInput(output, link->getOutputIndex());
                        }
                        catch(Error& e)
                        {
                            throw e;
                        }
                    }
                    else
                    {
                        class ErrorLink : public Error
                        {
                        public:
                            const char* what() const noexcept override {return "Kiwi::Dsp::Chain : A link isn't valid.";}
                        };
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
                                            const char* what() const noexcept override {return "Kiwi::Dsp::Chain : A loop is detected.";}
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
                throw e;
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
            for(auto it : m_nodes)
            {
                if(static_cast<bool>(it->m_index))
                {
                    try
                    {
                        it->prepare();
                    }
                    catch(Error& e)
                    {
                        throw e;
                    }
                    // Remove the node if the processor don't want to perform
                }
            }
            
            m_running = true;
        }
    }
}
