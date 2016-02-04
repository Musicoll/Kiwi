//
//  main.cpp
//  KiwiDspTest
//
//  Created by Guillot Pierre on 02/02/2016.
//  Copyright © 2016 CICM. All rights reserved.
//

#include <iostream>
#include <vector>

#include "KiwiDspSamples.hpp"

using namespace kiwi::dsp;

int main(int, const char*[]) {
    
    //int* isamples = Sample<int>::allocate(128);
    float* fsamples = Samples<float>::allocate(128);
    double* dsamples = Samples<double>::allocate(128);
    
    for(int i = 1; i < 128; i++)
    {
        if((reinterpret_cast<char *>(fsamples+i)-sizeof(float)) != (reinterpret_cast<char *>(fsamples+i-1)))
        {
            std::cout << "Wrong align float";
        }
        if((reinterpret_cast<char *>(dsamples+i)-sizeof(double)) != (reinterpret_cast<char *>(dsamples+i-1)))
        {
            std::cout << "Wrong align double";
        }
           
    }
    std::cout << "Size of vector : " << sizeof(fsamples) << " and should be " << (128 * sizeof(float)) << "\n";
    std::cout << "Size of vector : " << sizeof(dsamples) << " and should be " << (128 * sizeof(double)) << "\n";
    Samples<float>::release(fsamples);
    Samples<double>::release(dsamples);
    return 0;
}
