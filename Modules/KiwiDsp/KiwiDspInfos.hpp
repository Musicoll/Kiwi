//
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//

#ifndef KIWI_DSP_INFOS_H_INCLUDED
#define KIWI_DSP_INFOS_H_INCLUDED

#include "KiwiDspSignal.hpp"

namespace kiwi
{
namespace dsp
{
    class Infos
    {
    public:
        //! @brief The destructor.
        //! @details Frees the content of the Infos object if needed.
        ~Infos();
        
        //! @brief Gets the sample rate of the DSP.
        //! @details Gets the sample rate of the DSP used during the perform method of the
        //! @details Processor object.
        //! @return The vector size.
        //! @see getSampleRate()
        size_t getSampleRate() const noexcept;
        
        //! @brief Gets the vector size of the samples vectors.
        //! @details Gets the vectors size of the samples vectors used during the perform
        //! @details method of the Processor object.
        //! @return The vector size.
        //! @see getSampleRate()
        size_t getVectorSize() const noexcept;
        
        //! @brief Checks if an inlet is connected to any other Processor object.
        //! @details This method can be used to check if an inlet is connected to any other
        //! @details Processor object and to adapt the behavior of the Processor.
        //! @return true if the outlet is connected, otherwise it returns false.
        //! @see isOutputConnected()
        bool isInputConnected(const size_t index) const;
        
        //! @brief Checks if an outlet is connected to any other Processor object.
        //! @details This method can be used to check if an outlet is connected to any other
        //! @details Processor object and to adapt the behavior of the Processor.
        //! @return true if the outlet is connected, otherwise it returns false.
        //! @see isInputConnected()
        bool isOutputConnected(const size_t index) const;
        
    private:
        
        //! @brief The constructor.
        //! @details Allocates a Infos object that describes the behavior of the DSP of a
        //! @details Processor object.
        //! @param samplerate The sample rate.
        //! @param vectorsize The vector size.
        //! @param inputs     The inputs connection states.
        //! @param outputs    The outputs connection states.
        Infos(const size_t samplerate, const size_t vectorsize,
              std::vector< bool > const& inputs, std::vector< bool > const& outputs);
        
        const size_t              m_sample_rate;
        const size_t              m_vector_size;
        const std::vector< bool > m_inputs;
        const std::vector< bool > m_outputs;
        friend class Chain;
    };
}
}

#endif // KIWI_DSP_INFOS_H_INCLUDED
