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

#pragma once

#include <KiwiTool/KiwiTool_CircularBuffer.h>

#include <KiwiEngine/KiwiEngine_Object.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                DELAYSIMPLETILDE                                  //
        // ================================================================================ //
        
        class DelaySimpleTilde : public AudioObject
        {
        public: // methods
            
            DelaySimpleTilde(model::Object const& model, Patcher& patcher, std::vector<tool::Atom> const& args);
            
            void receive(size_t index, std::vector<tool::Atom> const& args) override final;
            
            void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void performDelay(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void prepare(dsp::Processor::PrepareInfo const& infos) override final;
            
        private: // methods
            
            dsp::sample_t cubicInterpolate(float const& x,
                                           float const& y0,
                                           float const& y1,
                                           float const& y2,
                                           float const& y3);
            
        private: // members
            
            std::unique_ptr<tool::CircularBuffer<dsp::sample_t>>    m_circular_buffer;
            std::unique_ptr<dsp::Signal>                            m_reinject_signal;
            float                                                   m_max_delay;
            std::atomic<float>                                      m_delay;
            std::atomic<float>                                      m_reinject_level;
            dsp::sample_t                                           m_sr;
            size_t                                                  m_vector_size;
            mutable std::mutex                                      m_mutex;
        };
    }
}
