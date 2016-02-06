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
    //! @brief The class wraps and manages a Processor objects.
    //! @details The class mamanges a Processor object to include it in a Chain object. During
    //! @details the calls of the DSP methods of the Processor object, this one can use the
    //! @details Node object to retrieve several informations or the vectors of samples.
    //! @see Processor, Chain, Link and Signal.
    class Node
    {
        friend class Chain;
    public:
        //! @brief The constructor.
        //! @details Allocates and initializes a Node object for a Processor object.
        Node(Processor& processor);
        
        //! @brief The destructor.
        ~Node() noexcept;
        
        //! @brief Checks if an input is connected to any Node object.
        //! @details This method can be used to ignore an input's sample vector if the input
        //! @details isn't connected. You should prefer to use it from the preprare method
        //! @details than directly from the perform method.
        //! @return true if the inlet is connected, otherwise it returns false.
        //! @see isOutputConnected() and prepare()
        inline bool isInputConnected(const size_t index) const noexcept {
            assert(index < m_inputs.size() && "Kiwi::Dsp::Node : Index must be inferior to the number of inputs.");
            return !m_inputs[index].empty();
        }
        
        //! @brief Checks if an outlet is connected to any Node object.
        //! @details This method can be used to ignore an output's sample vector if the output
        //! @details isn't connected. You should prefer to use it from the preprare method
        //! @details than directly from the perform method.
        //! @return true if the outlet is connected, otherwise it returns false.
        //! @see isInputConnected() and prepare()
        inline bool isOutputConnected(const size_t index) const noexcept {
            assert(index < m_outputs.size() && "Kiwi::Dsp::Node : Index must be inferior to the number of outputs.");
            return !m_outputs[index].empty();
        }
        
        //! @brief Gets the current sample rate.
        size_t getSampleRate() const noexcept;
        
        //! @brief Gets the current vector size of the chain.
        size_t getVectorSize() const noexcept;
        
    private:
        
        //! @brief Retrieves the index of the Node object in the Chain object.
        //! @details If the index is zero, the Node object is out of the Chain or the Chain
        //! @details isn't compiled. All the Node objects connected to the inputs should have
        //! @details a lower index and all the Node objects connected to the outputs should
        //! @details have an upper index.
        //! @return The index of the Node object in the Chain object.
        inline size_t getIndex() const noexcept {return m_index;}
        
        //! @brief Adds a Node object to an input.
        //! @param node The Node object to add.
        //! @param index The index of the input.
        //! @see addOutput()
        void addInput(Node* node, const size_t index);
        
        //! @brief Adds a Node object to an output.
        //! @param node The Node object to add.
        //! @param index The index of the output.
        //! @see addInput()
        void addOutput(Node* node, const size_t index);
        
        //! @brief Prepare the Node object to digital signal processing.
        //! @details The methods retrieves the signals from the inputs Node objects and calls
        //! @details the prepare method of the Processor object.
        //! @see perform()
        void prepare();
        
        //! @brief Ticks once the digital signal processing.
        //! @details This method is called by the Chain during its perform method's call. This
        //! @details method calls the inputs perform methods and the perform method of the
        //! @details Processor object.
        //! @see prepare()
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
