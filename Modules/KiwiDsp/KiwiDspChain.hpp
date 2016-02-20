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
        
        //! @brief The possible states of a Chain object.
        //! @see getState
        enum class State : uint8_t
        {
            NotCompiled = 0, ///< If the object chain is not compiled.
            Compiling   = 1, ///< If the object chain is compiling.
            Processing  = 2, ///< If the object chain is processing.
            Waiting     = 3  ///< If the object chain is waiting the next processing.
        };
        
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
        
        //! @brief Gets the current state.
        //! @see getSampleRate, getVectorSize, State
        State getState() const noexcept;
        
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
        std::atomic< State >                    m_state;
        std::mutex                              m_mutex;
        size_t                                  m_sample_rate;
        size_t                                  m_vector_size;
    };
    
    // ==================================================================================== //
    //                                      CHAIN::TIE                                      //
    // ==================================================================================== //
    //! @brief The class that manages a Link object inside a Chain object.
    class Chain::Tie : public std::enable_shared_from_this< Tie >
    {
    public:
        //! @brief The constructor that checks the validity of the connection.
        Tie(Link& link, std::vector< std::shared_ptr< Node> > const& nodes);
        
        //! @brief Gets the Link object.
        Link const& getLink() const noexcept;
        
        //! @brief Gets the Link object.
        Link& getLink() noexcept;
        
        //! @brief Connects the Node objects together.
        void connect() const;
        
        //! @brief Gets the output Node object.
        std::shared_ptr< const Node > getOutputNode() const noexcept;
        
        //! @brief Gets the input Node object.
        std::shared_ptr< const Node >  getInputNode() const noexcept;
        
        //! @brief Gets the index of the output Node object.
        size_t getOutputIndex() const noexcept;
        
        //! @brief Gets the index of the input Node object.
        size_t getInputIndex() const noexcept;
        
    private:
        Link&                   m_link;
        std::weak_ptr< Node >   m_from;
        std::weak_ptr< Node >   m_to;
        size_t                  m_from_index;
        size_t                  m_to_index;
    };
    
    // ==================================================================================== //
    //                                      CHAIN::NODE                                     //
    // ==================================================================================== //
    //! @brief The class that manages a Processor object inside a Chain object.
    class Chain::Node
    {
    public:
        
        
        Node(Processor& processor, size_t const samplerate, size_t const vectorsize);
        ~Node();
        Processor const& getProcessor() const noexcept;
        void addInput(std::shared_ptr< const  Tie > tie);
        void removeInput(std::shared_ptr< const Tie > tie);
        void addOutput(std::shared_ptr< const Tie > tie);
        void prepare();
        void perform() noexcept;
  
        Processor&              m_processor;
        Buffer                  m_buffer;
        size_t                  m_sample_rate;
        size_t                  m_vector_size;
        size_t                  m_index;
        bool                    m_valid;
        
    private:
        typedef std::set< std::weak_ptr< const Tie > , std::owner_less< std::weak_ptr< const Tie > > > tie_set;
        std::vector< tie_set >  m_inputs;
        std::vector< tie_set >  m_outputs;
    };
}
}

#endif // KIWI_DSP_CHAIN_H_INCLUDED
