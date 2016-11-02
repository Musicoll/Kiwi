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
        //                                    OBJECT TIMES                                  //
        // ================================================================================ //
        
        class ObjectTimes : public engine::Object
        {
        public:
            
            ObjectTimes(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override;
            
            void bang();
            
        private:
            double m_lhs = 0.0;
            double m_rhs = 0.0;
        };
        
        // ================================================================================ //
        //                                    OBJECT PRINT                                  //
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
            
            void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void prepare(dsp::Processor::PrepareInfo const& infos) override final;
            
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
            
            void performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void performFreq(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void performPhase(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void performPaseAndFreq(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void prepare(dsp::Processor::PrepareInfo const& infos) override final;
            
        private: // methods
            
            dsp::sample_t computePhaseInc(dsp::sample_t const& freq, dsp::sample_t const& sr) noexcept;
            
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
            
            void performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void performVec(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void prepare(dsp::Processor::PrepareInfo const& infos) override final;
            
        private: // members
            
            dsp::sample_t   m_value;
            bool            m_constant;
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
            
            dsp::sample_t   m_value;
            bool            m_constant;
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
            
            float   m_value;
        };
    }
}

#endif // KIWI_ENGINE_TYPED_OBJECTS_HPP_INCLUDED
