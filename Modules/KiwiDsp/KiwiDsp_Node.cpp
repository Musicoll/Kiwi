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
        
        Node::Node(std::shared_ptr<Processor> processor, uint64_t id) :
        m_want_perform(false),
        m_is_deleted(false),
        m_prepare_state(PrepareState::NotPrepared),
        m_perform_state(PerformState::NotReady),
        m_release_state(ReleaseState::Released),
        m_processor(processor),
        m_inputs(),
        m_outputs(),
        m_id(id)
        {
            const size_t inlets = processor->getNumberOfInputs();
            const size_t outlets = processor->getNumberOfOutputs();
            
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
            assert(m_release_state.load() == ReleaseState::Released && "Removing a node before releasing it");
            
            m_inlets.clear();
            
            m_outlets.clear();
        }
        
        std::shared_ptr<Processor> Node::getProcessor() noexcept
        {
            return m_processor;
        }
        
        std::shared_ptr<const Processor> Node::getProcessor() const noexcept
        {
            return m_processor;
        }
        
        uint64_t Node::getId() const noexcept
        {
            return m_id;
        }
        
        size_t Node::getNumberOfInlets() const noexcept
        {
            return m_inlets.size();
        }
        
        size_t Node::getNumberOfOutlets() const noexcept
        {
            return m_outlets.size();
        }
        
        bool Node::isGenerator() const noexcept
        {
            return getNumberOfInputConnections() == 0;
        }
        
        bool Node::isTerminal() const noexcept
        {
            return getNumberOfOutputConnections() == 0;
        }
        
        bool Node::isIsolated() const noexcept
        {
            return isTerminal() && isGenerator();
        }
        
        bool Node::wantPerform() const noexcept
        {
            return m_want_perform;
        }
        
        bool Node::canConnectWithoutLoop(Node const& other_node)
        {
            return !other_node.hasBackwardPathTo(*(this));
        }
        
        bool Node::connectInput(const size_t input_index, Node& other_node, const size_t output_index)
        {
            bool success = false;
            
            if(canConnectWithoutLoop(other_node))
            {
                success = getInlet(input_index).connect(other_node, output_index);
            }
            else
            {
                throw Error("A loop is detected");
            }
            
            return success;
        }
        
        void Node::setAsDisconnected()
        {
            for(Inlet& inlet : m_inlets)
            {
                inlet.setAsDisconnected();
            }
            
            for(Outlet& outlet : m_outlets)
            {
                outlet.setAsDisconnected();
            }
        }
        
        bool Node::disconnectInput(const size_t input_index, Node& other_node, const size_t output_index)
        {
            return getInlet(input_index).disconnect(other_node, output_index);
        }
        
        Node::Inlet const& Node::getInlet(const size_t index) const noexcept
        {
            assert(index < m_inlets.size() && "Index out of range.");
            
            return m_inlets[index];
        }
        
        Node::Inlet& Node::getInlet(const size_t index) noexcept
        {
            assert(index < m_inlets.size() && "Index out of range.");
            
            return m_inlets[index];
        }
        
        Node::Outlet const& Node::getOutlet(const size_t index) const noexcept
        {
            assert(index < m_outlets.size() && "Index out of range.");
            
            return m_outlets[index];
        }
        
        Node::Outlet& Node::getOutlet(const size_t index) noexcept
        {
            assert(index < m_outlets.size() && "Index out of range.");
            
            return m_outlets[index];
        }
        
        PrepareState Node::getPrepareState() const noexcept
        {
            return m_prepare_state.load();
        }
        
        bool Node::isDeleted() const noexcept
        {
            return m_is_deleted;
        }
        
        void Node::setAsDeleted() noexcept
        {
            setAsDisconnected();
            m_is_deleted = true;
        }
        
        void Node::setAsNotPrepared() noexcept
        {
            m_prepare_state.store(PrepareState::NotPrepared);
        }
        
        
        void Node::prepareBuffers(Infos const& infos)
        {
            std::vector<Signal::sPtr> inputs;
            std::vector<Signal::sPtr> outputs;
            
            for(Inlet& inlet : m_inlets)
            {
                inlet.prepare(infos);
                inputs.push_back(inlet.getSignal());
            }
            
            for(Outlet& outlet : m_outlets)
            {
                outlet.prepare(infos);
                outputs.push_back(outlet.getSignal());
            }
            
            m_inputs.reset(new Buffer(inputs));
            m_outputs.reset(new Buffer(outputs));
        }
        
        bool Node::prepare(size_t const samplerate, size_t const vectorsize)
        {
            if(m_prepare_state.load() == PrepareState::NotPrepared
               || m_release_state.load() == ReleaseState::Released)
            {
                m_prepare_state.store(PrepareState::Preparing);
                
                Infos processor_infos
                {
                    samplerate, vectorsize,
                    getConnectedInletsState(),
                    getConnectedOutletsState()
                };
                
                // Prepares the processor
                try
                {
                    m_want_perform = m_processor->prepare(processor_infos);
                }
                catch(std::exception&)
                {
                    m_prepare_state.store(PrepareState::NotPrepared);
                    m_release_state.store(ReleaseState::NotReleased);
                    throw Error("Audio Processor preparation failed");
                }
                
                // Will call recursively prepare on upstream connected Node objects.
                // Todo : maybe make it more explicit.
                prepareBuffers(processor_infos);
                
                m_prepare_state.store(PrepareState::Prepared);
                m_perform_state.store(PerformState::Ready);
                m_release_state.store(ReleaseState::NotReleased);
            }
            
            return m_want_perform;
        }
        
        void Node::clean()
        {
            
            if(m_perform_state.load() == PerformState::Performed)
            {
                for(Inlet& inlet : m_inlets)
                {
                    inlet.clean();
                }
                
                for(Outlet& outlet : m_outlets)
                {
                    outlet.clean();
                }
                
                m_perform_state.store(PerformState::Ready);
            }
        }
        
        void Node::beforePerform() noexcept
        {
            // Maybe start cpu count for this node here
        }
        
        void Node::perform() noexcept
        {
            if(m_perform_state.load() == PerformState::Ready)
            {
                if (m_inputs && m_outputs)
                {
                    m_perform_state.store(PerformState::Performing);
                    
                    for(Inlet& inlet : m_inlets)
                    {
                        inlet.perform();
                    }
                    
                    beforePerform();
                    m_processor->perform(*m_inputs, *m_outputs);
                    afterPerform();
                    
                    m_perform_state.store(PerformState::Performed);
                }
            }
        }
        
        void Node::afterPerform() noexcept
        {
            // Maybe stop cpu count for this node here
        }
        
        void Node::release()
        {
            if (m_release_state.load() != ReleaseState::Released)
            {
                m_release_state.store(ReleaseState::Releasing);
                
                for (Inlet& inlet : m_inlets)
                {
                    inlet.release();
                }
                
                for (Outlet& outlet : m_outlets)
                {
                    outlet.release();
                }
                
                m_inputs.release();
                m_outputs.release();
                
                try { m_processor->release(); }
                catch(std::exception&) { throw Error("Audio Processor release failed"); }
                
                m_release_state.store(ReleaseState::Released);
                m_perform_state.store(PerformState::NotReady);
                m_prepare_state.store(PrepareState::NotPrepared);
            }
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
        
        bool Node::hasBackwardPathTo(Node const& node) const noexcept
        {
            for(Inlet const& inlet : m_inlets)
            {
                if(inlet.hasBackwardPathTo(node))
                {
                    return true;
                }
            }
            
            return false;
        }
        
        size_t Node::getNumberOfInputConnections() const noexcept
        {
            size_t numberOfInputConnection = 0;
            
            for(Inlet const& inlet : m_inlets)
            {
                numberOfInputConnection += inlet.getNumberOfConnections();
            }
            
            return numberOfInputConnection;
        }
        
        size_t Node::getNumberOfOutputConnections() const noexcept
        {
            size_t numberOfOutputConnections = 0;
            
            for(Outlet const& outlet : m_outlets)
            {
                numberOfOutputConnections += outlet.getNumberOfConnections();
            }
            
            return numberOfOutputConnections;
        }
        
        // ================================================================================ //
        //                                    NODE::PIN                                     //
        // ================================================================================ //
        
        Node::Pin::Pin(Node& owner, const size_t index) :
        m_owner(owner),
        m_index(index),
        m_signal(),
        m_signal_owner(false),
        m_prepare_state(PrepareState::NotPrepared),
        m_release_state(ReleaseState::Released),
        m_connections()
        {
            ;
        }
        
        Node::Pin::Pin(Pin const& other):
        m_owner(other.m_owner),
        m_index(other.m_index),
        m_signal(other.m_signal),
        m_signal_owner(other.m_signal_owner),
        m_prepare_state(other.m_prepare_state.load()),
        m_release_state(other.m_release_state.load()),
        m_connections(other.m_connections)
        {
            ;
        }
        
        Node::Pin::~Pin()
        {
            assert(m_release_state.load() == ReleaseState::Released
                   && "Removing a pin before releasing it : ");
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
        
        bool Node::Pin::isConnected() const noexcept
        {
            return m_connections.size() > 0;
        }
        
        Signal::sPtr Node::Pin::getSignal() const noexcept
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
        
        const size_t Node::Pin::getNumberOfConnections() const noexcept
        {
            return m_connections.size();
        }
        
        bool Node::Pin::addConnection(std::shared_ptr<Connection> connection)
        {
            return m_connections.insert(connection).second;
        }
        
        bool Node::Pin::removeConnection(std::shared_ptr<Connection> connection)
        {
            return m_connections.erase(connection);
        }
        
        void Node::Pin::setAsDisconnected()
        {
            for(std::shared_ptr<Connection> connection : m_connections)
            {
                connection->setAsDisconnected();
            }
        }
        
        bool Node::Pin::compare_cnx::operator()(std::shared_ptr<const Connection> left_cnx,
                                                std::shared_ptr<const Connection> right_cnx) const
        {
            return *left_cnx < *right_cnx;
        }
        
        // ================================================================================ //
        //                                   NODE::INLET                                    //
        // ================================================================================ //
        
        Node::Inlet::Inlet(Node& owner, const size_t index) :
        Pin(owner, index),
        m_pull_method()
        {
            ;
        }
        
        Node::Inlet::Inlet(Inlet const& other):
        Pin(other),
        m_pull_method(other.m_pull_method)
        {
            ;
        }
        
        Node::Inlet::~Inlet()
        {
            disconnect();
        }
        
        void Node::Inlet::prepareConnections(Infos const& infos) const
        {
            for(std::shared_ptr<Connection> connection : m_connections)
            {
                connection->prepare(infos.getSampleRate(), infos.getVectorSize());
            }
        }
        
        void Node::Inlet::unlockConnections()
        {
            for(std::shared_ptr<Connection> connection : m_connections)
            {
                connection->unlock();
            }
        }
        
        Signal::sPtr Node::Inlet::acquireReadSignal()
        {
            Signal::sPtr read_signal;
            
            for (auto connection = m_connections.cbegin();
                 connection != m_connections.cend() && !read_signal;
                 ++connection)
            {
                read_signal = (*connection)->acquireReadSignal();
            }
            
            return read_signal;
        }
        
        Signal::sPtr Node::Inlet::acquireWriteSignal()
        {
            Signal::sPtr write_signal;
            
            for (auto connection = m_connections.cbegin();
                 connection != m_connections.cend() && !write_signal;
                 ++connection)
            {
                write_signal = (*connection)->acquireWriteSignal();
            }
            
            return write_signal;
        }
        
        void Node::Inlet::copyAndAddSignals()
        {
            bool signal_copied = false;
            
            for (std::shared_ptr<Connection> connection : m_connections)
            {
                if (!connection->isLocked())
                {
                    if (signal_copied)
                    {
                        m_signal->add(*(connection->getSignal()));
                    }
                    else
                    {
                        m_signal->copy(*(connection->getSignal()));
                        signal_copied = true;
                    }
                }
            }
        }
        
        void Node::Inlet::addSignals()
        {
            for(std::shared_ptr<Connection> connection : m_connections)
            {
                if (m_signal != connection->getSignal() && !connection->isLocked())
                {
                    m_signal->add(*(connection->getSignal()));
                }
            }
        }
        
        void Node::Inlet::setAsNotPrepared()
        {
            m_prepare_state.store(PrepareState::NotPrepared);
            
            getOwner().setAsNotPrepared();
            
            size_t inplace_index = 0;
            
            if (isInplace(&inplace_index))
            {
                getOwner().getOutlet(inplace_index).setAsNotPrepared();
            }
        }
        
        void Node::Inlet::prepare(Infos const& infos)
        {
            if (m_prepare_state.load() == PrepareState::NotPrepared
                || m_release_state.load() == ReleaseState::Released)
            {
                disconnect();
                
                resetConnections();
                
                // Call prepare on upstream nodes
                prepareConnections(infos);
                
                const size_t in_connections = getNumberOfConnections();
                const size_t vector_size = infos.getVectorSize();
                
                if (in_connections == 0)
                {
                    m_signal = std::make_shared<Signal>(vector_size);
                    m_signal_owner = true;
                    
                    m_pull_method = []{};
                }
                else
                {
                    if (!isInplace())
                    {
                        if (in_connections == 1)
                        {
                            Signal::sPtr read_signal = acquireReadSignal();
                            
                            if (read_signal)
                            {
                                m_signal = read_signal;
                                m_signal_owner = false;
                                
                                m_pull_method  = []{};
                            }
                            else
                            {
                                m_signal = std::make_shared<Signal>(vector_size);
                                m_signal_owner = true;
                                
                                m_pull_method  = std::bind(&Node::Inlet::copyAndAddSignals, this);
                            }
                        }
                        else
                        {
                            Signal::sPtr write_signal = acquireWriteSignal();
                            
                            if (write_signal)
                            {
                                m_signal = write_signal;
                                m_signal_owner = false;
                                
                                m_pull_method  = std::bind(&Node::Inlet::addSignals, this);
                            }
                            else
                            {
                                m_signal = std::make_shared<Signal>(vector_size);
                                m_signal_owner = true;
                                
                                m_pull_method  = std::bind(&Node::Inlet::copyAndAddSignals, this);
                            }
                        }
                    }
                    else
                    {
                        Signal::sPtr write_signal = acquireWriteSignal();
                        
                        if (write_signal)
                        {
                            m_signal = write_signal;
                            m_signal_owner = false;
                            
                            m_pull_method  = std::bind(&Node::Inlet::addSignals, this);
                        }
                        else
                        {
                            m_signal = std::make_shared<Signal>(vector_size);
                            m_signal_owner = true;
                            
                            m_pull_method  = std::bind(&Node::Inlet::copyAndAddSignals, this);
                        }
                    }
                }
                
                unlockConnections();
                
                assert(m_signal && m_pull_method && "Prepare method failed");
                
                m_prepare_state.store(PrepareState::Prepared);
                m_release_state.store(ReleaseState::NotReleased);
            }
        }
        
        void Node::Inlet::resetConnections()
        {
            for(std::shared_ptr<Connection> connection : m_connections)
            {
                connection->reset();
            }
        }
        
        void Node::Inlet::release()
        {
            if (m_release_state.load() == ReleaseState::NotReleased)
            {
                m_release_state.store(ReleaseState::Releasing);
                
                resetConnections(); // set as not prepared
                
                m_signal.reset();
                m_signal_owner = false;
                
                m_pull_method = []{};
                
                m_release_state.store(ReleaseState::Released);
                m_prepare_state.store(PrepareState::NotPrepared);
            }
        }
        
        void Node::Inlet::performConnections()
        {
            for(std::shared_ptr<Connection> connection : m_connections)
            {
                connection->perform();
            }
        }
        
        void Node::Inlet::perform()
        {
            // Call perform recursively on connected upstream nodes
            performConnections();
            
            // perform based on prepare state
            // case 1 : pass-through
            // case 2 : copy
            // case 3 : add
            
            m_pull_method();
            
            // set the inlet state to "performed" before return
        }
        
        bool Node::Inlet::connect(Node& node, const size_t index)
        {
            assert(index < node.getNumberOfOutlets()
                   && "Pin index out-of-range");
            
            std::shared_ptr<Connection> connection = std::make_shared<Connection>(*this, node.getOutlet(index));
            
            bool connection_inserted = addConnection(connection) && node.getOutlet(index).addConnection(connection);
            
            if (connection_inserted)
            {
                setAsNotPrepared();
            }
            
            return connection_inserted;
        }
        
        bool Node::Inlet::disconnect(Node& node, const size_t index)
        {
            assert(index < node.getNumberOfOutlets()
                   && "Pin index out-of-range");
            
            std::shared_ptr<Connection> connection = std::make_shared<Connection>(*this, node.getOutlet(index));
            
            auto found_connection = m_connections.find(connection);
            
            bool connection_existed = found_connection != m_connections.end() && !(*found_connection)->isDisconnected();
            
            if (connection_existed)
            {
                (*found_connection)->setAsDisconnected();
            }
            
            return connection_existed;
        }
        
        void Node::Inlet::disconnect()
        {
            for (auto connection = m_connections.begin(); connection != m_connections.end();)
            {
                if ((*connection)->isDisconnected())
                {
                    (*connection)->getOutlet().removeConnection(*connection);
                    connection = m_connections.erase(connection);
                }
                else
                {
                    ++connection;
                }
            }
        }
        
        bool Node::Inlet::hasBackwardPathTo(Node const& node) const noexcept
        {
            if(isConnected())
            {
                for(std::shared_ptr<Connection> connection : m_connections)
                {
                    return connection->hasBackwardPathTo(node);
                }
            }
            
            return false;
        }
        
        bool Node::Inlet::isInplace(size_t * const outlet_index)
        {
            return getOwner().getProcessor()->isInletInplace(getIndex(), outlet_index);
        }
        
        // ================================================================================ //
        //                                   NODE::OUTLET                                   //
        // ================================================================================ //
        
        Node::Outlet::Outlet(Node& owner, const size_t index) :
        Pin(owner, index)
        {
            ;
        }
        
        Node::Outlet::Outlet(Outlet const& other):
        Pin(other)
        {
        }
        
        Node::Outlet::~Outlet()
        {
            disconnect();
        }
        
        void Node::Outlet::setAsNotPrepared()
        {
            m_prepare_state.store(PrepareState::NotPrepared);
            
            for(std::shared_ptr<Connection> connection : m_connections)
            {
                if(!connection->isDisconnected() && (connection->isReadAcquired() || connection->isWriteAcquired()))
                {
                    connection->getInlet().setAsNotPrepared();
                }
            }
        }
        
        void Node::Outlet::prepare(Infos const& infos)
        {
            if (m_prepare_state.load() == PrepareState::NotPrepared
                || m_release_state.load() == ReleaseState::Released)
            {
                size_t inplace_index = 0;
                
                if(isInplace(&inplace_index))
                {
                    m_signal = m_owner.getInlet(inplace_index).getSignal();
                    m_signal_owner = false;
                }
                else
                {
                    m_signal = std::make_shared<Signal>(infos.getVectorSize());
                    m_signal_owner = true;
                }
                
                m_prepare_state.store(PrepareState::Prepared);
                m_release_state.store(ReleaseState::NotReleased);
            }
        }
        
        void Node::Outlet::release()
        {
            if (m_release_state.load() != ReleaseState::Released)
            {
                m_signal.reset();
                m_signal_owner = false;
                
                m_release_state.store(ReleaseState::Released);
                m_prepare_state.store(PrepareState::NotPrepared);
            }
        }
        
        bool Node::Outlet::isInplace(size_t * const inlet_index)
        {
            return getOwner().getProcessor()->isOutletInplace(getIndex(), inlet_index);
        }
        
        void Node::Outlet::disconnect()
        {
            for (auto connection = m_connections.begin(); connection != m_connections.end();)
            {
                if ((*connection)->isDisconnected())
                {
                    (*connection)->getInlet().removeConnection(*connection);
                    connection = m_connections.erase(connection);
                }
                else
                {
                    ++connection;
                }
            }
        }
        
        bool Node::Outlet::isWriteAcquired() const
        {
            bool write_acquired = false;
            
            for (auto connection = m_connections.cbegin();
                 connection != m_connections.cend() && !write_acquired;
                 ++connection)
            {
                if ((*connection)->isWriteAcquired())
                {
                    write_acquired = true;
                }
            }
            
            return write_acquired;
        }
        
        bool Node::Outlet::isReadAcquired() const
        {
            bool read_acquired = false;
            
            for (auto connection = m_connections.cbegin();
                 connection != m_connections.cend() && !read_acquired;
                 ++connection)
            {
                if ((*connection)->isReadAcquired())
                {
                    read_acquired = true;
                }
            }
            
            return read_acquired;
        }
        
        Node::Connection::Connection(Inlet& inlet, Outlet& outlet):
        m_inlet(inlet),
        m_outlet(outlet),
        m_is_write_acquired(false),
        m_is_read_acquired(false),
        m_is_locked(true),
        m_is_disconnected(false)
        {
        }
        
        Node::Inlet& Node::Connection::getInlet() noexcept
        {
            return m_inlet;
        }
        
        Node::Outlet& Node::Connection::getOutlet() noexcept
        {
            return m_outlet;
        }
        
        size_t Node::Connection::getInletIndex() const noexcept
        {
            return m_inlet.getIndex();
        }
        
        size_t Node::Connection::getOutletIndex() const noexcept
        {
            return m_outlet.getIndex();
        }
        
        void Node::Connection::setAsDisconnected() noexcept
        {
            getInlet().setAsNotPrepared();
            m_is_disconnected = true;
        }
        
        bool Node::Connection::isDisconnected() const noexcept
        {
            return m_is_disconnected;
        }
        
        void Node::Connection::reset() noexcept
        {
            m_is_read_acquired = false;
            m_is_read_acquired = false;
        }
        
        void Node::Connection::lock() noexcept
        {
            m_is_locked = true;
        }
        
        void Node::Connection::unlock() noexcept
        {
            m_is_locked = false;
        }
        
        bool Node::Connection::isLocked() const noexcept
        {
            return m_is_locked;
        }
        
        bool Node::Connection::operator<(Connection const& other) const noexcept
        {
            return &m_inlet < &other.m_inlet || (&m_inlet == &other.m_inlet && &m_outlet < &other.m_outlet);
        }
        
        bool Node::Connection::hasBackwardPathTo(Node const& node) const noexcept
        {
            if (m_is_disconnected)
            {
                return false;
            }
            else
            {
                if (&m_outlet.getOwner() == &node)
                {
                    return true;
                }
                else
                {
                    return m_outlet.getOwner().hasBackwardPathTo(node);
                }
            }
        }
        
        bool Node::Connection::prepare(const size_t samplerate, const size_t vectorsize)
        {
            return !m_is_disconnected ? m_outlet.getOwner().prepare(samplerate, vectorsize) : false;
        }
        
        bool Node::Connection::isReadAcquired() const
        {
            return m_is_read_acquired;
        }
        
        bool Node::Connection::isWriteAcquired() const
        {
            return m_is_write_acquired;
        }
        
        Signal::sPtr Node::Connection::acquireWriteSignal()
        {
            Signal::sPtr write_pointer;
            
            if (!m_is_read_acquired && !m_is_write_acquired && !m_is_disconnected)
            {
                if (!m_outlet.isReadAcquired() && !m_outlet.isWriteAcquired())
                {
                    write_pointer = m_outlet.getSignal();
                    m_is_write_acquired = true;
                }
            }
            
            return write_pointer;
        }
        
        Signal::sPtr Node::Connection::acquireReadSignal()
        {
            Signal::sPtr read_pointer;
            
            if (!m_is_read_acquired && !m_is_write_acquired && !m_is_disconnected)
            {
                if (!m_outlet.isWriteAcquired())
                {
                    read_pointer = m_outlet.getSignal();
                    m_is_read_acquired = true;
                }
            }
            
            return read_pointer;
        }
            
        Signal::scPtr Node::Connection::getSignal() const
        {
            return !m_is_locked ? m_outlet.getSignal() : Signal::scPtr(nullptr);
        }
        
        void Node::Connection::perform()
        {
            if (!m_is_locked)
            {
                m_outlet.getOwner().perform();
            }
        }
    }
}
