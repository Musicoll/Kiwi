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

#include <vector>
#include <memory>
#include <set>

#include "KiwiEngine_Object.h"
#include "KiwiEngine_Beacon.h"
#include "KiwiEngine_CircularBuffer.h"
#include "KiwiEngine_AudioControler.h"
#include "KiwiEngine_Scheduler.h"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                       NEWBOX                                     //
        // ================================================================================ //
        
        class NewBox : public engine::Object
        {
        public:
            NewBox(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override;
        };
        
        // ================================================================================ //
        //                                      ERRORBOX                                    //
        // ================================================================================ //
        
        class ErrorBox : public AudioObject
        {
        public:
            ErrorBox(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override;
            
            void prepare(dsp::Processor::PrepareInfo const& infos) override final;
        };
        
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
        
        class Plus : public engine::Object
        {
        public:
            
            Plus(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override;
            
            void bang();
            
        private:
            double m_lhs = 0.0;
            double m_rhs = 0.0;
        };
        
        // ================================================================================ //
        //                                    OBJECT TIMES                                  //
        // ================================================================================ //
        
        class Times : public engine::Object
        {
        public:
            
            Times(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override;
            
            void bang();
            
        private:
            double m_lhs = 0.0;
            double m_rhs = 0.0;
        };
        
        // ================================================================================ //
        //                                    OBJECT PRINT                                  //
        // ================================================================================ //
        
        class Print : public engine::Object
        {
        public:
            
            Print(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t, std::vector<Atom> const& args) override;
            
        private:
            std::string m_name;
        };
        
        // ================================================================================ //
        //                                  OBJECT RECEIVE                                  //
        // ================================================================================ //
        
        class Receive : public engine::Object, public Beacon::Castaway
        {
        private: // classes
            
            class Task;
            
        public: // methods
            
            Receive(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            ~Receive();
            
            void receive(size_t, std::vector<Atom> const& args) override;
            
            void receive(std::vector<Atom> const& args) override;
            
        private: // members
            
            std::string                     m_name;
            std::set<std::unique_ptr<Task>> m_tasks;
        };
        
        // ================================================================================ //
        //                                  OBJECT LOADMESS                                 //
        // ================================================================================ //
        
        class Loadmess : public engine::Object
        {
        public:
            
            Loadmess(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            ~Loadmess() = default;
            
            void receive(size_t, std::vector<Atom> const& args) override;
            
            void loadbang() override;
            
        private:
            
            const std::vector<Atom> m_args;
        };
        
        // ================================================================================ //
        //                                  OBJECT DELAY                                    //
        // ================================================================================ //
        
        class Delay final : public engine::Object
        {
        public: // methods
            
            Delay(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            ~Delay();
            
            void receive(size_t index, std::vector<Atom> const& args) override;
            
        private: // members
            
            struct Task final : public Scheduler<>::Task
            {
                Task(Delay & object);
                ~Task() = default;
                void execute() override;
                
            private:
                Delay& m_object;
            };
            
            Task                            m_task;
            Scheduler<>::clock_t::duration  m_delay;
        };
        
        // ================================================================================ //
        //                                  OBJECT PIPE                                     //
        // ================================================================================ //
        
        class Pipe final : public engine::Object
        {
        public: // classes
            
            class Task;
            
        public: // methods
            
            Pipe(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            ~Pipe();
            
            void receive(size_t index, std::vector<Atom> const& args) override;
            
        private: // members
            
            std::set<std::unique_ptr<Task>>     m_tasks;
            Scheduler<>::clock_t::duration      m_delay;
        };
        
        // ================================================================================ //
        //                                  OBJECT METRO                                    //
        // ================================================================================ //
        
        class Metro final : public engine::Object, Scheduler<>::Timer
        {
        public: // methods
            
            Metro(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            ~Metro();
            
            void receive(size_t index, std::vector<Atom> const& oargs) override;
            
            void timerCallBack() override;
            
        private: // members
            
            Scheduler<>::duration_t m_period;
        };
        
        
        // ================================================================================ //
        //                                       ROUTER                                     //
        // ================================================================================ //
        
        class Router
        {
        public: // classes
            
            struct Cnx
            {
                Cnx(size_t input, size_t output);
                
                bool operator<(Cnx const& other) const;
             
                size_t m_input = 0;
                size_t m_output = 0;
            };
            
        public: // method
            
            Router() = default;
            
            void connect(size_t input_index, size_t output_index);
            
            void disconnect(size_t intput_index, size_t output_index);
            
            size_t getNumberOfConnections() const;
            
            std::set<Cnx> const& getConnections() const;
            
            ~Router() = default;
            
        private: // memebers
            
            std::set<Cnx> m_cnx;
        };
        
        // ================================================================================ //
        //                                       AUDIO_INTERFACE                            //
        // ================================================================================ //
        
        class AudioInterfaceObject : public AudioObject
        {
        public: // methods
            
            AudioInterfaceObject(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const & args) override final;
            
            std::vector<size_t> parseArgs(std::vector<Atom> const& args) const;
            
            virtual ~AudioInterfaceObject() = default;
            
        protected: // members
            
            Router                      m_router;
            engine::AudioControler&     m_audio_controler;
        };
        
        // ================================================================================ //
        //                                       ADC~                                       //
        // ================================================================================ //
        
        class AdcTilde : public AudioInterfaceObject
        {
        public: // methods
            
            AdcTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void prepare(dsp::Processor::PrepareInfo const& infos) override final;
        };
        
        // ================================================================================ //
        //                                       DAC~                                       //
        // ================================================================================ //
        
        class DacTilde : public AudioInterfaceObject
        {
        public: // methods
            
            DacTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void prepare(dsp::Processor::PrepareInfo const& infos) override final;
        };
        
        // ================================================================================ //
        //                                       OSC~                                       //
        // ================================================================================ //
        
        class OscTilde : public AudioObject
        {
        public: // methods
            
            OscTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override;
            
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
        
        // ================================================================================ //
        //                                       TIMES~                                       //
        // ================================================================================ //
        
        class TimesTilde : public AudioObject
        {
        public: // methods
            
            TimesTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override;
            
            void performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void performVec(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void prepare(dsp::Processor::PrepareInfo const& infos) override final;
            
        private: // members
            
            std::atomic<dsp::sample_t>   m_value{0.f};
        };
        
        // ================================================================================ //
        //                                       PLUS~                                      //
        // ================================================================================ //
        
        class PlusTilde : public AudioObject
        {
        public: // methods
            
            PlusTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override;
            
            void performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void performVec(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void prepare(dsp::Processor::PrepareInfo const& infos) override final;
            
        private: // members
            
            std::atomic<dsp::sample_t>   m_value{0.f};
        };
        
        // ================================================================================ //
        //                                       SIG~                                       //
        // ================================================================================ //
        
        class SigTilde : public AudioObject
        {
        public: // methods
            
            SigTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override final;
            
            void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void prepare(dsp::Processor::PrepareInfo const& infos) override final;
            
        private: // members
            
            std::atomic<float>   m_value{0.f};
        };
        
        // ================================================================================ //
        //                                DELAYSIMPLETILDE                                  //
        // ================================================================================ //
        
        class DelaySimpleTilde : public AudioObject
        {
        public: // methods
            
            DelaySimpleTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override final;
            
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
            
            std::unique_ptr<CircularBuffer<dsp::sample_t>>   m_circular_buffer;
            std::unique_ptr<dsp::Signal>                     m_reinject_signal;
            float                                            m_max_delay;
            std::atomic<float>                               m_delay;
            std::atomic<float>                               m_reinject_level;
            dsp::sample_t                                    m_sr;
            size_t                                           m_vector_size;
            mutable std::mutex                               m_mutex;
        };
    }
}
