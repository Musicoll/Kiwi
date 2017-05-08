/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiDsp_Signal.hpp"
#include "KiwiDsp_Misc.hpp"

namespace kiwi
{
    namespace dsp
    {
        
        // ================================================================================ //
        //                                       SIGNAL                                     //
        // ================================================================================ //
        
        Signal::Signal(const size_t size, const sample_t val) :
        m_size(size),
        m_samples(new sample_t[size])
        {
            assert(size && "size must be greater than 0");
            fill(val);
        }
        
        Signal::Signal(Signal&& other) noexcept :
        m_size(std::move(other.m_size)),
        m_samples(std::move(other.m_samples))
        {
            other.m_size = 0ul;
            other.m_samples = nullptr;
        }
        
        Signal& Signal::operator=(Signal&& other) noexcept
        {
            m_size = std::move(other.m_size);
            m_samples = std::move(other.m_samples);
            other.m_size = 0ul;
            other.m_samples = nullptr;
            
            return *this;
        }
        
        Signal::~Signal()
        {
            if(m_samples != nullptr)
            {
                delete [] m_samples;
            }
            m_samples = nullptr;
            m_size    = 0ul;
        }
        
        size_t Signal::size() const noexcept
        {
            return m_size;
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
        
        void Signal::fill(sample_t const& value) noexcept
        {
            std::fill(m_samples, m_samples + m_size, value);
        }
        
        void Signal::copy(Signal const& other_signal) noexcept
        {
            assert(m_size == other_signal.size() && "Copying signals of different size");
            
            sample_t const* in = other_signal.m_samples;
            sample_t* out = m_samples;
            
            for(size_t i = m_size>>3; i; --i, in += 8, out += 8)
            {
                out[0] = in[0]; out[1] = in[1]; out[2] = in[2]; out[3] = in[3];
                out[4] = in[4]; out[5] = in[5]; out[6] = in[6]; out[7] = in[7];
            }
            for(size_t i = m_size&7; i; --i, in++, out++)
            {
                out[0] = in[0];
            }
        }
        
        void Signal::add(Signal const& other_signal) noexcept
        {
            assert(m_size == other_signal.size() && "Adding signals of different size");
            
            sample_t const* in = other_signal.m_samples;
            sample_t* out = m_samples;
            
            for(size_t i = m_size>>3; i; --i, in += 8, out += 8)
            {   
                out[0] += in[0]; out[1] += in[1]; out[2] += in[2]; out[3] += in[3];
                out[4] += in[4]; out[5] += in[5]; out[6] += in[6]; out[7] += in[7];
            }
            for(size_t i = m_size&7; i; --i, in++, out++)
            {
                out[0] += in[0];
            }
        }
        
        void Signal::add(Signal const& signal_1, Signal const& signal_2, Signal& result)
        {
            const size_t size = signal_1.size();
            
            assert(size == signal_2.size()
                   && "The two signals must have an equal size");
            
            sample_t const* in1 = signal_1.m_samples;
            sample_t const* in2 = signal_2.m_samples;
            sample_t* out = result.m_samples;
            
            for(size_t i = size>>3; i; --i, in1 += 8, in2 += 8, out += 8)
            {
                out[0] = in1[0] + in2[0]; out[1] = in1[1] + in2[1];
                out[2] = in1[2] + in2[2]; out[3] = in1[3] + in2[3];
                out[4] = in1[4] + in2[4]; out[5] = in1[5] + in2[5];
                out[6] = in1[6] + in2[6]; out[7] = in1[7] + in2[7];
            }
            for(size_t i = size&7; i; --i, in1++, in2++, out++)
            {
                out[0] = in1[0] + in2[0];
            }
        }
        
        // ================================================================================ //
        //                                      BUFFER                                      //
        // ================================================================================ //
        
        Buffer::Buffer():
        m_vectorsize(),
        m_signals()
        {
        }
        
        Buffer::Buffer(std::vector<Signal::sPtr> signals)
        {
            const size_t nsignals = signals.size();
            if(nsignals)
            {
                m_signals.reserve(nsignals);
                
                const Signal::sPtr first_sig(signals[0]);
                const size_t vectorsize = first_sig->size();
                m_signals.emplace_back(std::move(first_sig));
                
                for(int i = 1; i < signals.size(); ++i)
                {
                    const Signal::sPtr sig(signals[i]);
                    assert(sig->size() == vectorsize
                           && "Aggregating signals with different vector size");
                    
                    m_signals.emplace_back(std::move(sig));
                }
                
                m_vectorsize = vectorsize;
            }
            else
            {
                m_vectorsize = 0;
            }
        }
        
        Buffer::Buffer(const size_t nchannels, const size_t nsamples, const sample_t val) :
        m_vectorsize(nsamples)
        {
            m_signals.reserve(nchannels);
            for(size_t i = 0ul; i < nchannels; i++)
            {
                m_signals.emplace_back(std::make_shared<Signal>(nsamples, val));
            }
        }
        
        Buffer::Buffer(Buffer&& other) noexcept :
        m_vectorsize(std::move(other.m_vectorsize)),
        m_signals(std::move(other.m_signals))
        {
            other.m_vectorsize = 0ul;
        }
        
        Buffer& Buffer::operator=(Buffer&& other) noexcept
        {
            m_vectorsize = std::move(other.m_vectorsize);
            m_signals = std::move(other.m_signals);
            other.m_vectorsize = 0;
            return *this;
        }
        
        Buffer::~Buffer()
        {
            ;
        }
        
        void Buffer::setChannels(std::vector<Signal::sPtr> signals)
        {
            m_signals.clear();
            
            const size_t nsignals = signals.size();
            if(nsignals)
            {
                m_signals.reserve(nsignals);
                
                const Signal::sPtr first_sig(signals[0]);
                const size_t vectorsize = first_sig->size();
                m_signals.emplace_back(std::move(first_sig));
                
                for(int i = 1; i < signals.size(); ++i)
                {
                    const Signal::sPtr sig(signals[i]);
                    assert(sig->size() == vectorsize
                           && "Aggregating signals with different vector size");
                    
                    m_signals.emplace_back(std::move(sig));
                }
                
                m_vectorsize = vectorsize;
            }
            else
            {
                m_vectorsize = 0;
            }
        }

        void Buffer::clear()
        {
            m_signals.clear();
        }
        
        size_t Buffer::getVectorSize() const noexcept
        {
            return m_vectorsize;
        }
        
        size_t Buffer::getNumberOfChannels() const noexcept
        {
            return m_signals.size();
        }
        
        bool Buffer::empty() const noexcept
        {
            return (m_signals.size() == 0ul);
        }
        
        Signal const& Buffer::operator[](const size_t index) const
        {
            assert(index < m_signals.size() && "Index out of range.");
            
            return *m_signals[index].get();
        }
        
        Signal& Buffer::operator[](const size_t index)
        {
            assert(index < m_signals.size() && "Index out of range.");
            
            return *m_signals[index].get();
        }
    }
}
