/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
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

#include <queue>

namespace kiwi { namespace engine {
    
    class Ramp
    {
    public: // classes
        
        struct ValueTimePair
        {
            ValueTimePair(dsp::sample_t value_,
                          dsp::sample_t time_ms_)
            : value(value_)
            , time_ms(time_ms_)
            {
                assert(time_ms_ >= 0.);
            }
            
            dsp::sample_t value;
            dsp::sample_t time_ms;
        };
        
    public: // methods
        
        //! @brief Constructor
        Ramp(dsp::sample_t start = 0) noexcept;
        
        //! @brief Set sample rate.
        //! @details The sample rate is used to compute the step value of the ramp.
        //! @param sample_rate The current sample rate.
        void setSampleRate(double sample_rate) noexcept;
        
        //! @brief Set the function to call when the ramp reached its final destination.
        //! @details This callback function will be called by the getNextValue() method.
        //! @param callback The callback function to be called.
        void setEndOfRampCallback(std::function<void()> callback);
        
        //! @brief Set a new value directly.
        //! @param new_value New value
        void setValueDirect(dsp::sample_t new_value) noexcept;
        
        //! @brief Resets the value-time pairs of the ramp.
        //! @param value_time_pairs A vector of ValueTimePair.
        void setValueTimePairs(std::vector<ValueTimePair> value_time_pairs);
        
        //! @brief Compute and returns the next value.
        //! @details The sampling rate must be set before calling this method.
        dsp::sample_t getNextValue() noexcept;
        
        //! @brief Returns the current value.
        dsp::sample_t getValue() noexcept;
        
    private: // methods
        
        void reset();
        void setNextValueTime(ValueTimePair const& value_time_pair) noexcept;
        void triggerNextRamp();
        
    private: // variables
        
        std::mutex m_lock;
        double m_sr = 0.;
        dsp::sample_t m_current_value = 0, m_destination_value = 0, m_step = 0;
        int m_countdown = 0, m_steps_to_destination = 0;
        
        std::vector<ValueTimePair> m_value_time_pairs {};
        size_t m_valuetime_pairs_countdown = 0;
        
        std::function<void()> m_ramp_ended_callback = nullptr;
        bool m_should_notify_end {false};
    };
    
    // ================================================================================ //
    //                                      LINE~                                       //
    // ================================================================================ //
    
    class LineTilde : public AudioObject
    {
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<Object> create(model::Object const& model, Patcher & patcher);
        
        LineTilde(model::Object const& model, Patcher& patcher);
        
        ~LineTilde();
        
        void receive(size_t index, std::vector<tool::Atom> const& args) override;
        
        void prepare(dsp::Processor::PrepareInfo const& infos) override;
        
        void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        
    private: // methods
        
        std::vector<Ramp::ValueTimePair> parseAtomsAsValueTimePairs(std::vector<tool::Atom> const& atoms) const;
        
    private: // variables
        
        class BangTask;
        std::shared_ptr<BangTask> m_bang_task;
        
        double m_next_ramp_time_ms;
        bool m_next_ramp_time_consumed;
        
        Ramp m_ramp;
    };
    
}}
