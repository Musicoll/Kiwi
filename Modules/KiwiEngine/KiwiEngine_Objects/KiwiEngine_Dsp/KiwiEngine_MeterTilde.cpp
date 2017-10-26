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

#include <cmath>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Dsp/KiwiModel_MeterTilde.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Dsp/KiwiEngine_MeterTilde.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       METER~                                     //
    // ================================================================================ //
    
    MeterTilde::MeterTilde(model::Object const& model, Patcher& patcher, std::vector<tool::Atom> const& atom):
    engine::AudioObject(model, patcher),
    tool::Scheduler<>::Timer(patcher.getScheduler()),
    m_interval(50),
    m_current_peak(0),
    m_sample_index(0),
    m_target_sample_index(0),
    m_last_peak(0),
    m_uptodate(true),
    m_signal(model.getSignal<float>(model::MeterTilde::Signal::PeakChanged))
    {
    }
    
    void MeterTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
    }
    
    void MeterTilde::perform(dsp::Buffer const& input, dsp::Buffer& output)
    {
        dsp::sample_t const* const input_data = input[0ul].data();
        size_t sample_index = input[0].size();
        
        while(sample_index--)
        {
            m_current_peak = std::max(m_current_peak, std::abs(input_data[sample_index]));
            
            if (m_sample_index++ == m_target_sample_index)
            {
                m_last_peak.store(m_current_peak);
                m_uptodate.store(false);
                m_sample_index = 0;
                m_current_peak = 0;
            }
        }
    }
    
    void MeterTilde::timerCallBack()
    {
        bool expected = false;
        
        if (m_uptodate.compare_exchange_strong(expected, true))
        {
            float last_peak = m_last_peak.load();
            send(0, {last_peak});
            m_signal(last_peak);
        }
    }
    
    void MeterTilde::release()
    {
        stopTimer();
    }
    
    void MeterTilde::prepare(dsp::Processor::PrepareInfo const& infos)
    {
        m_target_sample_index = static_cast<size_t>(infos.sample_rate * (m_interval / 1000.));
        m_sample_index = 0;
        m_current_peak = 0;
        
        setPerformCallBack(this, &MeterTilde::perform);
        
        startTimer(std::chrono::milliseconds(m_interval));
    }
}
}
