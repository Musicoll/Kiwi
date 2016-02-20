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
        ~Chain();
        
        //! @brief Gets the current sample rate.
        //! @see getVectorSize, isProcessing
        inline size_t getSampleRate() const noexcept {return m_sample_rate;}
        
        //! @brief Gets the current vector size.
        //! @see getSampleRate, isProcessing
        inline size_t getVectorSize() const noexcept {return m_vector_size;}
        
        //! @brief Gets the DSP state.
        //! @see getSampleRate, getVectorSize
        inline bool isProcessing() const noexcept {return m_processing;}
        
        //! @brief Compiles the dsp chain.
        //! @details The function sorts Processor objects and call their prepares methods.
        void compile(size_t const samplerate, size_t const vectorsize,
                     std::vector< Processor * > const& processors,
                     std::vector< Link * > const& links);
        
        //! @brief Stops the digital signal processing.
        void release();
        
        //! @brief Ticks once all the Processor objects.
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
