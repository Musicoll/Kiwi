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

#include <map>

#include "KiwiDsp_Node.hpp"

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
        class Chain final
        {
        public: // methods
            
            //! @brief The default constructor.
            //! @details Allocates and initializes an empty Chain object.
            //! All the initializations will be performed with the compile method of the Chain object.
            //! @see compile
            Chain();
            
            //! @brief The destructor.
            //! @details Frees the Chain object and releases the digital signal processing if needed.
            //! Nevertheless the release method should always be called before
            //! the destruction in the case where exception are thrown by the Processor
            //! objects while their release methods are called.
            //! @see release
            //! @exception Error
            ~Chain();
            
            //! @brief Prepares all datas before processing.
            //! @details Calls prepare on Processors.
            void prepare(size_t const samplerate, size_t const vectorsize);
            
            //! @brief Call release on nodes enabling processors to deallocate memory.
            //! @details Tick will no longer be called after releasing.
            void release();
            
            //! @brief Gets the current sample rate.
            //! @see getVectorSize, isProcessing
            size_t getSampleRate() const noexcept;
            
            //! @brief Gets the current vector size.
            //! @see getSampleRate, isProcessing
            size_t getVectorSize() const noexcept;
            
            //! @brief Adds a processor to the chain.
            void addProcessor(uint64_t id, std::unique_ptr<Processor> processor);
            
            //! @brief Removes processors from the chain.
            void removeProcessor(uint64_t id);
            
            //! @brief Gets a const processor held by the node with id.
            //! @details Caller shares ownership with chain so it's same to maniplate processor and remoing node.
            std::shared_ptr<const Processor> getProcessor(uint64_t id) const;
            
            //! @brief Gets aprocessor held by the node with id.
            //! @details Caller shares ownership with chain so it's same to maniplate processor and remoing node.
            std::shared_ptr<Processor> getProcessor(uint64_t id);
            
            //! @brief Connects two processors.
            //! @details Returns false if the connection already existed.
            bool connect(uint64_t source_node, size_t outlet_index, uint64_t dest_node, size_t inlet_index);
            
            //! @brief Disconnect two processors.
            //! @details Returns false if the connection didn't existed.
            bool discconnect(uint64_t source_node, size_t outlet_index, uint64_t dest_node, size_t inlet_index);
            
            //! @brief Ticks once all the Processor objects.
            //! @see compile, release
            //! @todo The documentation.
            void tick() noexcept;
            
            //! @brief Cleans all nodes and buffers before ticking again.
            void clean() noexcept;
            
            //! @brief Prepares the node without changing the sample rate and vector size.
            void prepare();
            
        private: // methods
            
            //! @brief Returns true if the processor is already in the graph
            bool findProcessor(Processor const& processor) const;
            
        private: // members
            
            std::map<const uint64_t, Node>  m_nodes;
            size_t                          m_sample_rate;
            size_t                          m_vector_size;
            std::atomic<PrepareState>       m_prepare_state;
            std::atomic<ReleaseState>       m_release_state;
            std::atomic<PerformState>       m_perform_state;
            std::mutex                      m_tick_mutex;
        };
    }
}

#endif // KIWI_DSP_CHAIN_HPP_INCLUDED
