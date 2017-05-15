/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include <map>
#include <queue>

#include "KiwiDsp_Processor.h"
#include "KiwiDsp_Misc.h"

namespace kiwi
{
    namespace dsp
    {
        // ==================================================================================== //
        //                                       CHAIN                                          //
        // ==================================================================================== //
        
        //! @brief An audio rendering class that manages processors in a graph structure.
        //! @details Updating the chain can be updated by incrementingly adding processors and connecting them.
        //! The chain can then be prepared with a certain samplerate and vectorsize making it's tick
        //! function ready to be called in a separate thread.
        //! The chain is transactional that's to say that changes will not be effective
        //! until either prepare or update is called.
        
        class Chain final
        {
        public: // methods
            
            //! @brief The default constructor.
            //! @details Allocates and initializes an empty Chain object.
            //! All the initializations will be performed with the compile method of the Chain object.
            //! @see prepare
            Chain();
            
            //! @brief The destructor.
            //! @details Deallocate memory initialized during prepare and will call every processors release method
            //! as their computation. Note that as processors ownership are shared, chain destructor might not call
            //! processor destructor. As releasing might throw it's better practice to call release
            //! before calling the chain destructor and catch exception if needed.
            ~Chain();
            
            //! @brief Updates the chain making changes effective.
            //! @details As the chain is transactional changes are not effective immediatly. Changes are stored in
            //! a command stack that update will unstack and execute. Updating the chain will keep in it's previous
            //! state that it's to say that is the chain was prepared it will keep it that way. Update can be called
            //! concurrently with tick.
            void update();
            
            //! @brief Allocates memory needed for chain execution.
            //! @details The prepare method will allocate that signal that will passed through the graph.
            //! All processors prepare method will be called. If the prepare method is called twice, the second
            //! call will first release and reallocate the entire graph. If changes have been made to the chain
            //! prepare will make them effective as update would do. Ticking the chain without preparing
            //! is legal but does nothing. Prepare can be called concurretly with tick.
            void prepare(size_t const samplerate, size_t const vectorsize);
            
            //! @brief Deallocate memory needed to perform tick method.
            //! @details Will call release on every processor and will deallocate signal memory.
            //! As release might throw it shal be called before chain destructor. Calling release
            //! will disable chain tick.
            //! @exception Error
            void release();
            
            //! @brief Gets the current sample rate.
            //! @see getVectorSize
            size_t getSampleRate() const noexcept;
            
            //! @brief Gets the current vector size.
            //! @see getSampleRate
            size_t getVectorSize() const noexcept;
            
            //! @brief Adds a processor to the chain.
            //! @details Ownership is shared between caller and chain. The caller might
            //! keep a reference to the processor and update it. Calling addProcessor will add a command
            //! to the stack that will be executed at update or prepare time. Adding the same processor
            //! twice will cause an exception to be thrown at update time.
            void addProcessor(std::shared_ptr<Processor> processor);
            
            //! @brief Removes processors from the chain.
            //! @details Removes processor from the chain. The shared pointer then decrements its count.
            //! The change will be effective as soon as update or prepare is called. Removing a non existing
            //! processor will throw at update time.
            void removeProcessor(Processor& proc);
            
            //! @brief Connects two processors.
            //! @details Connects the source node (upper node) to dest node (lower node) making
            //! signal transimission effective. Connecting an non existing node will result in
            //! exception being called at update time. Connecting might also detect loop and throw.
            void connect(Processor& source, size_t outlet_index,
                         Processor& dest, size_t inlet_index);
            
            //! @brief Disconnect two processors.
            //! @details Disconnect the source node (upper node) from dest node (lower node) disabling
            //! signal transimission.
            void disconnect(Processor& source, size_t outlet_index,
                            Processor& destination, size_t inlet_index);
            
            //! @brief Ticks once all the Processor objects. Not recursive.
            //! @details Call iteratively all the node on their perform method.
            //! if the chain is not prepared the tick will result in doing nothing.
            //! Prepare, release, updates can be made concurrently to tick.
            void tick() noexcept;
            
        private: // classes
            
            //! @brief Chain state regarding preparation.
            //! @details If chain is not prepared tick will do nothing.
            enum class State : uint8_t
            {
                NotPrepared     = 0,    ///< The Chain has not been prepared.
                Preparing       = 1,    ///< The Chain is being prepared.
                Prepared        = 2,    ///< The chain has been prepared.
            };
            
            class Node;
            
        private: // methods
            
            //! @brief Functor called by indexNodes to index a node.
            //! @details To avoid too many recursion on the graph, index_node will also
            //! detect loops.
            struct index_node;
            
            //! @brief Funtor that checks if the node countains the processor
            //! @detail Used to retrieve the node that have a certain processor in findNode;
            struct compare_proc;
            
            //! @brief Returns an iterator to the object having processor proc.
            std::vector<std::unique_ptr<Node>>::iterator findNode(Processor& proc);
            
            //! @brief Returns an const iterator to the object having node_id.
            std::vector<std::unique_ptr<Node>>::const_iterator findNode(Processor& proc) const;
            
            //! @brief Attributes an index to the node before sorting by index.
            //! @details Indexing guarantees that every node will have a greater index than
            //! than all its parent node. Called during prepare.
            void indexNodes();
            
            //! @brief Sorts all node by index before computing.
            //! @details Sorting node will guarantee that all parent node will be executed
            //! before a chil node execution.
            void sortNodes();
            
        private: // commands
            
            //! @brief The command that will making adding a processor effective.
            void execAddProcessor(std::shared_ptr<Processor> proc);
            
            //! @brief The command that will making removing a processor effective.
            void execRemoveProcessor(Processor* proc);
            
            //! @brief The command that will making connection effective.
            void execConnect(Processor* source, size_t outlet_index,
                             Processor* dest, size_t inlet_index);
            
            //! @brief The command that will making disconnection effective.
            void execDisconnect(Processor* source, size_t outlet_index,
                                Processor* dest, size_t inlet_index);
            
            //! @brief Puts back a node in to the commands' list in order to add it again.
            //! @details If a nodes processor doesn't require compuration, for optimization concern,
            //! we remove it from the chain but it back into the command list. If the node status changes afterward
            //! the chain may be able to reinsert it into the chain.
            void restackNode(Node & node);
            
            //! @brief Returns a allocated signal for disconnected inlets
            //! @details Since disconnected inlet never modify their signal they can share the same signal.
            //! @details The first disconnected inlet to ask this will cause it's allocation.
            std::shared_ptr<Signal> getSignalIn();
            
            //! @brief Returns a allocated signal for disconnected outlets.
            //! @details Since disconnected outlets signals are never used after they're computed and never read,
            //! @details we can optimize and feed them the same signal.
            std::shared_ptr<Signal> getSignalOutlet(size_t outlet_index);
            
            //! @brief Returns an allocated signal for fanning inlets.
            //! @details Since fanning inlets always copy their parent node signal before computing.
            //! @details we can optimize and feed them the same buffer which will be overwritten before node computation.
            std::shared_ptr<Signal> getSignalInlet(size_t inlet_index);
            
        private: // members
            
            std::vector<std::unique_ptr<Node>>          m_nodes;
            size_t                                      m_sample_rate;
            size_t                                      m_vector_size;
            State                                       m_state;
            std::deque<std::function<void(void)>>       m_commands;
            std::mutex                                  m_tick_mutex;
            
            std::weak_ptr<Signal>                       m_signal_in;
            std::map<size_t, std::weak_ptr<Signal>>     m_signal_outlet;
            std::map<size_t, std::weak_ptr<Signal>>     m_signal_inlet;
        };
        
        // ================================================================================ //
        //                                      NODE                                        //
        // ================================================================================ //
        
        //! @brief The Node object wraps and manages a Processor object inside a Chain object.
        //! @details The node can be connected to other node and enables signal data
        //! to go through its pins (inlet, outlet). Node also manages its signal allocation via
        //! its prepare and release method.
        class Chain::Node final
        {
        public: // typedefs
            
            using uPtr = std::unique_ptr<Node>;
            
        public: // methods
            
            //! @brief Constructor.
            Node(std::shared_ptr<Processor> processor);
            
            // @brief Destructor.
            ~Node();
            
            //! @brief Connects the node input pin to another node output pin.
            //! @details Returns true didn't exist yet and was effectively inserted.
            bool connectInput(const size_t input_index, Node& other_node, const size_t output_index);
            
            //! @brief Disconnect the node input pin from another node output pin.
            //! @details Returns true existed and was effectively removed.
            bool disconnectInput(const size_t input_index, Node& other_node, const size_t output_index);
            
            //! @brief Prepare the Node object.
            //! @details Allocates and bind its memory before its computation. Call its processor prepare method.
            bool prepare(Chain& chain);
            
            //! @brief The digital signal processing perform method.
            //! @details Feeds the processor a buffer of sample to be processed.
            void perform() noexcept;
            
            //! @brief Releases the node disabling performing it.
            void release();
            
        private: // nested classes
            
            class Pin;
            class Tie;
            
        private: // members
            
            std::shared_ptr<Processor>                  m_processor;
            std::vector<Pin>                            m_inlets;
            std::vector<Pin>                            m_outlets;
            Buffer                                      m_inputs;
            Buffer                                      m_outputs;
            std::vector<Buffer>                         m_buffer_copy;
            size_t                                      m_index;
            
        private: // deleted methods
            
            Node(Node const& other) = delete;
            Node(Node && other) = delete;
            Node& operator=(Node const& other) = delete;
            Node& operator=(Node && other) = delete;
            
            friend class Chain;
        };
        
        // ================================================================================ //
        //                                    NODE::PIN                                     //
        // ================================================================================ //
        
        //! @brief The pin helds a set of tie that points to other pins.
        class Chain::Node::Pin
        {
        public: // methods
            
            //! @brief Constructor.
            //! @param owner    The Node that holds this Pin.
            //! @param index    The index of the Pin.
            Pin(Node& owner, const size_t index);
            
            //! @brief move constructor.
            //! @details Destructing pin will cause its deconnection.
            Pin(Pin && other);
            
            //! @brief Destructor.
            virtual ~Pin();
            
            //! @brief Connects the inlet to a node outlet.
            //! @return Returns true if the connection didn't exist and was inserted.
            bool connect(Pin& other_pin);
            
            //! @brief Disconnects the nodes.
            //! @return Returns true if the connection existed and was removed..
            bool disconnect(Pin& other_pin);
            
            //! @brief Removes all connection from the pin.
            void disconnect();
            
        public: // members
            
            Node&                                                       m_owner;
            const size_t                                                m_index;
            Signal::sPtr                                                m_signal;
            std::set<Node::Tie>                                         m_ties;
            
        private: // deleted methods
            
            Pin(Pin const& other) = delete;
            Pin& operator=(Pin const& other) = delete;
            
            friend class Node;
        };
        
        // ==================================================================================== //
        //                                   NODE::CONNECTION                                   //
        // ==================================================================================== //
        
        //! @brief A tie is a reference held by a pin to another pin.
        class Chain::Node::Tie final
        {
        public: // methods
            
            //! @brief Constructor with the pin that will be referenced.
            Tie(Pin& pin);
            
            //! @brief Copy constructor of Tie.
            Tie(Tie const& other) = default;
            
            //! @brief Comparison operator. Compares pin and pins' indexes.
            bool operator<(Tie const& other) const noexcept;
            
            ~Tie() = default;
            
        public: // members
            
            Pin & m_pin;
            
        private: // deleted methods
            
            Tie() = delete;
        };
        
        // ==================================================================================== //
        //                                          LOOPERROR                                   //
        // ==================================================================================== //
        //! @brief An exception to detect loops.
        //! @details An exception that is thrown whenever a loop is detected in a chain.
        //! @todo Add more infos about the detected loop.
        class LoopError : public Error
        {
        public:
            //! @brief The std::string constructor.
            //! @param message The message of the error
            explicit LoopError(const std::string& message) :
            Error("looperror: " + message)
            {}
            
            //! @brief The const char* constructor.
            //! @param message The message of the error
            explicit LoopError(const char* message) :
            Error(std::string("looperror: ") + std::string(message)) {}
            
            //! @brief The destructor.
            ~LoopError() noexcept override = default;
        };
    }
}
