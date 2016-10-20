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
        
        class ErrorBox : public engine::Object
        {
        public:
            ErrorBox(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override;
        };
        
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
        
        class ObjectPlus : public engine::Object
        {
        public:
            
            ObjectPlus(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override;
            
            void bang();
            
        private:
            double m_lhs = 0.0;
            double m_rhs = 0.0;
        };
        
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
        
        class ObjectPrint : public engine::Object
        {
        public:
            
            ObjectPrint(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t, std::vector<Atom> const& args) override;
            
        private:
            std::string m_name;
        };
        
        // ================================================================================ //
        //                                  OBJECT RECEIVE                                  //
        // ================================================================================ //
        
        class ObjectReceive : public engine::Object, public Beacon::Castaway
        {
        public:
            
            ObjectReceive(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            ~ObjectReceive();
            
            void receive(size_t, std::vector<Atom> const& args) override;
            
            void receive(std::vector<Atom> const& args) override;
            
        private:
            std::string m_name;
        };
        
        // ================================================================================ //
        //                                       DAC~                                       //
        // ================================================================================ //
        
        class DacTilde : public AudioObject
        {
        public:
            
            DacTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override final;
            
            void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept override final;
            
            bool prepare(dsp::Processor::PrepareInfo const& infos) override final;
            
        private:
            engine::AudioControler& m_audio_controler;
        };
        
        // ================================================================================ //
        //                                       OSC~                                       //
        // ================================================================================ //
        
        class OscTilde : public AudioObject
        {
        public: // methods
            
            OscTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override;
            
            void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept override final;
            
            bool prepare(dsp::Processor::PrepareInfo const& infos) override final;
            
        private: // methods
            
            void setFrequency(dsp::sample_t const& freq) noexcept;
            
            void setPhase(dsp::sample_t const& phase) noexcept;
            
            void setSampleRate(dsp::sample_t const& sample_rate);
            
        private: // members
            
            dsp::sample_t m_freq;
            dsp::sample_t m_sr;
            std::atomic<dsp::sample_t> m_phase;
            std::atomic<dsp::sample_t> m_phase_inc;
        };
        
        // ================================================================================ //
        //                                       TIMES~                                       //
        // ================================================================================ //
        
        class TimesTilde : public AudioObject
        {
        public: // methods
            
            TimesTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override;
            
            void performValue(dsp::Buffer const& input, dsp::Buffer& output, dsp::sample_t const& value) noexcept;
            
            void performVec(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept override final;
            
            bool prepare(dsp::Processor::PrepareInfo const& infos) override final;
            
        private: // members
            
            using perform_fn_t = std::function<void(dsp::Buffer const&, dsp::Buffer&)>;
            
            perform_fn_t    m_perform_fn;
            dsp::sample_t   m_value;
            bool            m_constant;
        };
        
    }
}

#endif // KIWI_ENGINE_TYPED_OBJECTS_HPP_INCLUDED
