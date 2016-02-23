//
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//

#ifndef KIWI_DSP_PROCESSOR_H_INCLUDED
#define KIWI_DSP_PROCESSOR_H_INCLUDED

#include "KiwiDspInfos.hpp"

namespace kiwi
{
namespace dsp
{
    class Chain;
    // ==================================================================================== //
    //                                      PROCESSOR                                       //
    // ==================================================================================== //
    //! @brief The pure virtual class that processes digital signal in a Chain object.
    //! @details The class is pure virtual and allows to implement digital signal processing.
    //! You should implement the pure virtual methods prepare, perform and release.
    //! @see Buffer and Infos
    class Processor
    {
    public:
        //! @brief The constructor.
        //! @details Allocates and initializes a default Processor object.
        //! @param ninputs The number of inputs.
        //! @param noutputs The number of outputs.
        Processor(const size_t ninputs, const size_t noutputs) noexcept;
        
        //! @brief The destructor.
        virtual ~Processor() noexcept;
        
        //! @brief Gets the current number of inputs.
        //! @return The number of inputs of the Processor object.
        inline size_t getNumberOfInputs() const noexcept {return m_ninputs;}
        
        //! @brief Gets the current number of outputs.
        //! @return The number of outputs of the Processor object.
        inline size_t getNumberOfOutputs() const noexcept {return m_noutputs;}
        
    private:
        //! @brief Prepares everything for the perform method.
        //! @details This is a pure virtual method. You should use this method to check the
        //! vector size, the sample rate, the connected inputs and outputs and to allocate
        //! memory if needed. The method should return true if the perform method of the
        //! Processor object can be called, otherwise it should return false.
        //! @param infos The DSP informations.
        //! @return true if the perform method can be called, otherwise false.
        //! @see release()
        virtual bool prepare(Infos const& infos) = 0;
        
        //! @brief Performs the digital signal processing.
        //! @details This is a pure virtual method. You should use this method to perform the
        //! digital signal processing but you should avoid to allocate memory or do it
        //! asynchronously.
        //! @param input    The input Buffer object.
        //! @param output   The output Buffer object.
        virtual void perform(Buffer const& input, Buffer& output) noexcept = 0;
        
        //! @brief Releases everything after the digital signal processing.
        //! @details You can use this method to free the memory allocated during the call of
        //! the prepare method for example.
        //! @see prepare()
        virtual void release() {};
        
        const size_t        m_ninputs;
        const size_t        m_noutputs;
        std::atomic< bool > m_used;
        friend class Chain;
    };
}
}

#endif // KIWI_DSP_PROCESSOR_H_INCLUDED
