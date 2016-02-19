//
//  main.cpp
//  KiwiDspTest
//
//  Created by Guillot Pierre on 02/02/2016.
//  Copyright © 2016 CICM. All rights reserved.
//

#include "test_class.hpp"
#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"


/*

int main(int , const char *[]) {
    
    std::unique_ptr<Processor> pr1(new Plus());
    std::unique_ptr<Processor> pr2(new Plus(1.f));
    std::unique_ptr<Processor> pr3(new Sig(1.3f));
    std::unique_ptr<Processor> pr4(new Sig(2.7f));
    std::unique_ptr<Link> li1(new Link(*pr1.get(), 0, *pr2.get(), 0));
    std::unique_ptr<Link> li2(new Link(*pr3.get(), 0, *pr1.get(), 0));
    std::unique_ptr<Link> li3(new Link(*pr2.get(), 0, *pr1.get(), 0)); // Loop
    std::unique_ptr<Link> li4(new Link(*pr4.get(), 0, *pr1.get(), 1));
    try
    {
        Chain chain;
        Chain chain2;
        std::vector<Processor*> processes;
        std::vector<Link*> links;
        processes.push_back(pr1.get());
        processes.push_back(pr2.get());
        processes.push_back(pr3.get());
        processes.push_back(pr4.get());
        links.push_back(li1.get());
        links.push_back(li2.get());
        links.push_back(li4.get());
        //links.push_back(li3.get());
        
        try
        {
            chain.compile(44100, 64, processes, links);
        }
        catch(std::exception& e)
        {
            throw;
        }

        try
        {
            chain2.compile(44100, 64, processes, links);
        }
        catch(std::exception& e)
        {
            std::cout << e.what() << "\n";
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
            throw;
        }
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << "\n";
        return -1;
    }
    
    
    return 0;
}
 */
