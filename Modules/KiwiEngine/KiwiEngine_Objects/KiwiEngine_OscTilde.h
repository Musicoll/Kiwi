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

#include <KiwiEngine/KiwiEngine_Object.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       OSC~                                       //
    // ================================================================================ //
    
    class OscTilde : public AudioObject
    {
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<Object> create(model::Object const& model, Patcher & patcher);
        
        OscTilde(model::Object const& model, Patcher& patcher);
        
        void receive(size_t index, std::vector<tool::Atom> const& args) override;
        
        void performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        
        void performFreq(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        
        void performPhase(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        
        void performPhaseAndFreq(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        
        void prepare(dsp::Processor::PrepareInfo const& infos) override final;
        
    private: // methods
        
        void setFrequency(dsp::sample_t const& freq) noexcept;
        
        void setOffset(dsp::sample_t const& offset) noexcept;
        
        void setSampleRate(dsp::sample_t const& sample_rate);
        
    private: // members
        
        dsp::sample_t m_sr = 0.f;
        dsp::sample_t m_time = 0.f;
        std::atomic<dsp::sample_t> m_freq{0.f};
        std::atomic<dsp::sample_t> m_offset{0.f};
    };
    
}}
