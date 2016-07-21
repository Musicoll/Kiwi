/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiDsp_Signal.hpp"

namespace kiwi
{
    namespace dsp
    {
        Buffer::Buffer() noexcept :
        m_sample_rate(0ul), m_vector_size(0ul), m_nchannels(0ul), m_signals()
        {
            ;
        }
        
        Buffer::Buffer(const size_t /*nchannels*/, const size_t /*nsamples*/, const sample_t /*val*/)
        {
            
        }
        
        
        Buffer::~Buffer()
        {
            ;
        }
        
        
        
        
        
        
        Signal::Signal() noexcept :
        m_samples(nullptr), m_size(0ul), m_owner(true)
        {
            
        }
        
        Signal::Signal(const size_t size, const sample_t val) :
        m_samples(nullptr), m_size(0ul), m_owner(true)
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
                throw Error("The Signal object can't allocate samples.");
            }
        }
        
        Signal::Signal(Signal const& other) :
        m_samples(nullptr), m_size(0ul), m_owner(true)
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
                    throw Error("The Signal object can't allocate samples.");
                }
            }
        }
        
        Signal::Signal(Signal&& other) noexcept :
        m_samples(nullptr), m_size(0ul), m_owner(other.m_owner)
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
