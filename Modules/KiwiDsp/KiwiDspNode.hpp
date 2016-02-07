//
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//

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
    public:
        //! @brief The constructor.
        //! @details Allocates and initializes a Node object for a Processor object.
        //! @param chain The Chain object that mamanges the Node object.
        //! @param processor The Processor object managed by the Node object.
        Node(Chain const& chain, Processor& processor);
        
        //! @brief The destructor.
        ~Node();
        
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
        
        //! @brief Gets the current sample rate of the chain.
        inline size_t getSampleRate() const noexcept {return m_sample_rate;}
        
        //! @brief Gets the current vector size of the chain.
        inline size_t getVectorSize() const noexcept {return m_vector_size;}
        
        //! @brief Gets the vector of samples of an input.
        inline sample const* getInputSamples(const size_t index) const noexcept
        {
            assert(index < m_inputs.size() && "Kiwi::Dsp::Node : Index must be inferior to the number of inputs.");
            return m_buffer_in+index * m_vector_size;
        }
        
        //! @brief Gets the vector of samples of the inputs.
        inline sample const* getInputsSamples() const noexcept {return m_buffer_in;}
        
        //! @brief Gets the vector of samples of an output.
        inline sample* getOutputSamples(const size_t index) const noexcept
        {
            assert(index < m_outputs.size() && "Kiwi::Dsp::Node : Index must be inferior to the number of outputs.");
            return m_buffer_out+index * m_vector_size;
        }
        
        //! @brief Gets the vector of samples of the inputs.
        inline sample* getOutputsSamples() const noexcept {return m_buffer_out;}
        
    private:
        
        //! @brief Adds a Node object to an input.
        //! @param node The Node object to add.
        //! @param index The index of the input.
        //! @see addOutput()
        void addInput(std::shared_ptr< Node > node, const size_t index);
        
        //! @brief Adds a Node object to an output.
        //! @param node The Node object to add.
        //! @param index The index of the output.
        //! @see addInput()
        void addOutput(std::shared_ptr< Node > node, const size_t index);
        
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
        void perform() const noexcept;
        
        Chain const&                        m_chain;
        Processor&                          m_processor;
        size_t                              m_sample_rate;
        size_t                              m_vector_size;
        sample*                             m_buffer_in;
        sample*                             m_buffer_out;
        size_t                              m_index;
        bool                                m_valid;
        std::vector< std::vector< sample const* > >  m_buffer_copy;
        std::vector< std::set< std::weak_ptr< Node >, std::owner_less< std::weak_ptr< Node > > > > m_inputs;
        std::vector< std::set< std::weak_ptr< Node >, std::owner_less< std::weak_ptr< Node > > > > m_outputs;
        friend class Chain;
    };
}
}

#endif // KIWI_DSP_NODE_H_INCLUDED
