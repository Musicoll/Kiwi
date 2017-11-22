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

#include <atomic>
#include <chrono>

#include <KiwiTool/KiwiTool_Scheduler.h>

#include <KiwiEngine/KiwiEngine_Object.h>

namespace kiwi { namespace engine {
    
    /// ================================================================================ //
    //                                       METER~                                      //
    // ================================================================================  //
    
    class MeterTilde : public engine::AudioObject, tool::Scheduler<>::Timer
    {
    public: // usings
        
        using clock_t = std::chrono::high_resolution_clock;
        
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<Object> create(model::Object const& model, Patcher & patcher);
        
        MeterTilde(model::Object const& model, Patcher& patcher);
        
        void receive(size_t index, std::vector<tool::Atom> const& args) override final;
        
        void perform(dsp::Buffer const& intput, dsp::Buffer& output);
        
        void prepare(dsp::Processor::PrepareInfo const& infos) override final;
        
        void release() override final;
        
        void timerCallBack() override final;
        
    private: // members
        
        size_t                          m_interval;
        dsp::sample_t                   m_current_peak;
        size_t                          m_sample_index;
        size_t                          m_target_sample_index;
        std::atomic<float>              m_last_peak;
        std::atomic<bool>               m_uptodate;
        flip::Signal<float> &           m_signal;
    };
}
}
