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

#include "KiwiDsp_Node.hpp"

namespace kiwi
{
    namespace dsp
    {
        // ================================================================================ //
        //                                      NODE                                        //
        // ================================================================================ //
        
        Node::Node(Processor& processor) :
        m_has_inlet_connections(false),
        m_has_outlet_connections(false),
        m_want_perform(false),
        m_state(Node::State::NotPrepared),
        m_processor(processor),
        m_inputs(m_processor.getNumberOfInputs()),
        m_outputs(m_processor.getNumberOfOutputs())
        {
            const size_t inlets = processor.getNumberOfInputs();
            const size_t outlets = processor.getNumberOfOutputs();
            
            if(inlets)
            {
                m_inlets.reserve(inlets);
                for(size_t i = 0; i < inlets; ++i)
                {
                    m_inlets.emplace_back(*this, i);
                }
            }
            
            if(outlets)
            {
                m_outlets.reserve(outlets);
                for(size_t i = 0; i < outlets; ++i)
                {
                    m_outlets.emplace_back(*this, i);
                }
            }            
        }
        
        Node::~Node()
        {
            if(m_processor.m_used)
            {
                // Pas ça ici
                try
                {
                    m_processor.m_used = false;
                    m_processor.release();
                }
                catch(std::exception&)
                {
                    throw;
                }
            }
        }
        
        Processor const& Node::getProcessor() const noexcept
        {
            return m_processor;
        }
        
        size_t Node::getNumberOfInlets() const noexcept
        {
            return static_cast<size_t>(m_inlets.size());
        }
        
        size_t Node::getNumberOfOutlets() const noexcept
        {
            return static_cast<size_t>(m_outlets.size());
        }
        
        bool Node::isGenerator() const noexcept
        {
            return ! m_has_inlet_connections;
        }
        
        bool Node::isTerminal() const noexcept
        {
            return ! m_has_outlet_connections;
        }
        
        bool Node::isIsolated() const noexcept
        {
            return isTerminal() && isGenerator();
        }
        
        bool Node::wantPerform() const noexcept
        {
            return m_want_perform;
        }
        
        Node::State Node::getState() const noexcept
        {
            return m_state.load();
        }
        
        bool Node::connect(Node& from, const size_t from_index,
                           Node& to, const size_t to_index)
        {
            bool success = false;
            if(canConnectWithoutLoop(from, to))
            {
                from.addOutput(from_index, to, to_index);
                to.addInput(to_index, from, from_index);
                success = true;
            }
            else
            {
                throw Error("A loop is detected");
            }
            
            return success;
        }
        
        bool Node::canConnectWithoutLoop(Node const& node_from, Node const& node_to)
        {
            return ! node_to.isConnectedTo(node_from);
        }
        
        std::vector<Node::Inlet> const& Node::getInlets() const noexcept
        {
            return m_inlets;
        }
        
        Node::Inlet const& Node::getInlet(const size_t index) const
        {
            assert(index < m_inlets.size() && "Index out of range.");
            
            return m_inlets[index];
        }
        
        Node::Inlet& Node::getInlet(const size_t index)
        {
            assert(index < m_inlets.size() && "Index out of range.");
            
            return m_inlets[index];
        }
        
        std::vector<Node::Outlet> const& Node::getOutlets() const
        {
            return m_outlets;
        }
        
        Node::Outlet const& Node::getOutlet(const size_t index) const
        {
            assert(index < m_outlets.size() && "Index out of range.");
            
            return m_outlets[index];
        }
        
        Node::Outlet& Node::getOutlet(const size_t index)
        {
            assert(index < m_outlets.size() && "Index out of range.");
            
            return m_outlets[index];
        }
        
        void Node::addInput(const size_t to_index, Node& from, const size_t from_index)
        {
            assert(to_index < m_inlets.size() && "Index out of range.");
            
            m_inlets[to_index].connect(from, from_index);
            
            // mark the node as NOT generator as it has at least one inlet connection.
            m_has_inlet_connections = true;
        }
        
        void Node::addOutput(const size_t from_index, Node& to, const size_t to_index)
        {
            assert(from_index < m_outlets.size() && "Index out of range.");
            
            m_outlets[from_index].connect(to, to_index);
            
            // mark the node as NOT terminal as it has at least one outlet connection.
            m_has_outlet_connections = true;
        }
        
        bool Node::prepare(size_t const samplerate, size_t const vectorsize)
        {
            if(m_state.load() == Node::State::NotPrepared)
            {
                m_state.store(Node::State::Preparing);
                
                Infos processor_infos
                {
                    samplerate, vectorsize,
                    getConnectedInletsState(),
                    getConnectedOutletsState()
                };
                
                // Prepares the processor
                try { m_want_perform = m_processor.prepare(processor_infos); }
                catch(std::exception&) { throw Error("Audio Processor preparation failed"); }
                
                // Will call recursively prepare on upstream connected Node objects.
                // Todo : maybe make it more explicit.
                prepareBuffers(processor_infos);
                
                m_state.store(Node::State::Prepared);
            }
            
            return m_want_perform;
        }
        
        void Node::clean()
        {
            if(m_state.load() == Node::State::Prepared
               || m_state.load() == Node::State::Performed)
            {
                for(Inlet& inlet : m_inlets)
                {
                    inlet.clean();
                }
                
                for(Outlet& outlet : m_outlets)
                {
                    outlet.clean();
                }
                
                m_state.store(Node::State::ReadyToPerform);
            }
        }
        
        void Node::beforePerform() noexcept
        {
            // Maybe start cpu count for this node here
        }
        
        void Node::perform() noexcept
        {
            if(m_state.load() == Node::State::ReadyToPerform)
            {
                m_state.store(Node::State::Performing);
                for(size_t i = 0; i < m_inlets.size(); ++i)
                {
                    m_inputs.getChannelPtr(i) = m_inlets[i].perform();
                }
             
                beforePerform();
                m_processor.perform(m_inputs, m_outputs);
                afterPerform();
                
                m_state.store(Node::State::Performed);
            }
        }
        
        void Node::afterPerform() noexcept
        {
            // Maybe stop cpu count for this node here
        }
        
        std::vector<bool> Node::getConnectedInletsState()
        {
            const size_t inlets = m_inlets.size();
            std::vector<bool> inlets_states(m_inlets.size());
            
            for(size_t i = 0ul; i < inlets; ++i)
            {
                inlets_states[i] = m_inlets[i].isConnected();
            }
            
            return inlets_states;
        }
        
        std::vector<bool> Node::getConnectedOutletsState()
        {
            const size_t outlets = m_outlets.size();
            std::vector<bool> outlets_states(m_outlets.size());
            
            for(size_t i = 0ul; i < outlets; ++i)
            {
                outlets_states[i] = m_outlets[i].isConnected();
            }
            
            return outlets_states;
        }
        
        void Node::prepareBuffers(Infos const& infos)
        {
            for(Inlet& inlet : m_inlets)
            {
                inlet.prepare(infos);
            }
            
            for(Outlet& outlet : m_outlets)
            {
                outlet.prepare(infos);
                
                // reset output buffer pointers.
                m_outputs.getChannelPtr(outlet.getIndex()) = outlet.getSignal();
            }
        }
        
        bool Node::isConnectedTo(Node const& node) const noexcept
        {
            for(Outlet const& outlet : m_outlets)
            {
                if(outlet.isConnectedTo(node))
                {
                    return true;
                }
            }

            return false;
        }
        
        // ================================================================================ //
        //                                    NODE::PIN                                     //
        // ================================================================================ //
        
        Node::Pin::Pin(Node& owner, bool is_inlet, const size_t index) :
        m_is_inlet(is_inlet),
        m_connected(false),
        m_index(index),
        m_owner(owner),
        m_signal()
        {
            ;
        }
        
        Node::Pin::~Pin()
        {
            m_signal.reset();
        }
        
        Node& Node::Pin::getOwner() noexcept
        {
            return m_owner;
        }
        
        Node const& Node::Pin::getOwner() const noexcept
        {
            return const_cast<Pin*>(this)->m_owner;
        }
        
        size_t Node::Pin::getIndex() const noexcept
        {
            return m_index;
        }
        
        bool Node::Pin::connect(Node& node, const size_t index)
        {
            assert(m_is_inlet ? (index < node.getNumberOfOutlets())
                   : (index < node.getNumberOfInlets())
                   && "Pin index out-of-range");
            
            Pin* pin_ptr = m_is_inlet
            ? static_cast<Pin*>(&node.getOutlet(index))
            : static_cast<Pin*>(&node.getInlet(index));
            
            bool success = false;
            const auto find_it = m_connected_pins.find(pin_ptr);
            
            if(find_it == m_connected_pins.cend())
            {
                success = m_connected_pins.insert(pin_ptr).second;
            }
            
            m_connected = m_connected ? true : success;
            return success;
        }
        
        bool Node::Pin::isConnected() const noexcept
        {
            return m_connected;
        }
        
        bool Node::Pin::isConnectedTo(Node const& node) const noexcept
        {
            if(isConnected())
            {
                for(Pin const* pin : m_connected_pins)
                {
                    Node const& owner_node = pin->m_owner;
                    if(&node == &owner_node)
                    {
                        return true;
                    }
                    else if(!m_is_inlet)
                    {
                        return owner_node.isConnectedTo(node);
                    }
                    else
                    {
                        return node.isConnectedTo(owner_node);
                    }
                }
            }
            
            return false;
        }
        
        Signal::sPtr const& Node::Pin::getSignal() const noexcept
        {
            return m_signal;
        }
        
        void Node::Pin::clean()
        {
            if(m_signal_owner)
            {
                m_signal->clear();
            }
        }
        
        //! @internal Returns the number of connections this Pin have.
        const size_t Node::Pin::getNumberOfConnections() const noexcept
        {
            return m_connected_pins.size();
        }
        
        // ================================================================================ //
        //                                   NODE::INLET                                    //
        // ================================================================================ //
        
        Node::Inlet::Inlet(Node& owner, const size_t index) :
        Node::Pin(owner, true, index)
        {
            ;
        }
        
        void Node::Inlet::prepare(Infos const& infos)
        {
            const size_t vectorsize = infos.getVectorSize();
            
            //const bool inplace = infos.isInplace();
            
            m_signal_owner = false;
            
            const size_t in_connections = getNumberOfConnections();
            
            if(in_connections == 0)
            {
                m_signal = std::make_shared<Signal>(vectorsize);
                m_signal_owner = true;
                return;
            }
            else
            {
                const size_t samplerate = infos.getSampleRate();
                
                if(in_connections > 1)
                {
                    // if we have more than one source input,
                    // we need to sum each of those in another signal.
                    // @todo we can optimize it by addding signal to an already allocated upstream signal (if possible)
                    
                    m_signal = std::make_shared<Signal>(vectorsize);
                    m_signal_owner = true;
                }
                
                // call prepare recursively on connected upstream nodes
                for(Pin* outlet : m_connected_pins)
                {
                    Node& upstream_node = outlet->getOwner();
                    
                    upstream_node.prepare(samplerate, vectorsize);
                    
                    const size_t out_connections = outlet->getNumberOfConnections();
                    
                    if(in_connections == 1)
                    {
                        // One-to-one connection, we can use the same signal that the outlet use.
                        if(out_connections == 1)
                        {
                            m_signal = outlet->getSignal();
                            
                            // perform = no-op
                        }
                        else
                        {
                            // we need to allocate a new signal that will be filled on perform
                            // we will need to copy signal on perform
                            // @todo : we could optimize that by only forwarding signal for the last inlet that use outlet datas (no alloc and no copy)
                            
                            m_signal = std::make_shared<Signal>(vectorsize);
                            m_signal_owner = true;
                            
                            Signal::sPtr source_sig = outlet->getSignal();
                            Signal::sPtr result = m_signal;
                            
                            m_perform_methods.emplace_back([source_sig, result]()
                            {
                                Samples<sample_t>::copy(source_sig->size(), source_sig->data(), result->data());
                            });
                        }
                    }
                    else
                    {
                        // we will need to add signal on perform
                        
                        Signal::sPtr source_sig = outlet->getSignal();
                        Signal::sPtr result = m_signal;
                        
                        m_perform_methods.emplace_back([source_sig, result]()
                        {
                            Samples<sample_t>::add(source_sig->size(), source_sig->data(), result->data());
                        });
                    }
                }
            }
        }
        
        Signal::sPtr Node::Inlet::perform()
        {
            // Call perform recursively on connected upstream nodes
            
            for(Pin* outlet : m_connected_pins)
            {
                Node& upstream_node = outlet->getOwner();
                upstream_node.perform();
            }
            
            // perform based on prepare state
            // case 1 : pass-through
            // case 2 : copy
            // case 3 : add
            
            for(perform_method_t& method : m_perform_methods)
            {
                method();
            }
            
            // set the inlet state to "performed" before return
            return m_signal;
        }
        
        // ================================================================================ //
        //                                   NODE::OUTLET                                   //
        // ================================================================================ //
        
        Node::Outlet::Outlet(Node& owner, const size_t index) :
        Node::Pin(owner, false, index)
        {
            ;
        }
        
        void Node::Outlet::prepare(Infos const& infos)
        {
            if(infos.isInplace() && m_owner.getNumberOfInlets() > m_index)
            {
                m_signal = m_owner.getInlet(m_index).getSignal();
                m_signal_owner = false;
            }
            else
            {
                m_signal = std::make_shared<Signal>(infos.getVectorSize());
                m_signal_owner = true;
            }
        }
    }
}
