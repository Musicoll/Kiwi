//
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//

#include "KiwiDspSignal.hpp"

namespace kiwi
{
    namespace dsp
    {
        Buffer::Buffer() noexcept :
        m_sample_rate(0ul), m_vector_size(0ul), m_ninputs(0ul), m_noutputs(0ul),
        m_inputs(nullptr), m_outputs(nullptr)
        {
            
        }
        
        Buffer::~Buffer()
        {
            ;
        }
        
        
        Signal::Signal() noexcept :
        m_samples(nullptr), m_size(0ul), m_owner(true), m_borrowed(false)
        {
            
        }
        
        Signal::Signal(const size_t size, const sample_t val) :
        m_samples(nullptr), m_size(0ul), m_owner(true), m_borrowed(false)
        {
            m_samples = Samples< sample_t >::allocate(size);
            if(m_samples)
            {
                m_size  = size;
                m_owner = true;
                Samples< sample_t >::fill(size, val, m_samples);
            }
            else
            {
                class ErrorAlloc : public Error
                {
                    const char* what() const noexcept final {return "Kiwi::Dsp::Signal : can't allocate samples !";}
                };
                throw Error();
            }
        }
        
        Signal::Signal(Signal const& other) :
        m_samples(nullptr), m_size(0ul), m_owner(true), m_borrowed(false)
        {
            if(other.m_samples && other.m_size)
            {
                m_samples = Samples< sample_t >::allocate(other.m_size);
                if(m_samples)
                {
                    m_size  = other.m_size;
                    m_owner = true;
                    Samples< sample_t >::copy(other.m_size, other.m_samples, m_samples);
                }
                else
                {
                    class ErrorAlloc : public Error
                    {
                        const char* what() const noexcept final {return "Kiwi::Dsp::Signal : can't allocate samples !";}
                    };
                    throw Error();
                }
            }
        }
        
        Signal::Signal(Signal&& other) noexcept :
        m_samples(nullptr), m_size(0ul), m_owner(other.m_owner), m_borrowed(false)
        {
            other.m_owner = true;
            std::swap(m_samples, other.m_samples);
            std::swap(m_size, other.m_size);
        }
        
        Signal::~Signal()
        {
            if(m_owner)
            {
                Samples< sample_t >::release(m_samples);
            }
            m_samples = nullptr;
            m_size    = 0ul;
        }
    }
}
