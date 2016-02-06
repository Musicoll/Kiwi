/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "KiwiDspNode.hpp"

namespace kiwi
{
    namespace dsp
    {
        Node::Node(Processor& processor) :
        m_processor(processor), m_index(0ul)
        {
            class ErrorResize : public Error
            {
            public:
                const char* what() const noexcept override {return "Kiwi::Dsp::Node : The Node can't allocate its ioputs.";}
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
        
        void Node::addInput(Node* node, const size_t index)
        {
            if(index < static_cast<size_t>(m_inputs.size()))
            {
                if(!m_inputs[index].insert(node).second)
                {
                    class ErrorDuplicate : public Error
                    {
                    public:
                        const char* what() const noexcept override {
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
                    const char* what() const noexcept override {
                        return "Kiwi::Dsp::Node : The input Node is connected to a wrong index.";}
                };
                throw ErrorIndex();
            }
        }
        
        void Node::addOutput(Node* node, const size_t index)
        {
            if(index < static_cast<size_t>(m_outputs.size()))
            {
                if(!m_outputs[index].insert(node).second)
                {
                    class ErrorDuplicate : public Error
                    {
                    public:
                        const char* what() const noexcept override {
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
                    const char* what() const noexcept override {
                        return "Kiwi::Dsp::Node : The output Node is connected to a wrong index.";}
                };
                throw ErrorIndex();
            }
        }
        
        void Node::prepare()
        {
            
        }
    }
}
