/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#ifndef __DEF_OSP_CHAIN__
#define __DEF_OSP_CHAIN__

#if (__cplusplus <= 199711L)
#define noexcept
#define nullptr NULL
#define constexpr
#endif

#include "OspMisc.hpp"
#include "OspSignal.hpp"
#include "OspProcessor.hpp"
#include "OspLink.hpp"
#include <vector>
#include <set>
#include <cassert>

namespace osp
{
    class Chain;
    
    // ==================================================================================== //
    //                                          NODE                                        //
    // ==================================================================================== //
    //! @brief The class manages a set of Node objects connected by a set Link objects.
    //! @details The class acts like a container for Node and Link objects. After
    //! @details adding the Node and Link objects, the class compiles the digital signal
    //! @details processing chain by sorting and connecting the Node objects depending on the
    //! @details Link objects.
    class Node
    {
    public:
        Node(Chain const* chain, Processor* processor) noexcept :
        m_chain(chain), m_processor(processor), m_index(0ul)
        {
            m_inputs.resize(processor->getNumberOfInputs());
            m_outputs.resize(processor->getNumberOfOutputs());
        }
        
        ~Node()
        {
            if(m_processor->isRunning())
            {
                m_processor->release();
            }
        }
        
        //! @brief Gets the Processor object of the Node object.
        //! @return The Processor object.
        //! @see getChain()
        inline Processor const* getProcessor() const noexcept {return m_processor;}
        
        //! @brief Gets the Chain object of the Node object.
        //! @return The Chain object.
        //! @see getProcessor()
        inline Chain const* getChain() const noexcept {return m_chain;}
        
        //! @brief Checks if an input is connected to any Node.
        //! @details This method can be used to ignore an input's sample vector if the
        //! @details input isn't connected. You should prefer to use it from the preprare
        //! @details method than directly from the perform method.
        //! @return true if the inlet is connected, otherwise it returns false.
        //! @see isOutputConnected() and prepare()
        inline bool isInputConnected(const size_t index) const noexcept {
            assert(index < m_inputs.size() && "Index must be inferior to the number of inputs.");
            return !m_inputs[index].empty();
        }
        
        //! @brief Checks if an outlet is connected to any Node.
        //! @details This method can be used to ignore an output's sample vector if the
        //! @details output isn't connected. You should prefer to use it from the preprare
        //! @details method than directly from the perform method.
        //! @return true if the outlet is connected, otherwise it returns false.
        //! @see isInputConnected() and prepare()
        inline bool isOutputConnected(const size_t index) const noexcept {
            assert(index < m_outputs.size() && "Index must be inferior to the number of outputs.");
            return !m_outputs[index].empty();
        }
        
        //! @brief Retrieves the current Node objects connected to an input.
        std::set<Node*> getInputNodes(const size_t index) const noexcept {
            assert(index < m_inputs.size() && "Index must be inferior to the number of inputs.");
            return m_inputs[index];
        }
        
        //! @brief Retrieves the current Node objects connected to all inputs.
        std::set<Node*> getInputsNodes() const noexcept
        {
            std::set<Node*> nodes;
            for(auto it : m_inputs) {nodes.insert(it.begin(), it.end());}
            return nodes;
        }
        
        //! @brief Retrieves the current Node objects connected to an output.
        std::set<Node*> getOutputNodes(const size_t index) const noexcept {
            assert(index < m_outputs.size() && "Index must be inferior to the number of outputs.");
            return m_inputs[index];
        }
        
        //! @brief Retrieves the current Node objects connected to all outputs.
        std::set<Node*> getOutputsNodes() const noexcept
        {
            std::set<Node*> nodes;
            for(auto it : m_outputs) {nodes.insert(it.begin(), it.end());}
            return nodes;
        }
        
        //! @brief Retrieves the index of the Node object in the Chain object.
        //! @details If the index is zero, the node is out of the Chain or the Chain
        //! @details isn't compiled. All the Node objects connected to the inputs
        //! @details should have a lower index and all the Node objects connected to
        //! @details the outputs should have an upper index.
        inline size_t getIndex() const noexcept {return m_index;}
        
    private:
        
        //! @brief Adds a node to an input.
        //! @param node The node to add.
        //! @param index The index of the input.
        //! @see addOutput()
        void addInput(Node* node, const size_t index)
        {
            if(index < size_t(m_inputs.size()))
            {
                if(!m_inputs[index].insert(node).second)
                {
                    class ErrorDuplicate : public Error
                    {
                    public:
                        const char* what() const noexcept override {
                            return "Osp::Node : The input Node is already connected.";}
                    };
                    throw ErrorDuplicate();
                }
            }
            else
            {
                class ErrorIndex : public Error
                {
                public:
                    const char* what() const noexcept override {
                        return "Osp::Node : The input Node is connected to a wrong index.";}
                };
                throw ErrorIndex();
            }
        }
        
        //! @brief Adds a node to an output.
        //! @param node The node to add.
        //! @param index The index of the output.
        //! @see addInput()
        void addOutput(Node* node, const size_t index)
        {
            if(index < size_t(m_outputs.size()))
            {
                if(!m_outputs[index].insert(node).second)
                {
                    class ErrorDuplicate : public Error
                    {
                    public:
                        const char* what() const noexcept override {
                            return "Osp::Node : The output Node is already connected.";}
                    };
                    throw ErrorDuplicate();
                }
            }
            else
            {
                class ErrorIndex : public Error
                {
                public:
                    const char* what() const noexcept override {
                        return "Osp::Node : The output Node is connected to a wrong index.";}
                };
                throw ErrorIndex();
            }
        }
        
        void prepare()
        {
            
        }
        
        //! Ticks once the digital signal processing.
        /** This method is called by the Chain during its tick method's call. This method calls the inputs
         perform methods and the perform method of the child class' implementation.
         @see start() and stop()
         */
        inline void perform() noexcept
        {
            if(m_processor->isRunning())
            {
                for(std::vector< std::set<Node*> >::size_type i = 0; i < m_inputs.size(); i++)
                {
                    //m_inputs[i].perform();
                }
                m_processor->perform();
            }
        }
        
        Chain const*    m_chain;
        Processor*      m_processor;
        size_t          m_index;
        std::vector< std::set<Node*> > m_inputs;
        std::vector< std::set<Node*> > m_outputs;
    };
    
    class LinkContainer
    {
    public:
        LinkContainer(Link const* link)
        {
            
        }
        
        ~LinkContainer()
        {
            
        }
        
        //! @brief Connects the two Node objects together.
        //! @details The method is only used by the Chain object.
        void connect() const
        {
            if(!m_from || !m_to)
            {
                class ErrorInvalid : public Error
                {
                public:
                    const char* what() const noexcept override {
                        return "Osp::Link : At least one Node object isn't valid.";}
                };
                throw ErrorInvalid();
            }
            if(m_from == m_to)
            {
                class ErrorSimilar : public Error
                {
                public:
                    const char* what() const noexcept override {
                        return "Osp::Link : The two Node objects are similar.";}
                };
                throw ErrorSimilar();
            }
            
            try
            {
                m_from->addOutput(m_to, m_output);
            }
            catch(Error& e)
            {
                throw e;
            }
            
            try
            {
                m_to->addInput(m_from, m_input);
            }
            catch(Error& e)
            {
                throw e;
            }
        }
    };
    
    // ==================================================================================== //
    //                                          CHAIN                                       //
    // ==================================================================================== //
    //! @brief The class manages a set of Node objects connected by a set Link objects.
    //! @details The class acts like a container for Node and Link objects. After
    //! @details adding the Node and Link objects, the class compiles the digital signal
    //! @details processing chain by sorting and connecting the Node objects depending on the
    //! @details Link objects.
    class Chain
    {
    public:
        //! @brief The constructor.
        //! @details Allocates and initializes an empty Chain.
        Chain() noexcept :
        m_running(false), m_sample_rate(0ul), m_vector_size(0ul) {}
        
        //! @brief The destructor.
        //! @details Stops the digital signal processing if needed and frees the
        //! @details containers.
        ~Chain()
        {
            if(m_running)
            {
                stop();
            }
            m_nodes.clear();
            m_links.clear();
        }
        
        //! @brief Retrieves the current sample rate.
        inline size_t getSampleRate() const noexcept {return m_sample_rate;}
        //! @brief Retrieves the current vector size of the chain.
        inline size_t getVectorSize() const noexcept {return m_vector_size;}
        //! @brief Retrieves the current internal state of the DSP.
        inline bool isRunning() const noexcept {return m_running;}
        //! @brief Retrieves the number of Node objects.
        inline size_t getNumberOfNodes() const noexcept {return size_t(m_nodes.size());}
        //! @brief Retrieves the number of Link objects.
        inline size_t getNumberOfLinks() const noexcept {return size_t(m_links.size());}
        //! @brief Retrieves the Node objects.
        inline std::vector<Node*> getNodes() const noexcept {return m_nodes;}
        //! @brief Retrieves the Link objects.
        inline std::vector<Link*> getLinks() const noexcept {return m_links;}
        //! @brief Compiles the dsp chain.
        //! @details The function sorts the dsp nodes and call the prepares methods of the
        //! @details Node objects.
        void compile(std::vector< Processor * > processor, std::vector< Link * > links)
        {
            if(m_running)
            {
                stop();
            }
            std::lock_guard<std::mutex> guard(m_mutex);
            for(auto it : links)
            {
                try
                {
                    it->connect();
                }
                catch(Error& e)
                {
                    throw e;
                }
            }
            
            struct Sorter
            {
                size_t index;
                std::set<NodeContainer> nodes;
                
                inline Sorter() noexcept : index(1ul) {}
                inline ~Sorter() noexcept {nodes.clear();}
                void operator()(NodeContainer& node)
                {
                    if(!node.index)
                    {
                        nodes.insert(node);
                        for(auto it : node->getInputsNodes())
                        {
                            NodeContainer& input = it;
                            if(input && !input->getIndex())
                            {
                                auto result = nodes.find(input);
                                if(result != nodes.end())
                                {
                                    class ErrorLoop : public Error
                                    {
                                    public:
                                        const char* what() const noexcept override {return "Osp::Chain : A loop is present.";}
                                    };
                                    
                                    throw ErrorLoop();
                                }
                                else
                                {
                                    this->operator()(input);
                                }
                            }
                        }
                        nodes.erase(node);
                        node.index = index++;
                    }
                }
            };
            
            try
            {
                for_each(m_nodes.begin(), m_nodes.end(), Sorter());
            }
            catch(Error& e)
            {
                throw e;
            }
            
            sort(m_nodes.begin(), m_nodes.end(), [] (sNode const& n1, sNode const& n2)
                 {
                     return n1->getIndex() < n2->getIndex();
                 });
            
            
            for(auto it : m_nodes)
            {
                if(bool(it->getIndex()))
                {
                    try
                    {
                        it->start();
                    }
                    catch(DspError& e)
                    {
                        throw e;
                    }
                }
            }
            
            m_running = true;
        }
        
        //! @brief Stops the digital signal processing.
        //! @details Stops the digital signal processing of all the Node objects.
        void stop()
        {
            if(m_running)
            {
                m_running = false;
                std::lock_guard<std::mutex> guard(m_mutex);
                for(auto it : m_nodes)
                {
                    try
                    {
                        it->stop();
                    }
                    catch(Error& e)
                    {
                        throw e;
                    }
                }
            }
        }
        
        //! @brief Ticks once all the Node objects.
        inline void tick() noexcept
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            for(std::vector<Node*>::size_type i = 0; i < m_nodes.size(); i++)
            {
                m_nodes[i]->tick();
            }
        }
        
        std::vector<Node*>  m_nodes;
        std::vector<Link*>  m_links;
        std::mutex          m_mutex;
        std::atomic<bool>   m_running;
        size_t              m_sample_rate;
        size_t              m_vector_size;
    };
}

#endif
