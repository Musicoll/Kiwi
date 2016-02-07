/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "KiwiDspLink.hpp"

namespace kiwi
{
    namespace dsp
    {
        Link::Link(Processor const& from, size_t output, Processor const& to, size_t input) noexcept :
        m_from(from), m_output(output), m_to(to), m_input(input)
        {
        }
        
        Link::~Link() noexcept
        {
        }
    }
}
