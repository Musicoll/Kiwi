/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_DSP_CHAIN_HPP_INCLUDED
#define KIWI_DSP_CHAIN_HPP_INCLUDED

#include "KiwiDsp_Node.hpp"
#include "KiwiDsp_Link.hpp"

namespace kiwi
{
    namespace dsp
    {
        // ==================================================================================== //
        //                                       CHAIN                                          //
        // ==================================================================================== //
        
        //! @brief The class manages a set of Processor objects connected by a set Link objects.
        //! @details The class acts like a container for Processor and Link objects. After
        //! retrieving the Processor and Link objects, the class compiles the chain by connecting
        //! and sorting the Processor objects depending on the Link objects. If the compilation
        //! has been processed without throwing any exception, the Chain object can tick the
        //! digital signal processing. The methods should be called in a thread safe context.
        //! @code
        //! Chain chain;
        //! std::set<Processor*> processes;
        //! std::set<Link*> links;
        //! ...
        //! try
        //! {
        //!     chain.compile(44100ul, 64ul, processes, links)
        //! }
        //! catch(std::exception& e)
        //! {
        //!     std::cout e.what();
        //!     return -1;
        //! }
        //! while(...)
        //! {
        //!     chain.tick()
        //! }
        //! try
        //! {
        //!     chain.release()
        //! }
        //! catch(std::exception& e)
        //! {
        //!     std::cout e.what();
        //!     return -1;
        //! }
        //! @endcode
        //! @todo Make it a little bit more thread safe !
        //! @todo Checks exceptions and assertions !
        //! @todo Checks if what must be link and processors to send (unique_ptr ? object ? Pointer ?) !
        class Chain
        {
        public: // methods
            
            //! @brief The possible states of a Chain object.
            //! @see getState
            enum class State : uint8_t
            {
                NotCompiled = 0, ///< If the object chain is not compiled.
                Compiling   = 1, ///< If the object chain is compiling.
                Ready       = 2  ///< If the object chain is ready to process.
            };
            
            //! @brief The default constructor.
            //! @details Allocates and initializes an empty Chain object.
            //! All the initializations will be performed with the compile method of the Chain object.
            //! @see compile
            Chain() noexcept;
            
            //! @brief The destructor.
            //! @details Frees the Chain object and releases the digital signal processing if needed.
            //! Nevertheless the release method should always be called before
            //! the destruction in the case where exception are thrown by the Processor
            //! objects while their release methods are called.
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
            //! @todo The documentation.
            void compile(size_t const samplerate, size_t const vectorsize,
                         std::set<Processor*> const& processors,
                         std::set<Link*> const& links);
            
            //! @brief Stops the digital signal processing.
            //! @see compile, tick
            //! @exception Error
            //! @todo The documentation.
            void release();
            
            //! @brief Ticks once all the Processor objects.
            //! @see compile, release
            //! @todo The documentation.
            void tick() const noexcept;
            
        private: // methods
            
            //! @internal Creates nodes.
            void createNodes(std::set<Processor*> const& processors);
            
            //! @internal connect nodes based on links infos.
            void connectNodes(std::set<Link*> const& links);
            
            //! @internal Prepares nodes.
            void prepareNodes();
            
        private: // members
            
            std::vector<Node::uPtr> m_nodes;
            std::atomic<State>      m_state;
            mutable std::mutex      m_mutex;
            size_t                  m_sample_rate;
            size_t                  m_vector_size;
            
        };    
    }
}

#endif // KIWI_DSP_CHAIN_HPP_INCLUDED
