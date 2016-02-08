//
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//

#include "KiwiDspNode.hpp"
#include "KiwiDspChain.hpp"

namespace kiwi
{
    namespace dsp
    {
        Node::Node(Chain const& chain, Processor& processor) :
        m_chain(chain), m_processor(processor),
        m_buffer_in(nullptr), m_buffer_out(nullptr),
        m_sample_rate(chain.getSampleRate()), m_vector_size(chain.getVectorSize()),
        m_index(0ul), m_valid(false)
        {
            if(m_processor.isRunning())
            {
                class ErrorRunning : public Error
                {
                public:
                    const char* what() const noexcept final {return "Kiwi::Dsp::Node : The processor is already in a Chain.";}
                };
                throw ErrorRunning();
            }
            
            class ErrorResize : public Error
            {
            public:
                const char* what() const noexcept final {return "Kiwi::Dsp::Node : The Node can't allocate its ioputs.";}
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
        
        Node::~Node()
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
            m_buffer_in     = Samples<sample>::release(m_buffer_in);
        }
        
        void Node::addInput(std::shared_ptr< Node > node, const size_t index)
        {
            if(m_processor.isRunning())
            {
                class ErrorRunning : public Error
                {
                public:
                    const char* what() const noexcept final {return "Kiwi::Dsp::Node : The processor is already in a Chain.";}
                };
                throw ErrorRunning();
            }
            
            if(index < static_cast<size_t>(m_inputs.size()))
            {
                if(!m_inputs[index].insert(node).second)
                {
                    class ErrorDuplicate : public Error
                    {
                    public:
                        const char* what() const noexcept final {
                            return "Kiwi::Dsp::Node : The input Node is already connected.";}
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
                        return "Kiwi::Dsp::Node : The input Node is connected to a wrong index.";}
                };
                throw ErrorIndex();
            }
        }
        
        void Node::addOutput(std::shared_ptr< Node > node, const size_t index)
        {
            if(m_processor.isRunning())
            {
                class ErrorRunning : public Error
                {
                public:
                    const char* what() const noexcept final {return "Kiwi::Dsp::Node : The processor is already in a Chain.";}
                };
                throw ErrorRunning();
            }
            
            if(index < static_cast<size_t>(m_outputs.size()))
            {
                if(!m_outputs[index].insert(node).second)
                {
                    class ErrorDuplicate : public Error
                    {
                    public:
                        const char* what() const noexcept final {
                            return "Kiwi::Dsp::Node : The output Node is already connected.";}
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
                        return "Kiwi::Dsp::Node : The output Node is connected to a wrong index.";}
                };
                throw ErrorIndex();
            }
        }
        
        void Node::prepare()
        {
            if(m_processor.isRunning())
            {
                class ErrorRunning : public Error
                {
                public:
                    const char* what() const noexcept final {return "Kiwi::Dsp::Node : The processor is already in a Chain.";}
                };
                throw ErrorRunning();
            }
            
            for(auto& set : m_inputs)
            {
                auto it = set.cbegin();
                while(it != set.cend())
                {
                    if(it->expired())
                    {
                        it = set.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
            
            for(std::vector< std::vector< sample const* > >::size_type i = 0; i < m_buffer_copy.size(); ++i)
            {
                for(auto node : m_inputs[i])
                {
                    std::shared_ptr<Node> snode = node.lock();
                    if(snode)
                    {
                        // TODO : get the sample from a specific output
                        m_buffer_copy[i].push_back(snode->m_buffer_out);
                    }
                }
            }
            if(m_processor.getNumberOfInputs() || m_processor.getNumberOfOutputs())
            {
                const size_t nchannels = m_processor.getNumberOfInputs() >= m_processor.getNumberOfOutputs() ? m_processor.getNumberOfInputs() : m_processor.getNumberOfOutputs();
                m_buffer_out = m_buffer_in = Samples<sample>::allocate(m_vector_size * nchannels);
            }
            
            
            if(m_buffer_in)
            {
                try
                {
                    m_valid = m_processor.m_running = m_processor.prepare(*this);
                }
                catch(std::exception& e)
                {
                    m_buffer_out = m_buffer_in = Samples<sample>::release(m_buffer_in);
                    throw e;
                }
            }
            else
            {
                class ErrorAlloc : public Error
                {
                public:
                    const char* what() const noexcept final {return "Kiwi::Dsp::Node : Can't allocate the buffer.";}
                };
                throw ErrorAlloc();
            }
        }
        
        void Node::perform() const noexcept
        {
            if(m_processor.isRunning())
            {
                typedef std::vector< std::vector< sample const* > >::size_type inc_type;
                for(inc_type i = 0; i < m_buffer_copy.size(); ++i)
                {
                    sample *const buffer = m_buffer_in+i*m_vector_size;
                    if(!m_buffer_copy[i].empty())
                    {
                        Samples<sample>::copy(m_vector_size, m_buffer_copy[i][0], buffer);
                        for(std::vector< sample const* >::size_type j = 1; j < m_buffer_copy[i].size(); ++j)
                        {
                            Samples<sample>::add(m_vector_size, m_buffer_copy[i][j], buffer);
                        }
                    }
                    else
                    {
                        Samples<sample>::clear(m_vector_size, buffer);
                    }
                }
                m_processor.perform(*this);
            }
        }
    }
}
