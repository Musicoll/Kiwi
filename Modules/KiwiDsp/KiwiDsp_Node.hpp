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
        //! @brief The possible states regarding preparation.
        //! @see getState
        enum class PrepareState : uint8_t
        {
            NotPrepared     = 0,    ///< The Node has not been prepared.
            Preparing       = 1,    ///< The Node is preparing.
            Prepared        = 2,    ///< The Node has been prepared.
        };
        
        //! @brief The possible states of a node regarding its release.
        //! @see getState
        enum class ReleaseState : uint8_t
        {
            NotReleased     = 0,
            Releasing       = 1,    ///< Ig the processor was released.
            Released        = 2,
        };
        
        //! @brief The possible states of a node regarding its release.
        //! @see getState
        enum class PerformState : uint8_t
        {
            NotReady        = 0,    ///< The Node is not yet ready to perform.
            Ready           = 1,    ///< The is ready to perform.
            Performing      = 2,    ///< The Node is performing.
            Performed       = 3,    ///< The Node has been performed.
        };
        
        // ================================================================================ //
        //                                      NODE                                        //
        // ================================================================================ //
        
        //! @brief The Node object wraps and manages a Processor object inside a Chain object.
        class Node final
        {
        public:
            
            using uPtr = std::unique_ptr<Node>;
            
        public: // methods
            
            //! @brief Constructor.
            Node(std::shared_ptr<Processor> processor, uint64_t id);
            
            // @brief Destructor.
            ~Node();
            
            //! @brief Gets the Processor object of the Node object.
            std::shared_ptr<Processor> getProcessor() noexcept;
            
            //! @brief Gets the Processor object of the Node object.
            std::shared_ptr<const Processor> getProcessor() const noexcept;
            
            //! brief Returns the id of the node.
            uint64_t getId() const noexcept;
            
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
            
            //! @brief Try to connect to another node in input
            bool connectInput(const size_t input_index, Node& other_node, const size_t output_index);
            
            //! @brief Tries to remove a connection to another node.
            bool disconnectInput(const size_t input_index, Node& other_node, const size_t output_index);
            
            //! @brief Returns the current prepare state of the node
            PrepareState getPrepareState() const noexcept;
            
            //! @brief Sets the node as not prepared enabling prepare method to be called.
            void setAsNotPrepared() noexcept;
            
            //! @brief Sets the node as deleted. Node will be deleted at next prepare.
            void setAsDeleted() noexcept;
            
            //! @brief Return true if the node as been mark as deleted.
            bool isDeleted() const noexcept;
            
            //! @brief Prepare the Node object.
            //! @brief Allocates IOBuffer after a call to the Processor::prepare() method.
            //! @param samplerate The sample rate of the digital signal processing.
            //! @param vectorsize The vector size of the digital signal processing.
            //! @returns True if the perform method of the Node can be called.
            bool prepare(const size_t samplerate, const size_t vectorsize);
            
            //! @brief Call pusblish on its preprocessor. Called outside the dsp thread to publish infos after prepare.
            void publish();
            
            //! @brief The digital signal processing perform method.
            void perform() noexcept;
            
            //! @brief Releases the node disabling performing it.
            void release();
            
        private: // nested classes
            
            class Pin;
            class Inlet;
            class Outlet;
            class Connection;
        
        public: // methods
            
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
            
            //! @internal Returns true if the two given node can be connected without forming a loop.
            bool canConnectWithoutLoop(Node const& other_node);
            
            //! @brief Returns true if one of the inlets is recursively connected to node
            bool hasBackwardPathTo(Node const& node) const noexcept;
            
            //! @brief Returns the number of connections held by the inlets.
            size_t getNumberOfInputConnections() const noexcept;
            
            //! @brief Returns the number of connections held by the outlets.
            size_t getNumberOfOutputConnections() const noexcept;
            
            //! @internal Returns a Node Inlet.
            Inlet const& getInlet(const size_t index) const noexcept;
            
            //! @internal Returns a Node Outlet.
            Outlet const& getOutlet(const size_t index) const noexcept;
            
        private:
            
            //! @internal Returns a Node Inlet.
            Inlet& getInlet(const size_t index) noexcept;
            
            //! @internal Returns a Node Outlet.
            Outlet& getOutlet(const size_t index) noexcept;
            
            //! @brief Marks all the connections as disconnected so that they can removed while preparing.
            void setAsDisconnected();
            
        private: // members
            
            bool                                    m_want_perform;
            bool                                    m_is_deleted;
            std::atomic<PrepareState>               m_prepare_state;
            std::atomic<PerformState>               m_perform_state;
            std::atomic<ReleaseState>               m_release_state;
            std::shared_ptr<Processor>              m_processor;
            std::vector<Inlet>                      m_inlets;
            std::vector<Outlet>                     m_outlets;
            std::unique_ptr<const Buffer>           m_inputs;
            std::unique_ptr<Buffer>                 m_outputs;
            uint64_t                                m_id;
        };
        
        // ================================================================================ //
        //                                    NODE::PIN                                     //
        // ================================================================================ //
        
        //! @brief the Pin is the base class of the Inlet and Outlet.
        class Node::Pin
        {
        public: // Classes
            
            struct compare_cnx
            {
                bool operator()(std::shared_ptr<const Connection> left_cnx,
                                std::shared_ptr<const Connection> right_cnx) const;
            };
            
        public: // methods
            
            //! @brief Constructor.
            //! @param owner    The Node that holds this Pin.
            //! @param index    The index of the Pin.
            Pin(Node& owner, const size_t index);
            
            //! @brief Copy constructor.
            Pin(Pin const& other);
            
            //! @brief Destructor.
            virtual ~Pin();
            
            //! @brief Returns the inlet index
            size_t getIndex() const noexcept;
            
            //! @brief Returns true if this Pin is connected to one or more other Pin.
            bool isConnected() const noexcept;
            
            //! @brief Prepares everything for the perform method.
            virtual void prepare(Infos const& infos) = 0;
            
            //! @brief Releases all datas acquired for performing.
            virtual void release() = 0;
            
            //! @brief Returns true if the Pin is inplace and sets the corresonding pin index.
            virtual bool isInplace(size_t * const index) = 0;
            
            //! @brief Returns true if the Pin is inplace and sets the corresonding pin index.
            virtual void setAsNotPrepared() = 0;
            
            //! Sets all connection to disconnected so that they can removed later.
            void setAsDisconnected();
            
            //! @internal Returns the number of connections this Pin have.
            const size_t getNumberOfConnections() const noexcept;
            
            //! @internal Returns the Signal hold by this Pin.
            Signal::sPtr getSignal() const noexcept;
            
            //! @internal Fill signal with zero (only if this Pin is owning it).
            //! @details Aims to be called by the chain to zeroes buffer before each perform call.
            void clean();
            
            //! @brief Adds a connection to the pin. Returns true if the connection was sucessfully inserted.
            bool addConnection(std::shared_ptr<Connection> connection);
            
            //! @brief Removes a connection to the pin. Returns true if the connection was sucessfully removed.
            bool removeConnection(std::shared_ptr<Connection> connection);
            
            //! @brief Removes all connection from the pin.
            virtual void disconnect() = 0;
            
        protected: // methods
            
            //! @brief Returns the Node that owns this Pin.
            Node const& getOwner() const noexcept;
            
            //! @brief Returns the Node that owns this Pin.
            Node& getOwner() noexcept;
            
        protected: // members
            
            Node&                                                       m_owner;
            const size_t                                                m_index;
            Signal::sPtr                                                m_signal;
            bool                                                        m_signal_owner;
            std::atomic<PrepareState>                                   m_prepare_state;
            std::atomic<ReleaseState>                                   m_release_state;
            std::set<std::shared_ptr<Connection>, compare_cnx>          m_connections;
            
            friend Node::Connection;
        };
        
        // ================================================================================ //
        //                                   NODE::INLET                                    //
        // ================================================================================ //
        
        //! @brief An InletPin holds an output Signal that can be provided or by from multiple input signals.
        class Node::Inlet final : public Node::Pin
        {
        public: // methods
            
            //! @brief Constructor.
            //! @param owner  The Node that holds this Pin.
            //! @param index  The index of the Pin.
            Inlet(Node& owner, const size_t index);
            
            //! @brief Copying and assigning a new owner to the copy
            Inlet(Inlet const& other);
            
            //! @brief Destructor.
            ~Inlet();
            
            //! @brief Connects the inlet to a node outlet. Returns true if the connection didn't exist and was inserted.
            bool connect(Node& node, const size_t index);
            
            //! @brief Disconnects the nodes
            bool disconnect(Node& node, const size_t index);
            
            //! @brief Disconnectes all connections in inlet.
            void disconnect() override;
            
            //! @brief Returns true if the Inlet is recursively connected to node.
            //! @details The search of a path is made backward.
            bool hasBackwardPathTo(Node const& node) const noexcept;
            
            //! Sets the inlet as not prepared. Enable preparing it again.
            void setAsNotPrepared() override;
            
            //! @brief Prepares everything for the perform method.
            void prepare(Infos const& infos) override;
            
            //! @brief Releases all datas acquired for performing.
            void release() override;
            
            //! @brief Returns true if the Inlet is inplace and sets the corresponding outlet index
            virtual bool isInplace(size_t * const outlet_index = nullptr) override;
            
            //! @brief The perform method.
            void perform();
            
        private: // methods
            
            //! @brief Iterate over connections and tries to acquired a read signal.
            Signal::sPtr acquireReadSignal();
            
            //! @brief Iterates over connections and tries to acquire a write signal.
            Signal::sPtr acquireWriteSignal();
            
            //! @brief Copies first signal and adds other incomming signal.
            void copyAndAddSignals();
            
            //! @brief Adds incomming signal excpept if it's already acquired.
            void addSignals();
            
            //! @brief Asks the outltes connected to this node to prepare.
            void prepareConnections(Infos const& infos) const;
            
            //! @brief Calls perform on all upstream nodes.
            void performConnections();
            
            //! Resets connection so that it can acquire signal again.
            void resetConnections();
            
            //! Unlock connections so that they can be performed again.
            void unlockConnections();
            
        private: // members
            
            std::function<void()>   m_pull_method;
        };
        
        // ================================================================================ //
        //                                  NODE::OUTLET                                    //
        // ================================================================================ //
        
        //! @brief An OutletPin holds an input Signal processed by the processor.
        class Node::Outlet final : public Node::Pin
        {
        public: // methods
            
            //! @brief Constructor.
            //! @param owner    The Node that holds this Pin.
            //! @param index    The index of the Pin.
            Outlet(Node& owner, const size_t index);
            
            //! @brief Copying and assigning a new owner to the copy
            Outlet(Outlet const& other);
            
            //! @brief Destructor.
            ~Outlet();
            
            //! @brief Sets the outlet as not prepared. Enable preparing it again.
            void setAsNotPrepared() override;
            
            //! @brief Disconnects all connections from outlet
            void disconnect() override;
            
            //! @brief Prepares everything for the perform method.
            void prepare(Infos const& infos) override;
            
            //! @brief Releases all datas acquired for performing.
            void release() override;
            
            //! @brief Returns true if the Outlet is inplace and sets the corresponding outlet inlet_index.
            virtual bool isInplace(size_t* const inlet_index = nullptr) override;
            
            //! @brief Returns true if one of the connection has requested a write pointer.
            bool isWriteAcquired() const;
            
            //! @brief Returns true if one of the connection has requested a read pointer.
            bool isReadAcquired() const;
        };
        
        // ==================================================================================== //
        //                                   NODE::CONNECTION                                   //
        // ==================================================================================== //
        
        //! @brief A connection is held by inlets and outlets to communicate throught it
        class Node::Connection final
        {
        public: // methods
            
            //! @brief Constructor an inlet and an outlet. Ownership is not taken.
            Connection(Inlet& inlet, Outlet& outlet);
            
            //! @brief Comparison operator. Compares inlet and outlet.
            bool operator<(Connection const& other) const noexcept;
            
            //! @brief Returns true if a path can be found recursively from the m_outlet.
            bool hasBackwardPathTo(Node const& node) const noexcept;
            
            //! @brief Calls prepare on the outlet node.
            bool prepare(const size_t samplerate, const size_t vectorsize);
            
            //! @brief Calls perform on the upstream outlet node.
            void perform();
            
            //! @brief Returns true if a read pointer as already been required.
            bool isReadAcquired() const;
            
            //! @brief Returns true if a write pointer as already been required.
            bool isWriteAcquired() const;
            
            //! @brief Returns a write pointer if it can be acquired.
            Signal::sPtr acquireWriteSignal();
            
            //! @brief Returns a read pointer if it can be acquired.
            Signal::sPtr acquireReadSignal();
            
            //! @brief Returns the pointer held by the outlet
            Signal::scPtr getSignal() const;
            
            //! @brief Disconnect the connection removing it from outlet and inlet
            bool disconnect();
            
            //! @brief Connect connection
            bool connect();
            
            //! @brief Sets the connection as disconnected.
            //! @details Calls to disconnect will then successfully remove the connection.
            void setAsDisconnected() noexcept;
            
            //! @brief Returns true if the connection can be removed.
            bool isDisconnected() const noexcept;
            
            //! @brief Locks the connection disabling pulling signal and performing the connection.
            void lock() noexcept;
            
            //! @brief Locks the connection enabling pulling signal and performing the connection.
            void unlock() noexcept;
            
            //! @brief Locks the connection enabling pulling signal throw it.
            bool isLocked() const noexcept;
            
            //! @brief Reset connection. Enables to require write and read pointer
            void reset() noexcept;
            
            //! @brief Returns the inlet index
            size_t getInletIndex() const noexcept;
            
            //! @brief Returns the outlet index
            size_t getOutletIndex() const noexcept;
            
            //! @brief Returns the Node Inlet.
            Inlet& getInlet() noexcept;
            
            //! @brief Returns the Node Outlet.
            Outlet& getOutlet() noexcept;
            
        private: // members
            
            Inlet&              m_inlet;
            Outlet&             m_outlet;
            
            bool                m_is_write_acquired;
            bool                m_is_read_acquired;
            
            bool                m_is_locked;
            bool                m_is_disconnected;
            
        private: // deleted methods
            
            Connection() = delete;
        };
    }
}

#endif // KIWI_DSP_NODE_HPP_INCLUDED
