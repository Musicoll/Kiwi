//
//  main.cpp
//  KiwiDspTest
//
//  Created by Guillot Pierre on 02/02/2016.
//  Copyright © 2016 CICM. All rights reserved.
//

#include <iostream>
#include <vector>

#include "KiwiDspSignal.hpp"

using namespace kiwi::dsp;

int main(int, const char*[]) {
    try
    {
        Signal sig1(64);
        Signal sig2(sig1);
        Signal sig3(std::move(Signal(128)));
    }
    catch (Error& e)
    {
        std::cout << e.what();
    }
    
    
    return 0;
}
