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
        m_chain(chain), m_processor(processor), m_index(0ul)
        {
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
        }
        
        Node::~Node() noexcept
        {
            
        }
        
        size_t Node::getSampleRate() const noexcept
        {
            return m_chain.getSampleRate();
        }
        
        size_t Node::getVectorSize() const noexcept
        {
            return m_chain.getVectorSize();
        }
        
        void Node::addInput(std::shared_ptr< Node > node, const size_t index)
        {
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
                    const char* what() const noexcept final {return "Kiwi::Dsp::Node : The processor is already is a Chain.";}
                };
                throw ErrorRunning();
            }
        }
        
        void Node::perform() const noexcept
        {
            if(m_processor.isRunning())
            {
                for(std::vector< std::set<Node*> >::size_type i = 0; i < m_inputs.size(); i++)
                {
                    //m_inputs[i].perform();
                }
                m_processor.perform(*this);
            }
        }
    }
}
