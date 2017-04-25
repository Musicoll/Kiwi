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

#include "KiwiDsp_Chain.hpp"
#include "KiwiDsp_Misc.hpp"

namespace kiwi
{
    namespace dsp
    {
        // ==================================================================================== //
        //                                          NODE                                        //
        // ==================================================================================== //
        
        Chain::Node::Node(std::shared_ptr<Processor> processor) :
        m_processor(std::move(processor)),
        m_inlets(),
        m_outlets(),
        m_inputs(),
        m_outputs(),
        m_buffer_copy(),
        m_index(0)
        {
            const size_t inlets = m_processor->getNumberOfInputs();
            const size_t outlets = m_processor->getNumberOfOutputs();
            
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
            
            m_buffer_copy.resize(m_processor->getNumberOfInputs());
        }
        
        Chain::Node::~Node()
        {
            try
            {
                m_processor->release();
            }
            catch(Error & e)
            {
                
            }
        }
        
        bool Chain::Node::connectInput(const size_t input_index,
                                       Node& other_node,
                                       const size_t output_index)
        {
            assert(input_index < m_inlets.size() && output_index < other_node.m_outlets.size());
            
            return m_inlets[input_index].connect(other_node.m_outlets[output_index]);
        }
        
        bool Chain::Node::disconnectInput(const size_t input_index,
                                          Node& other_node,
                                          const size_t output_index)
        {
            assert(input_index < m_inlets.size() && output_index < other_node.m_outlets.size());
            
            return m_inlets[input_index].disconnect(other_node.m_outlets[output_index]);
        }
        
        bool Chain::Node::prepare(Chain& chain)
        {
            // ======================================================================== //
            //                    INITIALIZE SAMPLE RATE, VECTOR SIZE                   //
            // ======================================================================== //
            
            size_t samplerate = chain.getSampleRate();
            size_t vectorsize = chain.getVectorSize();
            
            // ======================================================================== //
            //                    PREPARES INLETS, INPUT BUFFER                         //
            // ======================================================================== //
            
            std::vector<Signal::sPtr> inputs;
            inputs.reserve(m_inlets.size());
            
            for(Pin& inlet : m_inlets)
            {
                if (inlet.m_ties.size() == 0)
                {
                    inlet.m_signal = chain.getSignalIn();
                }
                else if(inlet.m_ties.size() == 1)
                {
                    inlet.m_signal = inlet.m_ties.begin()->m_pin.m_signal;
                }
                else
                {
                    inlet.m_signal = chain.getSignalInlet(inlet.m_index);
                    
                    std::vector<std::shared_ptr<Signal>> tie_signals;
                    const auto& ties = inlet.m_ties;
                    tie_signals.reserve(ties.size());
                    
                    for(Tie const& tie : ties)
                    {
                        tie_signals.emplace_back(tie.m_pin.m_signal);
                    }
                    
                    m_buffer_copy[inlet.m_index].setChannels(std::move(tie_signals));
                }
                
                inputs.emplace_back(inlet.m_signal);
            }
            
            m_inputs.setChannels(std::move(inputs));
            
            // ======================================================================== //
            //                    PREPARE OUTLETS, OUTPUT BUFFER                        //
            // ======================================================================== //
            
            std::vector<Signal::sPtr> outputs;
            outputs.reserve(m_outlets.size());
            
            for(Pin& outlet : m_outlets)
            {
                if(outlet.m_ties.size() == 0)
                {
                    outlet.m_signal = chain.getSignalOutlet(outlet.m_index);
                }
                else
                {
                    outlet.m_signal = std::make_shared<Signal>(vectorsize);
                }
                
                outputs.emplace_back(outlet.m_signal);
            }
            
            m_outputs.setChannels(std::move(outputs));
            
            // ======================================================================== //
            //                  INITIALIZE INFO FOR PREPARING PROCESSOR                 //
            // ======================================================================== //
            
            std::vector<bool> input_status(m_inlets.size());
            
            for (size_t i = 0; i < m_inlets.size(); ++i)
            {
                input_status[i] = !m_inlets[i].m_ties.empty();
            }
            
            Processor::PrepareInfo prepare_info {samplerate, vectorsize, std::move(input_status)};
            
            // ======================================================================== //
            //                           PREPARE PROCESSORS                             //
            // ======================================================================== //
            
            m_processor->prepare(std::move(prepare_info));
            
            return m_processor->shouldPerform();
        }
        
        void Chain::Node::perform() noexcept
        {
            const size_t nchannels = m_buffer_copy.size();
            
            for(size_t i = 0; i < nchannels; ++i)
            {
                Buffer const& buffer_copy = m_buffer_copy[i];
                
                if(!buffer_copy.empty())
                {
                    Signal& inputs = m_inputs[i];
                    
                    inputs.copy(buffer_copy[0]);
                    
                    const size_t ncopies = buffer_copy.getNumberOfChannels();
                    
                    for(size_t j = 1; j < ncopies; ++j)
                    {
                        inputs.add(buffer_copy[j]);
                    }
                }
            }
            
            m_processor->perform(m_inputs, m_outputs);
        }
        
        void Chain::Node::release()
        {
            m_index = 0;
            
            // ======================================================================== //
            //                    RELEASE INLET AND INPUT BUFFER                        //
            // ======================================================================== //
            
            for(Pin& inlet : m_inlets) { inlet.m_signal.reset(); }
            m_inputs.clear();
            
            // ======================================================================== //
            //                    RELEASE INLET AND INPUT BUFFER                        //
            // ======================================================================== //
            
            for(Pin& outlet : m_outlets) { outlet.m_signal.reset(); }
            m_outputs.clear();
            
            // ======================================================================== //
            //                         CLEAR BUFFER COPY                                //
            // ======================================================================== //

            for(Buffer& buffer : m_buffer_copy) { buffer.clear(); }
            
            // ======================================================================== //
            //                             RELEASE PROCESSOR                            //
            // ======================================================================== /
            
            m_processor->release();
        }
        
        // ==================================================================================== //
        //                                          NODE::PIN                                   //
        // ==================================================================================== //
        
        Chain::Node::Pin::Pin(Node& owner, const size_t index) :
        m_owner(owner),
        m_index(index),
        m_signal(),
        m_ties()
        {
            ;
        }
        
        Chain::Node::Pin::Pin(Pin && other):
        m_owner(other.m_owner),
        m_index(other.m_index),
        m_signal(std::move(other.m_signal)),
        m_ties(std::move(other.m_ties))
        {
        }
        
        Chain::Node::Pin::~Pin()
        {
            disconnect();
        }
        
        bool Chain::Node::Pin::connect(Pin& other_pin)
        {
            return m_ties.insert(Tie(other_pin)).second && other_pin.m_ties.insert(Tie(*this)).second;
        }
        
        bool Chain::Node::Pin::disconnect(Pin& other_pin)
        {
            return m_ties.erase(Tie(other_pin)) && other_pin.m_ties.erase(Tie(*this));
        }
        
        void Chain::Node::Pin::disconnect()
        {
            for (std::set<Tie>::iterator tie = m_ties.begin(); tie != m_ties.end();)
            {
                tie->m_pin.m_ties.erase(Tie(*this));
                tie = m_ties.erase(tie);
            }
        }
        
        // ==================================================================================== //
        //                                          NODE::TIE                                   //
        // ==================================================================================== //
        
        Chain::Node::Tie::Tie(Pin& pin):
        m_pin(pin)
        {
        }
        
        bool Chain::Node::Tie::operator<(Tie const& other) const noexcept
        {
            return (&m_pin.m_owner < &other.m_pin.m_owner
                    || (&m_pin.m_owner == &other.m_pin.m_owner && m_pin.m_index < other.m_pin.m_index));
        }
        
        // ==================================================================================== //
        //                                          CHAIN                                       //
        // ==================================================================================== //
        
        Chain::Chain() :
        m_nodes(),
        m_sample_rate(),
        m_vector_size(),
        m_state(State::NotPrepared),
        m_tick_mutex()
        {
            ;
        }
        
        Chain::~Chain()
        {
            ;
        }
        
        // ============================================================================ //
        //                              THE UPDATE AND PREPARE                          //
        // ============================================================================ //
        
        void Chain::update()
        {
            if(!m_commands.empty())
            {
                State prev_state = m_state;
                
                const size_t prev_samplerate = getSampleRate();
                const size_t prev_vectorsize = getVectorSize();
                
                release();
                
                while(!m_commands.empty())
                {
                    m_commands.front().operator()();
                    m_commands.pop_front();
                }
                
                if (prev_state == State::Prepared || prev_state == State::Preparing)
                {
                    prepare(prev_samplerate, prev_vectorsize);
                }
            }
        }
        
        void Chain::prepare(const size_t samplerate, const size_t vector_size)
        {
            release();
            
            update();
            
            m_state = State::Preparing;
            
            m_sample_rate = samplerate;
            m_vector_size = vector_size;
            
            indexNodes();
            sortNodes();
            
            for(auto node_it = m_nodes.begin(); node_it != m_nodes.end();)
            {
                if((*node_it)->prepare(*this))
                {
                    ++node_it;
                }
                else
                {
                    restackNode(**node_it);
                    node_it = m_nodes.erase(node_it);
                }
            }
            
            m_state = State::Prepared;
        }
        
        void Chain::restackNode(Node & node)
        {
            // ======================================================================== //
            //                    RESTACK INLET CONNECTIONS                             //
            // ======================================================================== //
            
            for(Node::Pin& inlet : node.m_inlets)
            {
                for(Node::Tie tie : inlet.m_ties)
                {
                    Node::Pin& prev_pin = tie.m_pin;
                    Node& prev_node  = tie.m_pin.m_owner;
                    
                    connect(*prev_node.m_processor, prev_pin.m_index, *node.m_processor, inlet.m_index);
                    
                    m_commands.emplace_front([this,
                                              source = prev_node.m_processor.get(),
                                              source_idx = prev_pin.m_index,
                                              dest = node.m_processor.get(),
                                              dest_idx = inlet.m_index]() {
                        execConnect(source, source_idx, dest, dest_idx);
                    });
                }
            }
            
            // ======================================================================== //
            //                    RESTACK OUTLET CONNECTIONS                            //
            // ======================================================================== //
            
            for(Node::Pin& outlet : node.m_outlets)
            {
                for(Node::Tie tie : outlet.m_ties)
                {
                    Node::Pin& next_pin = tie.m_pin;
                    Node& next_node  = tie.m_pin.m_owner;
                    
                    connect(*node.m_processor, outlet.m_index, *next_node.m_processor, next_pin.m_index);
                    
                    m_commands.emplace_front([this,
                                              source = node.m_processor.get(),
                                              source_idx = outlet.m_index,
                                              dest = next_node.m_processor.get(),
                                              dest_idx = next_pin.m_index]() {
                        execConnect(source, source_idx, dest, dest_idx);
                    });
                }
            }
            
            // ======================================================================== //
            //                           RESTACK PROCESSOR                              //
            // ======================================================================== //
            
            m_commands.emplace_front([this, proc = node.m_processor]() {
                execAddProcessor(proc);
            });
        }
        
        // ============================================================================ //
        //                              THE RELEASE METHOD                              //
        // ============================================================================ //
        
        void Chain::release()
        {
            std::lock_guard<std::mutex> lock(m_tick_mutex);
            
            m_state = State::NotPrepared;
            m_sample_rate = m_vector_size = 0.;
            
            for(Node::uPtr& node : m_nodes)
            {
                node->release();
            }
        }
        
        size_t Chain::getSampleRate() const noexcept
        {
            return m_sample_rate;
        }
        
        size_t Chain::getVectorSize() const noexcept
        {
            return m_vector_size;
        }
        
        void Chain::tick() noexcept
        {
            std::unique_lock<std::mutex> lock(m_tick_mutex, std::defer_lock);
            
            if(m_state == State::Prepared && lock.try_lock())
            {
                for(Node::uPtr& node : m_nodes)
                {
                    node->perform();
                }
                
                lock.unlock();
            }
        }
        
        // ============================================================================ //
        //                                NODE MANGEMENT                                //
        // ============================================================================ //
        
        std::vector<Chain::Node::uPtr>::iterator Chain::findNode(Processor& proc)
        {
            return std::find_if(m_nodes.begin(), m_nodes.end(), [&proc](auto const& proc_uptr){
                return &proc == &(*proc_uptr->m_processor);
            });
        }
        
        std::vector<Chain::Node::uPtr>::const_iterator Chain::findNode(Processor& proc) const
        {
            return std::find_if(m_nodes.begin(), m_nodes.end(), [&proc](auto const& proc_uptr){
                return &proc == &(*proc_uptr->m_processor);
            });
        }
        
        struct Chain::index_node
        {
            index_node() : m_next_index(1ul){};
            
            void computeIndex(Node& node)
            {
                if(node.m_index == 0)
                {
                    m_loop_nodes.insert(&node);
                    
                    for(Node::Pin& inlet : node.m_inlets)
                    {
                        for(Node::Tie tie : inlet.m_ties)
                        {
                            Node& parent_node = tie.m_pin.m_owner;
                            
                            if(!parent_node.m_index)
                            {
                                if(m_loop_nodes.find(&parent_node) != m_loop_nodes.end())
                                {
                                    throw LoopError("A loop is detected");
                                }
                                else
                                {
                                    this->computeIndex(parent_node);
                                }
                            }
                        }
                    }
                    
                    m_loop_nodes.erase(&node);
                    node.m_index = m_next_index++;
                }
            }
            
            void operator()(Node::uPtr const& node)
            {
                computeIndex(*node.get());
            };
            
            size_t          m_next_index;
            std::set<Node*> m_loop_nodes;
        };
        
        void Chain::indexNodes()
        {
            /*
            // sort nodes depending on their graph order 
            std::sort(m_nodes.begin(), m_nodes.end(), [](auto const& lhs_node, auto const& rhs_node){
                auto lhs_order = lhs_node->m_processor->getGraphOrder();
                auto rhs_order = rhs_node->m_processor->getGraphOrder();
                bool smaller = false;
                
                if(lhs_order == Processor::GraphOrder::PutFirst)
                {
                    smaller = (rhs_order != Processor::GraphOrder::PutFirst);
                }
                else if(lhs_order == Processor::GraphOrder::Unordered)
                {
                    smaller = (rhs_order == Processor::GraphOrder::PutLast);
                }
                
                return smaller;
            });
            */
            
            for_each(m_nodes.begin(), m_nodes.end(), index_node());
        }
        
        void Chain::sortNodes()
        {
            std::sort(m_nodes.begin(), m_nodes.end(), [](auto const& lhs_node, auto const& rhs_node){
                return lhs_node->m_index < rhs_node->m_index;
            });
        }
        
        // ============================================================================ //
        //                                NODE MODIFICATIONS                            //
        // ============================================================================ //
        
        void Chain::addProcessor(std::shared_ptr<Processor> processor)
        {
            m_commands.emplace_back([this, proc = std::move(processor)](){
                execAddProcessor(std::move(proc));
            });
        }
        
        void Chain::removeProcessor(Processor& proc)
        {
            m_commands.emplace_back(std::bind(&Chain::execRemoveProcessor, this, &proc));
        }
        
        void Chain::connect(Processor& source, size_t outlet_index,
                            Processor& dest, size_t inlet_index)
        {
            m_commands.emplace_back(std::bind(&Chain::execConnect,
                                              this,
                                              &source, outlet_index,
                                              &dest, inlet_index));
        }
        
        void Chain::disconnect(Processor& source, size_t outlet_index,
                               Processor& dest, size_t inlet_index)
        {
            m_commands.emplace_back(std::bind(&Chain::execDisconnect,
                                              this,
                                              &source, outlet_index,
                                              &dest, inlet_index));
        }
        
        // ==================================================================================== //
        //                                      SIGNAL MANAGEMENT                               //
        // ==================================================================================== //
        
        std::shared_ptr<Signal> Chain::getSignalIn()
        {
            std::shared_ptr<Signal> signal_in = m_signal_in.lock();
            
            if (!signal_in)
            {
                signal_in = std::make_shared<Signal>(m_vector_size);
                m_signal_in = signal_in;
            }
                
            return signal_in;
        }
        
        std::shared_ptr<Signal> Chain::getSignalInlet(size_t inlet_index)
        {
            std::shared_ptr<Signal> signal_inlet = m_signal_inlet[inlet_index].lock();
            
            if (!signal_inlet)
            {
                signal_inlet = std::make_shared<Signal>(m_vector_size);
                m_signal_inlet[inlet_index] = signal_inlet;
            }
            
            return signal_inlet;
        }
        
        std::shared_ptr<Signal> Chain::getSignalOutlet(size_t outlet_index)
        {
            std::shared_ptr<Signal> signal_outlet = m_signal_outlet[outlet_index].lock();
            
            if (!signal_outlet)
            {
                signal_outlet = std::make_shared<Signal>(m_vector_size);
                m_signal_outlet[outlet_index] = signal_outlet;
            }
            
            return signal_outlet;
        }
        
        // ==================================================================================== //
        //                                      CHAIN COMMANDS                                  //
        // ==================================================================================== //
        
        void Chain::execAddProcessor(std::shared_ptr<Processor> proc)
        {
            if (findNode(*proc) == m_nodes.end())
            {
                m_nodes.emplace_back(std::make_unique<Node>(std::move(proc)));
            }
            else
            {
                throw Error("Adding same processor twice");
            }
        }
        
        void Chain::execRemoveProcessor(Processor* proc)
        {
            auto node = findNode(*proc);
            
            if (node != m_nodes.end())
            {
                m_nodes.erase(node);
            }
            else
            {
                throw Error("Removing non existing processor");
            }
        }
        
        void Chain::execConnect(Processor* source, size_t outlet_index,
                                Processor* dest, size_t inlet_index)
        {
            auto source_node = findNode(*source);
            auto dest_node = findNode(*dest);
            
            if (source_node != m_nodes.end() && dest_node != m_nodes.end())
            {
                (*dest_node)->connectInput(inlet_index, **source_node, outlet_index);
            }
            else
            {
                throw Error("Connecting two non existing nodes : ");
            }
        }
        
        void Chain::execDisconnect(Processor* source, size_t outlet_index,
                                   Processor* dest, size_t inlet_index)
        {
            auto source_node = findNode(*source);
            auto dest_node = findNode(*dest);
            
            if (source_node != m_nodes.end() && dest_node != m_nodes.end())
            {
                (*dest_node)->disconnectInput(inlet_index, **source_node, outlet_index);
            }
            else
            {
                throw Error("Disconnecting non existing node : ");
            }
        }
    }
}
