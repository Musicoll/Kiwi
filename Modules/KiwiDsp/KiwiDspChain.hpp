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
    //! @details The class acts like a container for Processor and Link objects. After
    //! @details retrieving the Processor and Link objects, the class compiles the chain by
    //! @details connecting and sorting the Processor objects depending on the Link objects.
    //! @details If the compilation has been processed without throwing any exception, the
    //! @details Chain object can tick the digital signal processing. The methods should be
    //! @details called in a thread safe context. Indeed, the release or compile methods
    //! @details should never be called during the tick call or a Processor object should
    //! @details never be detroyed during the compilation or the processing of the Chain
    //! @details object.
    //! @code zaza
    //! @todo make it a little bit more thread safe
    class Chain
    {
    public:
        //! @brief The default constructor.
        //! @details Allocates and initializes an empty Chain object. All the initializations
        //! @details will be performed with the compile method of the Chain object.
        //! @see compile
        Chain();
        
        //! @brief The destructor.
        //! @details Frees the Chain object and releases the digital signal processing if
        //! @details needed. Nevertheless the release method should always be called before
        //! @details the destruction in the case where exception are thrown by the Processor
        //! @details objects while they release methods are called.
        //! @see release
        //! @exception Error
        ~Chain();
        
        //! @brief Gets the current sample rate.
        //! @see getVectorSize, isProcessing
        size_t getSampleRate() const noexcept;
        
        //! @brief Gets the current vector size.
        //! @see getSampleRate, isProcessing
        size_t getVectorSize() const noexcept;
        
        //! @brief Gets the state.
        //! @see getSampleRate, getVectorSize
        bool isProcessing() const noexcept;
        
        //! @brief Compiles the dsp chain.
        //! @details The function sorts Processor objects and call their prepares methods.
        //! @param samplerate   The sample rate of the digital signal processing.
        //! @param vectorsize   The vector size of the digital signal processing.
        //! @param processors   The set of Processor objects to add to the Chain object.
        //! @param links        The set of Links objects used to connect the Processor objects.
        //! @see tick, release
        //! @exception Error
        void compile(size_t const samplerate, size_t const vectorsize,
                     std::set< Processor * > const& processors,
                     std::set< Link * > const& links);
        
        //! @brief Stops the digital signal processing.
        //! @see compile, tick
        //! @exception Error
        void release();
        
        //! @brief Ticks once all the Processor objects.
        //! @see compile, release
        void tick() const noexcept;
        
    private:
        class Node;
        class Tie;
        
        std::vector< std::shared_ptr< Node > >  m_nodes;
        std::vector< std::shared_ptr< Tie > >   m_ties;
        bool                                    m_processing;
        size_t                                  m_sample_rate;
        size_t                                  m_vector_size;
    };
}
}

#endif // KIWI_DSP_CHAIN_H_INCLUDED
