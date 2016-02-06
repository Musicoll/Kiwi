/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "KiwiDspChain.hpp"

namespace kiwi
{
    namespace dsp
    {
        Chain::Chain() :
        m_running(false), m_sample_rate(0ul), m_vector_size(0ul)
        {
            
        }
    }
}
