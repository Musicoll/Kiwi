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
        : AudioObject(model, patcher)
        {
            ;
        }
        
        void ErrorBox::receive(size_t index, std::vector<Atom> const& args)
        {
            ;
        }
        
        void ErrorBox::prepare(dsp::Processor::PrepareInfo const& infos)
        {
            ;
        }
        
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
        
        Plus::Plus(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : Object(model, patcher)
        {
            m_lhs = 0.;
            if(!args.empty() && args[0].isNumber())
            {
                m_rhs = args[0].getFloat();
            }
        }
        
        void Plus::receive(size_t index, std::vector<Atom> const& args)
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
        
        void Plus::bang()
        {
            send(0, {m_rhs + m_lhs});
        }
        
        // ================================================================================ //
        //                                    OBJECT TIMES                                  //
        // ================================================================================ //
        
        Times::Times(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : Object(model, patcher)
        {
            m_lhs = 0.;
            if(!args.empty() && args[0].isNumber())
            {
                m_rhs = args[0].getFloat();
            }
        }
        
        void Times::receive(size_t index, std::vector<Atom> const& args)
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
        
        void Times::bang()
        {
            send(0, {m_rhs * m_lhs});
        }
        
        // ================================================================================ //
        //                                    OBJECT PRINT                                  //
        // ================================================================================ //
        
        Print::Print(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : Object(model, patcher)
        {
            m_name = !args.empty() ? args[0].getString() : "print";
        }
        
        void Print::receive(size_t, std::vector<Atom> const& args)
        {
            if(!args.empty())
            {
                post(m_name + " \xe2\x80\xa2 " + AtomHelper::toString(args));
            }
        }
        
        // ================================================================================ //
        //                                  OBJECT RECEIVE                                  //
        // ================================================================================ //
        
        class Receive::Task final : public Scheduler<>::Task
        {
        public: // methods
            
            Task(Receive& object, std::vector<Atom> const& atoms):
            Scheduler<>::Task(Thread::Gui, Thread::Engine),
            m_object(object),
            m_atoms(atoms)
            {
            }
            
            ~Task()
            {
            }
            
            void execute() override
            {
                m_object.send(0, m_atoms);
                
                m_object.m_tasks.erase(std::find_if(m_object.m_tasks.begin(),
                                                    m_object.m_tasks.end(),
                                                    [this](std::unique_ptr<Task> const& task)
                                                    {
                                                        return task.get() == this;
                                                    }));
            }
            
        private: // members
            
            Receive&            m_object;
            std::vector<Atom>   m_atoms;
        };
        
        Receive::Receive(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args):
        Object(model, patcher),
        m_name(),
        m_tasks()
        {
            m_name = !args.empty() ? args[0].getString() : "";
            
            if(!m_name.empty())
            {
                Beacon& beacon = getBeacon(m_name);
                beacon.bind(*this);
            }
        }
        
        Receive::~Receive()
        {
            if(!m_name.empty())
            {
                Beacon& beacon = getBeacon(m_name);
                beacon.unbind(*this);
            }
        }
        
        void Receive::receive(size_t, std::vector<Atom> const& args)
        {
            
        }
        
        void Receive::receive(std::vector<Atom> const& args)
        {
            if (!args.empty())
            {
                Task* task = new Task(*this, args);
                Scheduler<>::use().schedule(task, std::chrono::milliseconds(0));
                m_tasks.insert(std::unique_ptr<Task>(task));
            }
        }
        
        // ================================================================================ //
        //                                  OBJECT LOADMESS                                 //
        // ================================================================================ //
        
        Loadmess::Loadmess(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : Object(model, patcher),
        m_args(args)
        {
            ;
        }
        
        void Loadmess::receive(size_t, std::vector<Atom> const& args)
        {
            send(0ul, m_args);
        }
        
        void Loadmess::loadbang()
        {
            send(0ul, m_args);
        }
        
        // ================================================================================ //
        //                                     OBJECT DELAY                                 //
        // ================================================================================ //
        
        Delay::Task::Task(Delay& object):
        Scheduler<>::Task(Thread::Engine, Thread::Engine),
        m_object(object)
        {
        }
        
        void Delay::Task::execute()
        {
            m_object.send(0, {"bang"});
        }
        
        Delay::Delay(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args):
        Object(model, patcher),
        m_task(*this),
        m_delay(std::chrono::milliseconds(0))
        {
            if (!args.empty())
            {
                m_delay = std::chrono::milliseconds(args[0].getInt());
            }
        }
        
        Delay::~Delay()
        {
        }
        
        void Delay::receive(size_t index, std::vector<Atom> const& args)
        {
            if (!args.empty())
            {
                if (index == 0)
                {
                    if(args[0].isString() && args[0].getString() == "bang")
                    {
                        Scheduler<>::use().schedule(&m_task, m_delay);
                    }
                    else if(args[0].isString() && args[0].getString() == "stop")
                    {
                        Scheduler<>::use().unschedule(&m_task);
                    }
                }
                else if(index == 1)
                {
                    if (args[0].isNumber())
                    {
                        m_delay = std::chrono::milliseconds(args[0].getInt());
                    }
                }
            }
        }
        
        // ================================================================================ //
        //                                  OBJECT PIPE                                     //
        // ================================================================================ //
        
        class Pipe::Task final : public Scheduler<>::Task
        {
        public: // methods
            
            Task(Pipe & object, std::vector<Atom> const& atoms):
            Scheduler<>::Task(Thread::Engine, Thread::Engine),
            m_object(object),
            m_atoms(atoms)
            {
            }
            
            ~Task()
            {
            }
            
            void execute()
            {
                m_object.send(0, m_atoms);
                
                m_object.m_tasks.erase(std::find_if(m_object.m_tasks.begin(),
                                                    m_object.m_tasks.end(),
                                                    [this](std::unique_ptr<Task> const& task)
                {
                    return task.get() == this;
                }));
            }
            
        private: // members
            
            Pipe&               m_object;
            std::vector<Atom>   m_atoms;
        };
        
        Pipe::Pipe(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args):
        Object(model, patcher),
        m_tasks(),
        m_delay(std::chrono::milliseconds(0))
        {
            if (!args.empty())
            {
                m_delay = std::chrono::milliseconds(args[0].getInt());
            }
        }
        
        Pipe::~Pipe()
        {
        }
        
        void Pipe::receive(size_t index, std::vector<Atom> const& args)
        {
            if (!args.empty())
            {
                if (index == 0)
                {
                    Task* task = new Task(*this, args);
                    Scheduler<>::use().schedule(task, m_delay);
                    m_tasks.insert(std::unique_ptr<Task>(task));
                }
                else if(index == 1 && args[0].isNumber())
                {
                    m_delay = std::chrono::milliseconds(args[0].getInt());
                }
            }
        }
        
        // ================================================================================ //
        //                                  OBJECT METRO                                    //
        // ================================================================================ //
        
        Metro::Metro(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args):
        engine::Object(model, patcher),
        Scheduler<>::Timer(Thread::Engine, Thread::Engine),
        m_period(std::chrono::milliseconds(0))
        {
            if(!args.empty())
            {
                m_period = std::chrono::milliseconds(args[0].getInt());
            }
        }
        
        Metro::~Metro()
        {
        }
        
        void Metro::receive(size_t index, std::vector<Atom> const& args)
        {
            if (!args.empty())
            {
                if (index == 0)
                {
                    if((args[0].isString() && args[0].getString() == "start")
                       || (args[0].isNumber() && static_cast<bool>(args[0].getInt())))
                    {
                        startTimer(m_period);
                    }
                    else if((args[0].isString() && args[0].getString() == "stop")
                            || (args[0].isNumber() && !static_cast<bool>(args[0].getInt())))
                    {
                        stopTimer();
                    }
                }
                else if(index == 1)
                {
                    if (args[0].isNumber())
                    {
                        m_period = std::chrono::milliseconds(args[0].getInt());
                    }
                }
            }
        }
        
        void Metro::timerCallBack()
        {
            send(0, {"bang"});
        }
        
        // ================================================================================ //
        //                                       ROUTER                                     //
        // ================================================================================ //
        
        Router::Cnx::Cnx(size_t input, size_t output):
        m_input(input),
        m_output(output)
        {
        }
        
        bool Router::Cnx::operator<(Cnx const& other) const
        {
            return m_input < other.m_input
                   || (m_input == other.m_input && m_output < other.m_output);
        }
        
        void Router::connect(size_t input_index, size_t output_index)
        {
            m_cnx.insert(Cnx(input_index, output_index));
        }
        
        void Router::disconnect(size_t input_index, size_t ouptut_index)
        {
            m_cnx.erase(Cnx(input_index, ouptut_index));
        }
        
        size_t Router::getNumberOfConnections() const
        {
            return m_cnx.size();
        }
        
        std::set<Router::Cnx> const& Router::getConnections() const
        {
            return m_cnx;
        }
        
        // ================================================================================ //
        //                                       AUDIOINTERFACE                             //
        // ================================================================================ //
        
        AudioInterfaceObject::AudioInterfaceObject(model::Object const& model,
                                                   Patcher& patcher,
                                                   std::vector<Atom> const& args):
        AudioObject(model, patcher),
        m_router(),
        m_audio_controler(patcher.getAudioControler())
        {
        }
        
        std::vector<size_t> AudioInterfaceObject::parseArgs(std::vector<Atom> const& args) const
        {
            std::vector<size_t> routes;
            
            for(Atom const& arg : args)
            {
                if (arg.isNumber())
                {
                    routes.push_back(arg.getInt() - 1);
                }
                else if(arg.isString())
                {
                    std::string inputs(arg.getString());
                    
                    int left_input = std::stoi(inputs.substr(0, inputs.find(":"))) - 1;
                    int right_input = std::stoi(inputs.substr(inputs.find(":") + 1)) - 1;
                    
                    bool rev = left_input > right_input;
                    
                    for (int channel = left_input; rev ? channel >= right_input : channel <= right_input; rev ? --channel : ++channel)
                    {
                        routes.push_back(channel);
                    }
                }
            }
            
            if (routes.empty())
            {
                routes = {0, 1};
            }
            
            return routes;
        }
        
        void AudioInterfaceObject::receive(size_t index, std::vector<Atom> const & args)
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
        
        // ================================================================================ //
        //                                       ADC~                                       //
        // ================================================================================ //
        
        AdcTilde::AdcTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args):
        AudioInterfaceObject(model, patcher, args)
        {
            std::vector<size_t> routes = parseArgs(args);
            
            for (size_t index = 0; index < routes.size(); ++index)
            {
                m_router.connect(routes[index], index);
            }
        }
        
        void AdcTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            std::set<Router::Cnx> const& connections = m_router.getConnections();
            
            for(Router::Cnx const& connection : connections)
            {
                m_audio_controler.getFromChannel(connection.m_input, output[connection.m_output]);
            }
        }
        
        void AdcTilde::prepare(dsp::Processor::PrepareInfo const& infos)
        {
            setPerformCallBack(this, &AdcTilde::perform);
        }
        
        // ================================================================================ //
        //                                       DAC~                                       //
        // ================================================================================ //
        
        DacTilde::DacTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args):
        AudioInterfaceObject(model, patcher, args)
        {
            std::vector<size_t> routes = parseArgs(args);
            
            for (size_t index = 0; index < routes.size(); ++index)
            {
                m_router.connect(index, routes[index]);
            }
        }
        
        void DacTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            std::set<Router::Cnx> const& connections = m_router.getConnections();
            
            for(Router::Cnx const& connection : connections)
            {
                m_audio_controler.addToChannel(connection.m_output, input[connection.m_input]);
            }
        }
        
        void DacTilde::prepare(dsp::Processor::PrepareInfo const& infos)
        {
            setPerformCallBack(this, &DacTilde::perform);
        }
        
        // ================================================================================ //
        //                                       OSC~                                       //
        // ================================================================================ //
        
        OscTilde::OscTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : AudioObject(model, patcher)
        {
            if (!args.empty() && args[0].isNumber())
            {
                setFrequency(args[0].getFloat());
            }
        }
        
        void OscTilde::setFrequency(dsp::sample_t const& freq) noexcept
        {
            m_freq = freq;
        }
        
        void OscTilde::setSampleRate(dsp::sample_t const& sample_rate)
        {
            m_sr = sample_rate;
        }
        
        void OscTilde::setOffset(dsp::sample_t const& offset) noexcept
        {
            m_offset = fmodf(offset, 1.f);
        }
        
        void OscTilde::receive(size_t index, std::vector<Atom> const& args)
        {            
            if (index == 0 && args[0].isNumber())
            {
                setFrequency(args[0].getFloat());
            }
            else if(index == 1 && args[0].isNumber())
            {
                setOffset(args[0].getFloat());
            }
        }
        
        void OscTilde::prepare(PrepareInfo const& infos)
        {
            setSampleRate(static_cast<dsp::sample_t>(infos.sample_rate));
            
            if (infos.inputs[0] && infos.inputs[1])
            {
                setPerformCallBack(this, &OscTilde::performPhaseAndFreq);
            }
            else if(infos.inputs[0])
            {
                setPerformCallBack(this, &OscTilde::performFreq);
            }
            else if(infos.inputs[1])
            {
                setPerformCallBack(this, &OscTilde::performPhase);
            }
            else
            {
                setPerformCallBack(this, &OscTilde::performValue);
            }
        }
        
        void OscTilde::performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            dsp::sample_t *sig_data = output[0ul].data();
            size_t sample_index = output[0ul].size();
            dsp::sample_t const time_inc = m_freq/m_sr;
            dsp::sample_t const offset = m_offset;
            
            while(sample_index--)
            {
                *sig_data++ = std::cos(2.f * dsp::pi * (m_time + offset));
                m_time += time_inc;
            }
            
            m_time = fmodf(m_time, 1.f);
        }
        
        void OscTilde::performFreq(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            size_t sample_index = output[0ul].size();
            dsp::sample_t* output_sig = output[0ul].data();
            dsp::sample_t const* freq = input[0ul].data();
            dsp::sample_t const offset = m_offset;
            
            while(sample_index--)
            {
                *output_sig++ = std::cos(2.f *dsp::pi * (m_time + offset));
                m_time += (*freq++ / m_sr);
            }
            
            m_time = fmodf(m_time, 1.f);
        }
        
        void OscTilde::performPhase(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            dsp::sample_t* output_sig = output[0ul].data();
            size_t sample_index = output[0ul].size();
            dsp::sample_t const* phase = input[1ul].data();
            dsp::sample_t const time_inc = m_freq/m_sr;
            
            while(sample_index--)
            {
                *output_sig++  = std::cos(2.f * dsp::pi * (m_time + fmodf(*phase++, 1.f)));
                m_time += time_inc;
            }
            
            m_time = fmodf(m_time, 1.f);
        }
        
        void OscTilde::performPhaseAndFreq(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            size_t sample_index = output[0].size();
            dsp::sample_t* output_sig = output[0].data();
            dsp::sample_t const* freq = input[0].data();
            dsp::sample_t const* phase = input[1].data();
            
            while(sample_index--)
            {
                *output_sig++  = std::cos(2.f * dsp::pi * (m_time + fmodf(*phase++, 1.f)));
                m_time += (*freq++ / m_sr);
            }
            
            m_time = fmodf(m_time, 1.f);
        }
        
        // ================================================================================ //
        //                                       TIMES~                                       //
        // ================================================================================ //
        
        TimesTilde::TimesTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : AudioObject(model, patcher)
        {
            if (!args.empty() && args[0].isNumber())
            {
                m_value = args[0].getFloat();
            }
        }
        
        void TimesTilde::receive(size_t index, std::vector<Atom> const& args)
        {
            if(index == 1 && args[0].isNumber())
            {
                m_value = args[0].getFloat();
            }
        }
        
        void TimesTilde::performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            const size_t size = input[0ul].size();
            dsp::sample_t const* in1 = input[0ul].data();
            dsp::sample_t* out = output[0ul].data();
            
            dsp::sample_t const value = m_value;
            
            for(size_t i = size>>3; i; --i, in1 += 8, out += 8)
            {
                out[0] = in1[0] * value; out[1] = in1[1] * value;
                out[2] = in1[2] * value; out[3] = in1[3] * value;
                out[4] = in1[4] * value; out[5] = in1[5] * value;
                out[6] = in1[6] * value; out[7] = in1[7] * value;
            }
            for(size_t i = size&7; i; --i, in1++, out++)
            {
                out[0] = in1[0] * value;
            }
        }
        
        void TimesTilde::performVec(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            const size_t size = input[0ul].size();
            dsp::sample_t const* in1 = input[0ul].data();
            dsp::sample_t const* in2 = input[1ul].data();
            dsp::sample_t* out = output[0ul].data();
            
            for(size_t i = size>>3; i; --i, in1 += 8, in2 += 8, out += 8)
            {
                out[0] = in1[0] * in2[0]; out[1] = in1[1] * in2[1];
                out[2] = in1[2] * in2[2]; out[3] = in1[3] * in2[3];
                out[4] = in1[4] * in2[4]; out[5] = in1[5] * in2[5];
                out[6] = in1[6] * in2[6]; out[7] = in1[7] * in2[7];
            }
            for(size_t i = size&7; i; --i, in1++, in2++, out++)
            {
                out[0] = in1[0] * in2[0];
            }
        }
        
        void TimesTilde::prepare(PrepareInfo const& infos)
        {
            if (infos.inputs.size() > 1 && infos.inputs[1])
            {
                setPerformCallBack(this, &TimesTilde::performVec);
            }
            else
            {
                setPerformCallBack(this, &TimesTilde::performValue);
            }
        }
        
        // ================================================================================ //
        //                                       PLUS~                                      //
        // ================================================================================ //
        
        PlusTilde::PlusTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : AudioObject(model, patcher)
        {
            if (!args.empty() && args[0].isNumber())
            {
                m_value = args[0].getFloat();
            }
        }
        
        void PlusTilde::receive(size_t index, std::vector<Atom> const& args)
        {
            if(index == 1 && args[0].isNumber())
            {
                m_value = args[0].getFloat();
            }
        }
        
        void PlusTilde::performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            dsp::Signal const& in = input[0];
            const size_t size = in.size();
            dsp::sample_t const* in1 = in.data();
            dsp::sample_t* out = output[0].data();
            
            dsp::sample_t const value = m_value;
            
            for(size_t i = size>>3; i; --i, in1 += 8, out += 8)
            {
                out[0] = in1[0] + value; out[1] = in1[1] + value;
                out[2] = in1[2] + value; out[3] = in1[3] + value;
                out[4] = in1[4] + value; out[5] = in1[5] + value;
                out[6] = in1[6] + value; out[7] = in1[7] + value;
            }
            for(size_t i = size&7; i; --i, in1++, out++)
            {
                out[0] = in1[0] * value;
            }
        }
        
        void PlusTilde::performVec(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            dsp::Signal const& in = input[0];
            const size_t size = in.size();
            dsp::sample_t const* in1 = in.data();
            dsp::sample_t const* in2 = input[1].data();
            dsp::sample_t* out = output[0].data();
            
            for(size_t i = size>>3; i; --i, in1 += 8, in2 += 8, out += 8)
            {
                out[0] = in1[0] + in2[0]; out[1] = in1[1] + in2[1];
                out[2] = in1[2] + in2[2]; out[3] = in1[3] + in2[3];
                out[4] = in1[4] + in2[4]; out[5] = in1[5] + in2[5];
                out[6] = in1[6] + in2[6]; out[7] = in1[7] + in2[7];
            }
            for(size_t i = size&7; i; --i, in1++, in2++, out++)
            {
                out[0] = in1[0] * in2[0];
            }
        }
        
        void PlusTilde::prepare(PrepareInfo const& infos)
        {
            if (infos.inputs.size() > 1 && infos.inputs[1])
            {
                setPerformCallBack(this, &PlusTilde::performVec);
            }
            else
            {
                setPerformCallBack(this, &PlusTilde::performValue);
            }
        }
        
        // ================================================================================ //
        //                                       SIG~                                       //
        // ================================================================================ //
        
        SigTilde::SigTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args):
        AudioObject(model, patcher)
        {
            if (!args.empty() && args[0].isNumber())
            {
                m_value = args[0].getFloat();
            }
        }
        
        void SigTilde::receive(size_t index, std::vector<Atom> const& args)
        {
            if (index == 0 && args[0].isNumber())
            {
                m_value = args[0].getFloat();
            }
        }
        
        void SigTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
        {
            size_t sample_index = output[0].size();
            dsp::sample_t* output_sig = output[0].data();
            dsp::sample_t const value = m_value;
            
            while(sample_index--)
            {
                *output_sig++  = value;
            }
        }
        
        void SigTilde::prepare(dsp::Processor::PrepareInfo const& infos)
        {
            setPerformCallBack(this, &SigTilde::perform);
        }
    }
}


#endif // KIWI_ENGINE_OBJECTS_HPP_INCLUDED
