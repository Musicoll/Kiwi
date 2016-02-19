//
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//

#ifndef KIWI_DSP_CHAIN_H_INCLUDED
#define KIWI_DSP_CHAIN_H_INCLUDED

#include "KiwiDspLink.hpp"
#include "KiwiDspProcessor.hpp"

namespace kiwi
{
namespace dsp
{
    // ==================================================================================== //
    //                                          CHAIN                                       //
    // ==================================================================================== //
    //! @brief The class manages a set of Processor objects connected by a set Link objects.
    //! @details The class acts like a container for Processor and Link objects. After adding
    //! @details the Processor and Link objects, the class compiles the chain  by sorting and
    //! @details connecting the Processor objects depending on the Link objects.
    class Chain
    {
    public:
        //! @brief The constructor.
        //! @details Allocates and initializes an empty Chain.
        Chain();
        
        //! @brief The destructor.
        ~Chain();
        
        //! @brief Gets the current sample rate.
        inline size_t getSampleRate() const noexcept {return m_sample_rate;}
        
        //! @brief Gets the current vector size of the chain.
        inline size_t getVectorSize() const noexcept {return m_vector_size;}
        
        //! @brief Gets the current internal state of the DSP.
        inline bool isRunning() const noexcept {return m_running;}
        
        //! @brief Compiles the dsp chain.
        //! @details The function sorts Processor objects and call their prepares methods.
        void compile(size_t const samplerate, size_t const vectorsize,
                     std::vector< Processor * > const& processors,
                     std::vector< Link * > const& links);
        
        //! @brief Stops the digital signal processing.
        //! @details Stops the digital signal processing of all the Node objects.
        void stop();
        
        //! @brief Ticks once the DSP chain.
        void tick() const noexcept;
        
    private:
        class Node;
        class Tie;
        std::vector< std::shared_ptr< Node > >  m_nodes;
        bool                                    m_running;
        size_t                                  m_sample_rate;
        size_t                                  m_vector_size;
    };
}
}

#endif // KIWI_DSP_CHAIN_H_INCLUDED
