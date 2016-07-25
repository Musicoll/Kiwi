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
        // ================================================================================ //
        //                                       SIGNAL                                     //
        // ================================================================================ //

        Signal::Signal() noexcept : m_owner(true), m_size(0ul), m_samples(nullptr)
        {
            ;
        }
        
        Signal::Signal(const size_t size, const sample_t val) :
        m_owner(true),
        m_size(size),
        m_samples(Samples<sample_t>::allocate(size))
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
        
        Signal::Signal(Signal const& other) : Signal()
        {
            if(other.m_samples && other.m_size)
            {
                m_samples = Samples< sample_t >::allocate(other.m_size);
                if(m_samples != nullptr)
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
        m_owner(other.m_owner),
        m_size(std::move(other.m_size)),
        m_samples(std::move(other.m_samples))
        {
            other.m_owner = false;
            other.m_size = 0ul;
            other.m_samples = nullptr;
        }
        
        Signal& Signal::operator=(Signal other)
        {
            using std::swap;
            swap(m_owner, other.m_owner);
            swap(m_size, other.m_size);
            swap(m_samples, other.m_samples);
            return *this;
        }
        
        Signal::~Signal()
        {
            if(m_samples != nullptr && m_owner)
            {
                Samples< sample_t >::release(m_samples);
            }
            m_samples = nullptr;
            m_size    = 0ul;
        }
        
        void swap(Signal& first, Signal& second)
        {
            using std::swap;
            swap(first.m_owner, second.m_owner);
            swap(first.m_size, second.m_size);
            swap(first.m_samples, second.m_samples);
        }
        
        size_t Signal::size() const noexcept
        {
            return m_size;
        }
        
        bool Signal::empty() const noexcept
        {
            return (m_size == 0ul);
        }
        
        sample_t const* Signal::data() const noexcept
        {
            return const_cast<sample_t const*>(m_samples);
        }
        
        sample_t* Signal::data() noexcept
        {
            return m_samples;
        }
        
        sample_t const& Signal::operator[](const size_t index) const
        {
            assert(index < size() && "Index out of range.");
            
            return m_samples[index];
        }
        
        sample_t& Signal::operator[](const size_t index)
        {
            assert(index < size() && "Index out of range.");
            
            return m_samples[index];
        }
        
        void Signal::clear() noexcept
        {
            Samples<sample_t>::clear(m_size, m_samples);
        }
        
        void Signal::fillWith(sample_t const& value) noexcept
        {
            Samples<sample_t>::fill(m_size, value, m_samples);
        }
        
        void Signal::add(Signal const& signal) noexcept
        {
            Samples<sample_t>::add(m_size, signal.data(), m_samples);
        }
        
        Signal Signal::add(Signal const& signal_1, Signal const& signal_2)
        {
            const size_t size = signal_1.size();
            
            assert(size == signal_2.size()
                   && "The two signals must have an equal size");
            
            Signal result(size);
            Samples<sample_t>::add(size, signal_1.data(), signal_2.data(), result.data());
            
            return std::move(result);
        }
    }
}
