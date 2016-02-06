/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef KIWI_DSP_NODE_H_INCLUDED
#define KIWI_DSP_NODE_H_INCLUDED

#include "KiwiDspProcessor.hpp"
#include "KiwiDspLink.hpp"

namespace kiwi
{
namespace dsp
{
    class Chain;
    // ==================================================================================== //
    //                                          NODE                                        //
    // ==================================================================================== //
    //! @brief The class manages a set of Node objects connected by a set Link objects.
    //! @details The class acts like a container for Node and Link objects. After
    //! @details adding the Node and Link objects, the class compiles the digital signal
    //! @details processing chain by sorting and connecting the Node objects depending on the
    //! @details Link objects.
    class Node
    {
    public:
        Node(Processor& processor) noexcept :
        m_processor(processor), m_index(0ul)
        {
            m_inputs.resize(processor.getNumberOfInputs());
            m_outputs.resize(processor.getNumberOfOutputs());
        }
        
        ~Node()
        {
            if(m_processor.isRunning())
            {
                m_processor.release();
            }
        }
        
        //! @brief Checks if an input is connected to any Node.
        //! @details This method can be used to ignore an input's sample vector if the
        //! @details input isn't connected. You should prefer to use it from the preprare
        //! @details method than directly from the perform method.
        //! @return true if the inlet is connected, otherwise it returns false.
        //! @see isOutputConnected() and prepare()
        inline bool isInputConnected(const size_t index) const noexcept {
            assert(index < m_inputs.size() && "Index must be inferior to the number of inputs.");
            return !m_inputs[index].empty();
        }
        
        //! @brief Checks if an outlet is connected to any Node.
        //! @details This method can be used to ignore an output's sample vector if the
        //! @details output isn't connected. You should prefer to use it from the preprare
        //! @details method than directly from the perform method.
        //! @return true if the outlet is connected, otherwise it returns false.
        //! @see isInputConnected() and prepare()
        inline bool isOutputConnected(const size_t index) const noexcept {
            assert(index < m_outputs.size() && "Index must be inferior to the number of outputs.");
            return !m_outputs[index].empty();
        }
        
        //! @brief Retrieves the index of the Node object in the Chain object.
        //! @details If the index is zero, the node is out of the Chain or the Chain
        //! @details isn't compiled. All the Node objects connected to the inputs
        //! @details should have a lower index and all the Node objects connected to
        //! @details the outputs should have an upper index.
        inline size_t getIndex() const noexcept {return m_index;}
        
    private:
        
        //! @brief Adds a node to an input.
        //! @param node The node to add.
        //! @param index The index of the input.
        //! @see addOutput()
        void addInput(Node* node, const size_t index)
        {
            if(index < size_t(m_inputs.size()))
            {
                if(!m_inputs[index].insert(node).second)
                {
                    class ErrorDuplicate : public Error
                    {
                    public:
                        const char* what() const noexcept override {
                            return "Osp::Node : The input Node is already connected.";}
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
                        return "Osp::Node : The input Node is connected to a wrong index.";}
                };
                throw ErrorIndex();
            }
        }
        
        //! @brief Adds a node to an output.
        //! @param node The node to add.
        //! @param index The index of the output.
        //! @see addInput()
        void addOutput(Node* node, const size_t index)
        {
            if(index < size_t(m_outputs.size()))
            {
                if(!m_outputs[index].insert(node).second)
                {
                    class ErrorDuplicate : public Error
                    {
                    public:
                        const char* what() const noexcept override {
                            return "Osp::Node : The output Node is already connected.";}
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
                        return "Osp::Node : The output Node is connected to a wrong index.";}
                };
                throw ErrorIndex();
            }
        }
        
        void prepare()
        {
            
        }
        
        //! Ticks once the digital signal processing.
        /** This method is called by the Chain during its tick method's call. This method calls the inputs
         perform methods and the perform method of the child class' implementation.
         @see start() and stop()
         */
        inline void perform() noexcept
        {
            if(m_processor.isRunning())
            {
                for(std::vector< std::set<Node*> >::size_type i = 0; i < m_inputs.size(); i++)
                {
                    //m_inputs[i].perform();
                }
                m_processor.perform();
            }
        }
        
        Processor&                      m_processor;
        size_t                          m_index;
        std::vector< std::set<Node*> >  m_inputs;
        std::vector< std::set<Node*> >  m_outputs;
    };
}
}

#endif // KIWI_DSP_NODE_H_INCLUDED
