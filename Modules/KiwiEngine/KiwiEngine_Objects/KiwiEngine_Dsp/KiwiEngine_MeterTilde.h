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

#include <KiwiTool/KiwiTool_Atom.h>
#include <KiwiModel/KiwiModel_Object.h>

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
        
        //! @brief Constructor
        MeterTilde(model::Object const& model, Patcher& patcher, std::vector<tool::Atom> const& atom);
        
        //! @brief The engine's receive method.
        //! @details Does nothing as no control inlets exists.
        void receive(size_t index, std::vector<tool::Atom> const& args) override final;
        
        //! @brief The dsp perform method called.
        void perform(dsp::Buffer const& intput, dsp::Buffer& output);
        
        //! @brief Called to prepare dsp processing.
        void prepare(dsp::Processor::PrepareInfo const& infos) override final;
        
        //! @brief Stops the timer callback method.
        void release() override final;
        
        //! @brief The timer callback method.
        //! @details Sends last peak and trigger signal.
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
