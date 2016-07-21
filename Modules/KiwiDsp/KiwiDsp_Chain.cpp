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

namespace kiwi
{
    namespace dsp
    {
        // ==================================================================================== //
        //                                          TIE                                         //
        // ==================================================================================== //
        Chain::Tie::Tie(Link& link, std::shared_ptr< Node > const& from, std::shared_ptr< Node > const& to) :
        m_link(link), m_from(from), m_to(to)
        {
            ;
        }
        
        Link const& Chain::Tie::getLink() const noexcept
        {
            return m_link;
        }
        
        Link& Chain::Tie::getLink() noexcept
        {
            return m_link;
        }
        
        void Chain::Tie::connect() const
        {
            std::shared_ptr< Node> from = m_from.lock(), to = m_to.lock();
            assert(static_cast< bool >(from) && static_cast< bool >(to) && "A Link isn't valid.");
            from->addOutput(shared_from_this());
            to->addInput(shared_from_this());
        }
        
        std::shared_ptr< Chain::Node > Chain::Tie::getOutputNode() const noexcept
        {
            return m_from.lock();
        }
        
        std::shared_ptr< Chain::Node >  Chain::Tie::getInputNode() const noexcept
        {
            return m_to.lock();
        }
        
        size_t Chain::Tie::getOutputIndex() const noexcept
        {
            return m_link.getOutputIndex();
        }
        
        size_t Chain::Tie::getInputIndex() const noexcept
        {
            return m_link.getInputIndex();
        }
        
        // ==================================================================================== //
        //                                          NODE                                        //
        // ==================================================================================== //
        
        Chain::Node::Node(Processor& processor) : m_processor(processor), m_index(0ul),
        m_inputs(processor.getNumberOfInputs()), m_outputs(processor.getNumberOfOutputs())
        {
            ;
        }
        
        Chain::Node::~Node()
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
        
        Processor const& Chain::Node::getProcessor() const noexcept
        {
            return m_processor;
        }
        
        std::vector< Chain::tie_set > const& Chain::Node::getInputs() const noexcept
        {
            return m_inputs;
        }
        
        size_t Chain::Node::getIndex() const noexcept
        {
            return m_index;
        }
        
        bool Chain::Node::isSorted() const noexcept
        {
            return static_cast< bool >(m_index);
        }
        
        void Chain::Node::setIndex(size_t const index) noexcept
        {
            m_index = index;
        }
        
        void Chain::Node::addInput(std::shared_ptr< const Chain::Tie > tie)
        {
            assert(tie->getInputIndex() < static_cast< size_t >(m_inputs.size()) && "Index out of range.");
            m_inputs[static_cast< tie_set::size_type >(tie->getInputIndex())].insert(tie);
        }
        
        void Chain::Node::addOutput(std::shared_ptr< const Chain::Tie > tie)
        {
            assert(tie->getOutputIndex() < static_cast< size_t >(m_outputs.size()) && "Index out of range.");
            m_outputs[static_cast< tie_set::size_type >(tie->getOutputIndex())].insert(tie);
        }
        
        bool Chain::Node::prepare(size_t const samplerate, size_t const vectorsize)
        {
            // Remove unecessary inputs and outputs nodes
            // Fills two vectors of states
            std::vector< bool > inputs_states(m_inputs.size());
            {
                auto in = inputs_states.begin();
                for(auto& set : m_inputs)
                {
                    auto it = set.cbegin();
                    while(it != set.cend())
                    {
                        if(!(it->lock()->getInputNode()))
                        {
                            it = set.erase(it);
                        }
                        else
                        {
                            ++it;
                        }
                    }
                    *in = !set.empty();
                }
            }
            std::vector< bool > outputs_states(m_outputs.size());
            {
                auto out = outputs_states.begin();
                for(auto& set : m_outputs)
                {
                    auto it = set.cbegin();
                    while(it != set.cend())
                    {
                        if(!(it->lock()->getInputNode()))
                        {
                            it = set.erase(it);
                        }
                        else
                        {
                            ++it;
                        }
                    }
                    *out = !set.empty();
                }
            }
            // Prepares the processor
            // Fills two vectors of states
            bool state = false;
            Infos infos(samplerate, vectorsize, std::move(inputs_states), std::move(outputs_states));
            try
            {
                state = m_processor.prepare(infos);
            }
            catch(std::exception&)
            {
                throw;
            }
            
            // Allocates the buffers of samples
            // Fills two vectors of states
            if(infos.isAligned() && infos.isInplace())
            {
                
            }
            else if(infos.isAligned() && !infos.isInplace())
            {
                
            }
            else if(!infos.isAligned() && infos.isInplace())
            {
                
            }
            else
            {
                
            }
            
            return state;
        }
        
        void Chain::Node::perform() noexcept
        {
            Buffer input, output;
            m_processor.perform(input, output);
        }
        
        // ==================================================================================== //
        //                                          CHAIN                                       //
        // ==================================================================================== //
        Chain::Chain() :
        m_state(State::NotCompiled), m_sample_rate(0ul), m_vector_size(0ul)
        {
            
        }
        
        Chain::~Chain()
        {
            assert(m_state != State::NotCompiled && "The chain must be not compiled to be destroyed !");
        }
        
        size_t Chain::getSampleRate() const noexcept
        {
            return m_sample_rate;
        }
        
        size_t Chain::getVectorSize() const noexcept
        {
            return m_vector_size;
        }
        
        Chain::State Chain::getState() const noexcept
        {
            return m_state;
        }
        
        void Chain::release()
        {
            std::lock_guard< std::mutex > guard(m_mutex);
            if(m_state != State::NotCompiled)
            {
                try
                {
                    m_nodes.clear();
                }
                catch(std::exception&)
                {
                    throw;
                }
                try
                {
                    m_ties.clear();
                }
                catch(std::exception&)
                {
                    throw;
                }
                m_sample_rate = 0ul;
                m_vector_size = 0ul;
                m_state.store(State::NotCompiled);
            }
        }
        
        void Chain::tick() const noexcept
        {
            std::lock_guard< std::mutex > guard(m_mutex);
            for(std::vector< Node >::size_type i = 0; i < m_nodes.size(); i++)
            {
                m_nodes[i]->perform();
            }
        }
        
        void Chain::compile(size_t const samplerate, size_t const vectorsize,
                            std::set< Processor *> const& processors,
                            std::set< Link *> const& links)
        {
            assert(vectorsize && ((vectorsize & (vectorsize - 1)) == 0) && "The vector size must be a power of two.");
            
            // ============================================================================ //
            //                              STOPS THE DSP                                   //
            // ============================================================================ //
            try
            {
                release();
            }
            catch(std::exception&)
            {
                throw;
            }
            std::lock_guard< std::mutex > guard(m_mutex);
            m_state.store(State::Compiling);
            m_sample_rate = samplerate;
            m_vector_size = vectorsize;
            
            // ============================================================================ //
            //                              CREATES THE NODES                               //
            // ============================================================================ //
            m_nodes.reserve(processors.size());
            for(auto processor : processors)
            {
                if(processor->m_used)
                {
                    throw Error("The Processor object is already in a Chain object.");
                }
                processor->m_used = true;
                assert(processor != nullptr && "A Processor pointer is nullptr.");
                try
                {
                    m_nodes.push_back(std::make_shared< Node >(*processor));
                }
                catch(std::exception&)
                {
                    throw;
                }
            }
            
            // ============================================================================ //
            //                              CONNECTS THE NODES                              //
            // ============================================================================ //
            m_ties.reserve(links.size());
            for(auto it = links.cbegin(); it != links.cend(); ++it)
            {
                assert((*it) != nullptr && "A Link pointer is nullptr.");
                auto cmp = it;
                while(++cmp != links.cend())
                {
                    // Comparer ici
                    ;
                }
            }
            for(auto const link : links)
            {
                std::shared_ptr< Node> from, to;
                for(auto const& node : m_nodes)
                {
                    if(&node->getProcessor() == &link->getInputProcessor())
                    {
                        to = node;
                    }
                    else if(&node->getProcessor() == &link->getOutputProcessor())
                    {
                        from = node;
                    }
                    if(from && to)
                    {
                        break;
                    }
                }
                assert(static_cast< bool >(from) && static_cast< bool >(to) && "A Link isn't valid.");
                try
                {
                    m_ties.push_back(std::make_shared< Tie >(*link, from, to));
                }
                catch(std::exception& e)
                {
                    throw;
                }
                
                try
                {
                    m_ties.back()->connect();
                }
                catch(std::exception& e)
                {
                    throw;
                }
            }
            
            // ============================================================================ //
            //                              NUMBERS THE NODES                               //
            // ============================================================================ //
            class Sorter
            {
            public:
                Sorter() : m_index(1ul) {}
                
                void operator()(std::shared_ptr< Node >& node)
                {
                    if(!static_cast<bool>(node->isSorted()))
                    {
                        m_nodes.insert(node);
                        for(auto const& tieset : node->getInputs())
                        {
                            for(auto const& tie : tieset)
                            {
                                std::shared_ptr< Node > pnode = (tie.lock())->getOutputNode();
                                if(!pnode->isSorted())
                                {
                                    if(m_nodes.find(pnode) != m_nodes.end())
                                    {
                                        throw Error("A loop is detected.");
                                    }
                                    else
                                    {
                                        this->operator()(pnode);
                                    }
                                }
                            }
                        }
                        m_nodes.erase(node);
                        node->setIndex(m_index++);
                    }
                }
            private:
                size_t      m_index;
                node_set    m_nodes;
            };
            
            try
            {
                for_each(m_nodes.begin(), m_nodes.end(), Sorter());
            }
            catch(Error& e)
            {
                throw;
            }
            
            // ============================================================================ //
            //                              SORTS THE NODES                                 //
            // ============================================================================ //
            sort(m_nodes.begin(), m_nodes.end(), [] (std::shared_ptr<Node> const& n1, std::shared_ptr<Node> const& n2)
                 {
                     return n1->getIndex() < n2->getIndex();
                 });
            
            
            // ============================================================================ //
            //                              PREPARES THE NODES                              //
            // ============================================================================ //
            auto it = m_nodes.cbegin();
            while(it != m_nodes.cend())
            {
                bool ready = false;
                try
                {
                    ready = (*it)->prepare(m_sample_rate, m_vector_size);
                }
                catch(std::exception& e)
                {
                    throw;
                }
                if(!ready)
                {
                    it = m_nodes.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            
            m_state = State::Ready;
        }
    }
}
