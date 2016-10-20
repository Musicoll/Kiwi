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

#ifndef KIWI_ENGINE_OBJECTS_HPP_INCLUDED
#define KIWI_ENGINE_OBJECTS_HPP_INCLUDED

#include <cmath>

#include "KiwiEngine_Console.hpp"
#include "KiwiEngine_Objects.hpp"
#include "KiwiEngine_Patcher.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                       NEWBOX                                     //
        // ================================================================================ //
        
        NewBox::NewBox(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : Object(model, patcher)
        {
            ;
        }
        
        void NewBox::receive(size_t index, std::vector<Atom> const& args)
        {
            ;
        }
        
        // ================================================================================ //
        //                                      ERRORBOX                                    //
        // ================================================================================ //
        
        ErrorBox::ErrorBox(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : Object(model, patcher)
        {
            ;
        }
        
        void ErrorBox::receive(size_t index, std::vector<Atom> const& args)
        {
            ;
        }
        
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
        
        ObjectPlus::ObjectPlus(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : Object(model, patcher)
        {
            m_lhs = 0.;
            if(!args.empty() && args[0].isNumber())
            {
                m_rhs = args[0].getFloat();
            }
        }
        
        void ObjectPlus::receive(size_t index, std::vector<Atom> const& args)
        {
            if(!args.empty())
            {
                if(args[0].isNumber())
                {
                    if(index == 0)
                    {
                        m_lhs = args[0].getFloat();
                        bang();
                    }
                    else if(index == 1)
                    {
                        m_rhs = args[0].getFloat();
                    }
                }
                else if(index == 0 && args[0].getString() == "bang")
                {
                    bang();
                }
            }
        }
        
        void ObjectPlus::bang()
        {
            send(0, {m_rhs + m_lhs});
        }
        
        // ================================================================================ //
        //                                    OBJECT PRINT                                  //
        // ================================================================================ //
        
        ObjectPrint::ObjectPrint(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : Object(model, patcher)
        {
            m_name = !args.empty() ? args[0].getString() : "print";
        }
        
        void ObjectPrint::receive(size_t, std::vector<Atom> const& args)
        {
            if(!args.empty())
            {
                post(m_name + " \u2022 " + AtomHelper::toString(args));
            }
        }
        
        // ================================================================================ //
        //                                  OBJECT RECEIVE                                  //
        // ================================================================================ //
        
        ObjectReceive::ObjectReceive(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : Object(model, patcher)
        {
            m_name = !args.empty() ? args[0].getString() : "";
            
            if(!m_name.empty())
            {
                Beacon& beacon = getBeacon(m_name);
                beacon.bind(*this);
            }
        }
        
        ObjectReceive::~ObjectReceive()
        {
            if(!m_name.empty())
            {
                Beacon& beacon = getBeacon(m_name);
                beacon.unbind(*this);
            }
        }
        
        void ObjectReceive::receive(size_t, std::vector<Atom> const& args)
        {
            
        }
        
        void ObjectReceive::receive(std::vector<Atom> const& args)
        {
            if(!args.empty())
            {
                send(0, args);
            }
        }
        
        // ================================================================================ //
        //                                       DAC~                                       //
        // ================================================================================ //
        
        DacTilde::DacTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args):
        AudioObject(model, patcher),
        m_audio_controler(patcher.getAudioControler())
        {
        }
        
        void DacTilde::receive(size_t, std::vector<Atom> const& args)
        {
            if(!args.empty())
            {
                if(args[0].isString())
                {
                    const std::string sym = args[0].getString();
                    if(sym == "start")
                    {
                        m_audio_controler.startAudio();
                    }
                    else if(sym == "stop")
                    {
                        m_audio_controler.stopAudio();
                    }
                }
            }
        }
        
        void DacTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            m_audio_controler.addSignal(input);
        }
        
        bool DacTilde::prepare(dsp::Processor::PrepareInfo const& infos)
        {
            return true;
        }
        
        // ================================================================================ //
        //                                       OSC~                                       //
        // ================================================================================ //
        
        OscTilde::OscTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : AudioObject(model, patcher)
        {
            if (!args.empty() && args[0].isNumber())
            {
                m_freq = args[0].getFloat();
            }
        }
        
        void OscTilde::setFrequency(dsp::sample_t const& freq) noexcept
        {
            m_freq = freq;
            m_phase_inc = (m_freq != 0.) ? (1./(m_sr/(m_freq))) : 0.;
        }
        
        void OscTilde::setSampleRate(dsp::sample_t const& sample_rate)
        {
            m_sr = sample_rate;
            m_phase_inc = (m_freq != 0.) ? (1./(m_sr/(m_freq))) : 0.;
        }
        
        void OscTilde::setPhase(dsp::sample_t const& phase) noexcept
        {
            m_phase = fmod(phase, 1.);
        }
        
        void OscTilde::receive(size_t index, std::vector<Atom> const& args)
        {
            if (index == 0 && args[0].isNumber())
            {
                setFrequency(args[0].getFloat());
            }
            else if(index == 1 && args[0].isNumber())
            {
                setPhase(args[0].getFloat());
            }
        }
        
        bool OscTilde::prepare(PrepareInfo const& infos)
        {
            setSampleRate(static_cast<dsp::sample_t>(infos.sample_rate));
            return true;
        }
        
        void OscTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            dsp::Signal& sig = output[0ul];
            dsp::sample_t* sig_data = sig.data();
            size_t framesize = sig.size();
            while(framesize--)
            {
                *sig_data++ = std::cos(m_phase * 6.283185307179586476925286766559);
                if(m_phase >= 1.) { m_phase = m_phase.load() - 1.; }
                m_phase = m_phase + m_phase_inc;
            }
        }
    }
}


#endif // KIWI_ENGINE_OBJECTS_HPP_INCLUDED
