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

#ifndef KIWI_ENGINE_TYPED_OBJECTS_HPP_INCLUDED
#define KIWI_ENGINE_TYPED_OBJECTS_HPP_INCLUDED

#include <vector>

#include "KiwiEngine_Object.hpp"
#include "KiwiEngine_Beacon.hpp"

#include "KiwiEngine_AudioControler.hpp"

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
        public:
            
            Receive(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            ~Receive();
            
            void receive(size_t, std::vector<Atom> const& args) override;
            
            void receive(std::vector<Atom> const& args) override;
            
        private:
            std::string m_name;
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
        //                                       AUDIO_INTERFACE                            //
        // ================================================================================ //
        
        class AudioInterfaceObject : public AudioObject
        {
        public: // classes
            
            class Router
            {
            public: // method
                
                Router() = default;
                
                Router(size_t number_of_inputs, size_t number_of_ouputs);
                
                void resize(size_t number_of_inputs, size_t number_of_ouputs);
                
                size_t getNumberOfInput() const;
                
                size_t getNumberOfOutput() const;
                
                void connect(size_t input_index, size_t output_index, bool enable_resize = false);
                
                void disconnect(size_t intput_index, size_t output_index);
                
                bool isConnected(size_t intput_index, size_t output_index) const;
                
                ~Router() = default;
                
            private: // members
                
                std::vector<std::vector<bool>> m_matrix;
            };
            
        public: // methods
            
            AudioInterfaceObject(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const & args) override final;
            
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
    }
}

#endif // KIWI_ENGINE_TYPED_OBJECTS_HPP_INCLUDED
