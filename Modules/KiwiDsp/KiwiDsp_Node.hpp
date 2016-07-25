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

#ifndef KIWI_DSP_NODE_HPP_INCLUDED
#define KIWI_DSP_NODE_HPP_INCLUDED

#include "KiwiDsp_Processor.hpp"

namespace kiwi
{
    namespace dsp
    {
        // ================================================================================ //
        //                                      NODE                                        //
        // ================================================================================ //
        
        //! @brief The Node object wraps and manages a Processor object inside a Chain object.
        class Node
        {
        public:
            
            typedef std::unique_ptr<Node> uPtr;
            
            //! @brief The possible states of a Node object.
            //! @see getState
            enum class State : uint8_t
            {
                NotPrepared     = 0,    ///< The Node has not been prepared.
                Preparing       = 1,    ///< The Node is preparing.
                Prepared        = 2,    ///< The Node has been prepared.
                ReadyToPerform  = 3,    ///< The Node is ready to perform.
                Performing      = 4,    ///< The Node is performing.
                Performed       = 5,    ///< The Node has been performed.
            };
            
        public: // methods
            
            //! @brief Constructor.
            Node(Processor& processor);
            
            // @brief Destructor.
            virtual ~Node();
            
            //! @brief Gets the Processor object of the Node object.
            Processor const& getProcessor() const noexcept;
            
            //! @brief Returns the number of Inlet the Node object has.
            size_t getNumberOfInlets() const noexcept;
            
            //! @brief Returns the number of Outlet the Node object has.
            size_t getNumberOfOutlets() const noexcept;
            
            //! @brief Returns true if this is a generator Node.
            //! @brief A generator Node is a Node that has NO input connections.
            bool isGenerator() const noexcept;
            
            //! @brief Returns true if this is a terminal Node.
            //! @brief A terminal Node is a Node that has NO output connections.
            bool isTerminal() const noexcept;
            
            //! @brief Returns true if this is an isolated Node.
            //! @brief An isolated Node is a Node that has NO input NOR output connections.
            bool isIsolated() const noexcept;
            
            //! @brief Returns true if this Node wants its perform method to be called.
            bool wantPerform() const noexcept;
            
            //! @brief Returns a Node State.
            Node::State getState() const noexcept;
            
            //! @brief Try to connect two Node objects.
            static bool connect(Node& from, const size_t from_index,
                                Node& to, const size_t to_index);
            
            //! @brief Prepare the Node object.
            //! @brief Allocates IOBuffer after a call to the Processor::prepare() method.
            //! @param samplerate The sample rate of the digital signal processing.
            //! @param vectorsize The vector size of the digital signal processing.
            //! @returns True if the perform method of the Node can be called.
            bool prepare(const size_t samplerate, const size_t vectorsize);
            
            //! @brief The digital signal processing perform method.
            void perform() noexcept;
            
        private: // nested classes
            
            class Pin;
            class Inlet;
            class Outlet;
        
        private: // methods
            
            //! @internal Clean buffers.
            //! @details Aims to be called by the chain to zeroes buffer before each perform call.
            void clean();
            
            //! @internal Called just before the processor perform method.
            void beforePerform() noexcept;
            
            //! @internal Called just after the processor perform method.
            void afterPerform() noexcept;
            
            //! @internal Prepare and returns the inputs state (connected or not).
            std::vector<bool> getConnectedInletsState();
            
            //! @internal Prepare and returns the outputs state (connected or not).
            std::vector<bool> getConnectedOutletsState();
            
            //! @internal Prepare input and output buffers based on processor infos.
            void prepareBuffers(Infos const& infos);
            
            //! @internal Returns true if this Node has an output connection to the other Node.
            bool isConnectedTo(Node const& node) const noexcept;
            
            //! @internal Returns true if the two given node can be connected without forming a loop.
            static bool canConnectWithoutLoop(Node const& node_from, Node const& node_to);
            
            //! @internal Returns the Node inlets.
            std::vector<Inlet> const& getInlets() const noexcept;
            
            //! @internal Returns a Node Inlet.
            Inlet const& getInlet(const size_t index) const;
            
            //! @internal Returns a Node Inlet.
            Inlet& getInlet(const size_t index);
            
            //! @internal Returns the Node outlets.
            std::vector<Outlet> const& getOutlets() const;
            
            //! @internal Returns a Node Outlet.
            Outlet const& getOutlet(const size_t index) const;
            
            //! @internal Returns a Node Outlet.
            Outlet& getOutlet(const size_t index);
            
            //! @brief Adds an input source to the Node object.
            void addInput(const size_t to_index, Node& from, const size_t from_index);
            
            //! @brief Adds an output source to the Node object.
            void addOutput(const size_t from_index, Node& to, const size_t to_index);
            
        private: // members
            
            bool                    m_has_inlet_connections;
            bool                    m_has_outlet_connections;
            bool                    m_want_perform;
            std::atomic<State>      m_state;
            Processor&              m_processor;
            std::vector<Inlet>      m_inlets;
            std::vector<Outlet>     m_outlets;
            Buffer                  m_inputs;
            Buffer                  m_outputs;
            
            friend Chain;
        };
        
        // ================================================================================ //
        //                                    NODE::PIN                                     //
        // ================================================================================ //
        
        //! @brief the Pin is the base class of the Inlet and Outlet.
        class Node::Pin
        {
        public: // methods
            
            //! @brief Constructor.
            //! @param owner    The Node that holds this Pin.
            //! @param is_input Pass true if this is an input Pin, false otherwise.
            //! @param index    The index of the Pin.
            Pin(Node& owner, bool is_input, const size_t index);
            
            //! @brief Destructor.
            virtual ~Pin();
            
            //! @brief Returns the Node that owns this Pin.
            Node& getOwner() noexcept;
            
            //! @brief Returns the Node that owns this Pin.
            Node const& getOwner() const noexcept;
            
            //! @brief Returns the index of this Pin.
            size_t getIndex() const noexcept;
            
            //! @brief Connects the Pin to another node Pin.
            bool connect(Node& node, const size_t index);
            
            //! @brief Returns true if this Pin is connected to one or more other Pin.
            bool isConnected() const noexcept;
            
            //! @brief Prepares everything for the perform method.
            virtual void prepare(Infos const& infos) = 0;
            
            //! @internal Returns true if this Pin is connected to another Node.
            bool isConnectedTo(Node const& node) const noexcept;
            
            //! @internal Returns the number of connections this Pin have.
            const size_t getNumberOfConnections() const noexcept;
            
            //! @internal Returns the Signal hold by this Pin.
            Signal::sPtr const& getSignal() const noexcept;
            
            //! @internal Fill signal with zero (only if this Pin is owning it).
            //! @details Aims to be called by the chain to zeroes buffer before each perform call.
            void clean();
            
        protected: // members
            
            bool                m_is_inlet;
            bool                m_connected;
            bool                m_signal_owner;
            const size_t        m_index;
            Node&               m_owner;
            Signal::sPtr        m_signal;
            std::set<Pin*>      m_connected_pins;
        };
        
        // ================================================================================ //
        //                                   NODE::INLET                                    //
        // ================================================================================ //
        
        //! @brief An InletPin holds an output Signal that can be provided or by from multiple input signals.
        class Node::Inlet : public Node::Pin
        {
        public: // methods
            
            //! @brief Constructor.
            //! @param owner  The Node that holds this Pin.
            //! @param index  The index of the Pin.
            Inlet(Node& owner, const size_t index);
            
            //! @brief Destructor.
            ~Inlet() = default;
            
            //! @brief Prepares everything for the perform method.
            void prepare(Infos const& infos) override;
            
            //! @brief The perform method.
            Signal::sPtr perform();
            
        private: // members
            
            using perform_method_t = std::function<void()>;
            
            std::vector<perform_method_t> m_perform_methods;
        };
        
        // ================================================================================ //
        //                                  NODE::OUTLET                                    //
        // ================================================================================ //
        
        //! @brief An OutletPin holds an input Signal processed by the processor.
        class Node::Outlet : public Node::Pin
        {
        public: // methods
            
            //! @brief Constructor.
            //! @param owner    The Node that holds this Pin.
            //! @param index    The index of the Pin.
            Outlet(Node& owner, const size_t index);
            
            //! @brief Destructor.
            ~Outlet() = default;
            
            //! @brief Prepares everything for the perform method.
            void prepare(Infos const& infos) override;
        };
    }
}

#endif // KIWI_DSP_NODE_HPP_INCLUDED
