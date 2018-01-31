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
#include <vector>

#include <KiwiTool/KiwiTool_Scheduler.h>
#include <KiwiTool/KiwiTool_CircularBuffer.h>

#include <KiwiEngine/KiwiEngine_Object.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                DELAYSIMPLETILDE                                  //
    // ================================================================================ //
    
    class DelaySimpleTilde : public AudioObject, public tool::Scheduler<>::Timer
    {
    private: //classes
        
        using CircularBuffer = tool::CircularBuffer<dsp::sample_t>;
        
        class ReleasePool
        {
        public: // methods
            
            ReleasePool();
            
            ~ReleasePool();
            
            void add(std::shared_ptr<CircularBuffer> & buffer);
            
            void clear();
            
        private: // members
            
            std::vector<std::shared_ptr<CircularBuffer>>    m_pool;
            mutable std::mutex                              m_mutex;
        };
        
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<Object> create(model::Object const& model, Patcher & patcher);
        
        DelaySimpleTilde(model::Object const& model, Patcher& patcher);
        
        ~DelaySimpleTilde();
        
        void receive(size_t index, std::vector<tool::Atom> const& args) override final;
        
        void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        
        void performDelay(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        
        void prepare(dsp::Processor::PrepareInfo const& infos) override final;
        
        void timerCallBack() override final;
        
        void store(std::shared_ptr<CircularBuffer> new_buffer);
        
        std::shared_ptr<CircularBuffer> load();
        
    private: // methods
        
        dsp::sample_t cubicInterpolate(float const& x,
                                       float const& y0,
                                       float const& y1,
                                       float const& y2,
                                       float const& y3);
        
    private: // members
        
        std::shared_ptr<CircularBuffer>     m_circular_buffer;
        std::unique_ptr<dsp::Signal>        m_reinject_signal;
        float                               m_max_delay;
        std::atomic<float>                  m_delay;
        std::atomic<float>                  m_reinject_level;
        dsp::sample_t                       m_sr;
        ReleasePool                         m_pool;
        mutable std::mutex                  m_mutex;
    };
    
}}
