//
//  main.cpp
//  KiwiDspTest
//
//  Created by Guillot Pierre on 02/02/2016.
//  Copyright © 2016 CICM. All rights reserved.
//

#define KIWI_DSP_DOUBLE
#include <KiwiDsp/KiwiDspChain.hpp>

using namespace kiwi::dsp;

static inline void signal_print(const size_t size, sample* in)
{
    for(size_t i = size>>3; i; --i, in += 8)
    {
        std::cout << in[0] << " " << in[1] << " " << in[2] << " " << in[3] << " "
        << in[4] << " " << in[5] << " " << in[6] << " " << in[7] << " ";
    }
    for(size_t i = size&7; i; --i, in++)
    {
        std::cout << in[0] << " ";
    }
}

class Sig : public Processor
{
public:
    Sig(sample value) noexcept : m_value(value)
    {
        setNumberOfOutlets(1);
    }
    
    ~Sig()  noexcept
    {
        
    }
    
private:
    
    bool prepare(Node const& node) final
    {
        return node.isOutputConnected(0ul);
    }
    
    void perform(Node const& node) noexcept final
    {
        Samples<sample>::fill(node.getVectorSize(), m_value, node.getOutputsSamples());
    }
    
    sample m_value;
};

class Plus : public Processor
{
public:
    Plus() noexcept
    {
        setNumberOfInlets(2);
        setNumberOfOutlets(1);
    }
    
    Plus(sample value) noexcept : m_value(value)
    {
        setNumberOfInlets(1);
        setNumberOfOutlets(1);
    }
    
    ~Plus()  noexcept
    {
        
    }
    
private:
    
    bool prepare(Node const& node) final
    {
        return node.isInputConnected(0ul) || (getNumberOfInputs() > 1 && node.isInputConnected(1ul));
    }
    
    void perform(Node const& node) noexcept final
    {
        if(getNumberOfInputs() > 1)
        {
            Samples<sample>::add(node.getVectorSize(), node.getInputSamples(0ul),
                                 node.getInputSamples(1ul), node.getOutputsSamples());
            signal_print(node.getVectorSize(), node.getOutputsSamples());
        }
        else
        {
            Samples<sample>::add(node.getVectorSize(), m_value,
                                 node.getInputsSamples(), node.getOutputsSamples());
            signal_print(node.getVectorSize(), node.getOutputsSamples());
        }
    }
    
    sample m_value;
};

int main(int , const char *[]) {
    
    std::unique_ptr<Processor> pr1(new Plus());
    std::unique_ptr<Processor> pr2(new Plus(1.f));
    std::unique_ptr<Processor> pr3(new Sig(1.3f));
    std::unique_ptr<Link> li1(new Link(*pr1.get(), 0, *pr2.get(), 0));
    std::unique_ptr<Link> li2(new Link(*pr3.get(), 0, *pr1.get(), 0));
    
    {
        Chain chain;
        std::vector<Processor*> processes;
        std::vector<Link*> links;
        processes.push_back(pr1.get());
        processes.push_back(pr2.get());
        processes.push_back(pr3.get());
        links.push_back(li1.get());
        links.push_back(li2.get());
        
        try
        {
            chain.compile(44100, 64, processes, links);
        }
        catch(std::exception& e)
        {
            std::cout << e.what() << "\n";
            return 1;
        }
        
        for(size_t i = 1; i; --i)
        {
            chain.tick();
        }
        
        try
        {
            chain.stop();
        }
        catch(std::exception& e)
        {
            std::cout << e.what() << "\n";
            return 1;
        }
    }
    
    
    return 0;
}
