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

#include "KiwiDsp_Buffer.hpp"

namespace kiwi
{
    namespace dsp
    {
        // ================================================================================ //
        //                                      BUFFER                                      //
        // ================================================================================ //
        
        Buffer::Buffer() noexcept : m_vectorsize(0ul), m_signals()
        {
            ;
        }
        
        Buffer::Buffer(const size_t nchannels) :
        m_vectorsize(0ul),
        m_signals(nchannels, std::make_shared<Signal>())
        {
            ;
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
        
        Buffer::Buffer(Buffer const& other) :
        m_vectorsize(other.m_vectorsize),
        m_signals(other.m_signals)
        {
            ;
        }
        
        Buffer::Buffer(Buffer&& other) noexcept :
        m_vectorsize(std::move(other.m_vectorsize)),
        m_signals(std::move(other.m_signals))
        {
            other.m_vectorsize = 0ul;
        }
        
        Buffer::~Buffer()
        {
            ;
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

            return *(const_cast<Buffer*>(this)->m_signals[index]);
        }
        
        Signal& Buffer::operator[](const size_t index)
        {
            assert(index < m_signals.size() && "Index out of range.");
            
            return *m_signals[index].get();
        }
        
        Signal::sPtr& Buffer::getChannelPtr(const size_t index)
        {
            return m_signals[index];
        }
    }
}
