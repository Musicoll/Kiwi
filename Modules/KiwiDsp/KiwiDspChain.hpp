/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef KIWI_DSP_CHAIN_H_INCLUDED
#define KIWI_DSP_CHAIN_H_INCLUDED

#include "KiwiDspLink.hpp"
#include "KiwiDspNode.cpp"

namespace kiwi
{
namespace dsp
{
    // ==================================================================================== //
    //                                          CHAIN                                       //
    // ==================================================================================== //
    //! @brief The class manages a set of Node objects connected by a set Link objects.
    //! @details The class acts like a container for Node and Link objects. After adding the
    //! @details Node and Link objects, the class compiles the digital signal processing chain
    //! @details by sorting and connecting the Node objects depending on the Link objects.
    class Chain
    {
    public:
        //! @brief The constructor.
        //! @details Allocates and initializes an empty Chain.
        Chain();
        
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
                        it.stop();
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
            for(std::vector<Node>::size_type i = 0; i < m_nodes.size(); i++)
            {
                m_nodes[i].perform();
            }
        }
        
        std::vector<Node>   m_nodes;
        std::vector<Link>   m_links;
        std::mutex          m_mutex;
        std::atomic<bool>   m_running;
        size_t              m_sample_rate;
        size_t              m_vector_size;
    };
}
}

#endif // KIWI_DSP_CHAIN_H_INCLUDED
