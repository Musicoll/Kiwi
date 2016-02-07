//
//  main.cpp
//  KiwiDspTest
//
//  Created by Guillot Pierre on 02/02/2016.
//  Copyright © 2016 CICM. All rights reserved.
//

#define KIWI_DSP_FLOAT
#include "../../../Modules/KiwiDsp/KiwiDsp.hpp"

using namespace kiwi::dsp;

class Plus : public Processor
{
public:
    Plus() noexcept
    {
        std::cout << "Plus\n";
        setNumberOfInlets(2);
        setNumberOfOutlets(1);
    }
    
    Plus(sample value) noexcept : m_value(value)
    {
        std::cout << "Plus " << std::to_string(value) << "\n";
        setNumberOfInlets(2);
        setNumberOfOutlets(1);
    }
    
    ~Plus()  noexcept
    {
        
    }
    
private:
    
    bool prepare(Node const& node) final
    {
        if(node.isInputConnected(0ul))
        {
            return true;
        }
        return false;
    }
    
    void perform(Node const& node) noexcept final
    {
        if(getNumberOfInputs() > 1)
        {
            Samples<sample>::add(node.getVectorSize(), node.getInputSamples(0ul),
                                 node.getInputSamples(1ul), node.getOutputsSamples());
        }
        else
        {
            Samples<sample>::add(node.getVectorSize(), m_value,
                                 node.getInputsSamples(), node.getOutputsSamples());
        }
    }
    
    sample m_value;
};

int main(int , const char *[]) {
    
    std::unique_ptr<Processor> pr1(new Plus());
    std::unique_ptr<Processor> pr2(new Plus(1.f));
    std::unique_ptr<Link> li1(new Link(*pr1.get(), 0, *pr2.get(), 0));
    
    {
        Chain chain;
        std::vector<Processor*> processes;
        std::vector<Link*> links;
        processes.push_back(pr1.get());
        processes.push_back(pr2.get());
        links.push_back(li1.get());
        
        try
        {
            chain.compile(44100, 64, processes, links);
        }
        catch(std::exception& e)
        {
            std::cout << e.what() << "\n";
            return 1;
        }
        
        for(size_t i = 100; i; --i)
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
