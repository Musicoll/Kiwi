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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_LineTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi{ namespace engine {
    
    // ================================================================================ //
    //                                      RAMP                                        //
    // ================================================================================ //
    
    Ramp::Ramp(dsp::sample_t start) noexcept
    : m_current_value(start)
    , m_destination_value(start)
    {
    }
    
    void Ramp::setSampleRate(double sample_rate) noexcept
    {
        //! Note: this will only affect next value-time pairs.
        m_sr = sample_rate;
    }
    
    void Ramp::setEndOfRampCallback(std::function<void()> callback)
    {
        m_ramp_ended_callback = callback;
    }
    
    void Ramp::setValueDirect(dsp::sample_t new_value) noexcept
    {
        auto lock = std::unique_lock<std::mutex>(m_lock);
        reset();
        m_current_value = m_destination_value = new_value;
        m_countdown = m_steps_to_destination = 0;
        m_step = 0.;
    }
    
    void Ramp::setValueTimePairs(std::vector<ValueTimePair> value_time_pairs)
    {
        auto lock = std::unique_lock<std::mutex>(m_lock);
        reset();
        m_value_time_pairs.swap(value_time_pairs);
        m_valuetime_pairs_countdown = m_value_time_pairs.size();
    }
    
    dsp::sample_t Ramp::getNextValue() noexcept
    {
        auto lock = std::unique_lock<std::mutex>(m_lock);
        
        if (m_countdown <= 0)
        {
            const auto value = m_destination_value;
            triggerNextRamp();
            return value;
        }
        
        --m_countdown;
        m_current_value += m_step;
        return m_current_value;
    }
    
    dsp::sample_t Ramp::getValue() noexcept
    {
        return m_current_value;
    }
    
    void Ramp::triggerNextRamp()
    {
        if(m_valuetime_pairs_countdown > 0)
        {
            assert(m_valuetime_pairs_countdown <= m_value_time_pairs.size());
            
            const size_t index = m_value_time_pairs.size() - m_valuetime_pairs_countdown;
            setNextValueTime(m_value_time_pairs[index]);
            --m_valuetime_pairs_countdown;
            m_should_notify_end = (m_valuetime_pairs_countdown == 0);
        }
        else if(m_should_notify_end && m_ramp_ended_callback)
        {
            m_ramp_ended_callback();
            m_should_notify_end = false;
        }
    }
    
    void Ramp::setNextValueTime(ValueTimePair const& value_time) noexcept
    {
        m_countdown = m_steps_to_destination = std::floor(value_time.time_ms * 0.001 * m_sr);
        m_destination_value = value_time.value;
        
        if (m_countdown <= 0)
        {
            m_current_value = m_destination_value;
            m_step = 0.;
        }
        else
        {
            m_step = (m_destination_value - m_current_value) / (dsp::sample_t) m_countdown;
        }
    }
    
    void Ramp::reset()
    {
        m_value_time_pairs.clear();
        m_valuetime_pairs_countdown = 0;
        m_destination_value = m_current_value;
        m_countdown = 0;
        m_should_notify_end = false;
    }
    
    // ================================================================================ //
    //                                   LINE~ TASK                                     //
    // ================================================================================ //
    
    class LineTilde::BangTask : public tool::Scheduler<>::Task
    {
    public: // methods
        
        BangTask(LineTilde& owner) : m_owner(owner) {}
        ~BangTask() = default;
        void execute() override { m_owner.send(1ul, {"bang"}); }
        
    private: // members
        LineTilde& m_owner;
    };
    
    // ================================================================================ //
    //                                      LINE~                                       //
    // ================================================================================ //
    
    void LineTilde::declare()
    {
        Factory::add<LineTilde>("line~", &LineTilde::create);
    }
    
    std::unique_ptr<Object> LineTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<LineTilde>(model, patcher);
    }
    
    LineTilde::LineTilde(model::Object const& model, Patcher& patcher)
    : AudioObject(model, patcher)
    , m_bang_task(std::make_shared<BangTask>(*this))
    , m_ramp(0.)
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (!args.empty() && args[0].isNumber())
        {
            m_ramp.setValueDirect(args[0].getFloat());
        }
        
        m_ramp.setEndOfRampCallback([this]{
            getScheduler().defer(m_bang_task);
        });
    }
    
    LineTilde::~LineTilde()
    {
        getScheduler().unschedule(m_bang_task);
    }
    
    std::vector<Ramp::ValueTimePair>
    LineTilde::parseAtomsAsValueTimePairs(std::vector<tool::Atom> const& atoms) const
    {
        std::vector<Ramp::ValueTimePair> value_time_pairs;
        
        for(size_t i = 0; i < atoms.size(); i += 2)
        {
            auto const& time_atom = atoms[i+1];
            if(!time_atom.isNumber() || !atoms[i].isNumber())
            {
                error("line~ only accepts value-time numbers");
                return value_time_pairs;
            }
            
            if(time_atom.getFloat() < 0.)
            {
                error("line~ do not accepts negative ramp time");
                return value_time_pairs;
            }
            
            Ramp::ValueTimePair pair {
                (dsp::sample_t) atoms[i].getFloat(),
                (dsp::sample_t) time_atom.getFloat()
            };
            
            value_time_pairs.emplace_back(std::move(pair));
        }
        
        return value_time_pairs;
    }
    
    void LineTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (!args.empty())
        {
            if(args[0].isNumber())
            {
                if(index == 0)
                {
                    if(args.size() >= 2)
                    {
                        auto value_time_pairs = parseAtomsAsValueTimePairs(args);
                        
                        if(!value_time_pairs.empty())
                        {
                            m_ramp.setValueTimePairs(value_time_pairs);
                        }
                    }
                    else
                    {
                        if(!m_next_ramp_time_consumed)
                        {
                            Ramp::ValueTimePair pair {
                                (dsp::sample_t) args[0].getFloat(),
                                (dsp::sample_t) m_next_ramp_time_ms
                            };
                            
                            m_ramp.setValueTimePairs({std::move(pair)});
                            m_next_ramp_time_consumed = true;
                        }
                        else
                        {
                            m_ramp.setValueDirect(args[0].getFloat());
                        }
                    }
                }
                else if(index == 1)
                {
                    if (args[0].isNumber())
                    {
                        m_next_ramp_time_ms = args[0].getFloat();
                        m_next_ramp_time_consumed = false;
                    }
                }
            }
            else
            {
                warning("line~ inlet " + std::to_string(index + 1) + " parameter must be a number");
            }
            /* //! @todo
            else if(args[0].isString())
            {
                const auto str = args[0].getString();
                if(str == "stop")
                {
                    ;
                }
                else if(str == "pause")
                {
                    ;
                }
                else if(str == "resume")
                {
                    ;
                }
            }
            */
        }
    }
    
    void LineTilde::prepare(PrepareInfo const& infos)
    {
        m_ramp.setSampleRate((double) infos.sample_rate);
        setPerformCallBack(this, &LineTilde::perform);
    }
    
    void LineTilde::perform(dsp::Buffer const&, dsp::Buffer& output) noexcept
    {
        size_t sampleframes = output[0ul].size();
        dsp::sample_t* out = output[0ul].data();
        
        while(sampleframes--)
        {
            *out++ = m_ramp.getNextValue();
        }
    }
    
}}
