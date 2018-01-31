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
#include <atomic>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_DelaySimpleTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  DELAYSIMPLETILDE                                //
    // ================================================================================ //
    
    void DelaySimpleTilde::declare()
    {
        Factory::add<DelaySimpleTilde>("delaysimple~", &DelaySimpleTilde::create);
    }

    std::unique_ptr<Object> DelaySimpleTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<DelaySimpleTilde>(model, patcher);
    }
    
    DelaySimpleTilde::DelaySimpleTilde(model::Object const& model, Patcher& patcher):
    AudioObject(model, patcher),
    tool::Scheduler<>::Timer(patcher.getScheduler()),
    m_circular_buffer(),
    m_reinject_signal(),
    m_max_delay(60.),
    m_delay(1.),
    m_reinject_level(0.),
    m_sr(0.),
    m_pool()
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (args.size() > 0)
        {
            m_delay = (args[0].getFloat() / 1000.);
        }
        
        if (args.size() > 1)
        {
            m_reinject_level = std::max(0., std::min(args[1].getFloat(), 1.));
        }
        
        std::shared_ptr<CircularBuffer> buffer(new CircularBuffer(0., 0., 0.));
        m_pool.add(buffer);
        store(buffer);
        
        startTimer(std::chrono::milliseconds(1000));
    }
    
    DelaySimpleTilde::~DelaySimpleTilde()
    {
        stopTimer();
        timerCallBack();
    }
    
    void DelaySimpleTilde::timerCallBack()
    {
        m_pool.clear();
    }
    
    std::shared_ptr<DelaySimpleTilde::CircularBuffer> DelaySimpleTilde::load()
    {
        std::shared_ptr<CircularBuffer> load_buffer;
        
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            load_buffer = m_circular_buffer;
        }
        
        return load_buffer;
    }
    
    void DelaySimpleTilde::store(std::shared_ptr<CircularBuffer> new_buffer)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_circular_buffer = new_buffer;
    }
    
    void DelaySimpleTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0 && args[0].isString())
        {
            if (args[0].isString() && args[0].getString() == "clear")
            {
                std::shared_ptr<CircularBuffer> new_buffer(new CircularBuffer(m_circular_buffer->size(),
                                                                              m_circular_buffer->size(),
                                                                              0.));
                m_pool.add(new_buffer);
                store(new_buffer);
            }
            else
            {
                warning("delaysimple~ inlet 1 doesn't understand + [" + args[0].getString() + "]");
            }
        }
        if (index == 1)
        {
            if (args[0].isNumber())
            {
                m_delay.store(args[0].getFloat() / 1000.);
            }
            else
            {
                warning("delaysimple~ inlet 2 requires a number");
            }
        }
        else if(index == 2)
        {
            if (args[0].isNumber())
            {
                m_reinject_level.store(std::max(0., std::min(1., args[0].getFloat())));
            }
            else
            {
                warning("delaysimple~ inlet 3 requires a number");
            }
        }
    }
    
    dsp::sample_t DelaySimpleTilde::cubicInterpolate(float const& x,
                                                     float const& y0,
                                                     float const& y1,
                                                     float const& y2,
                                                     float const& y3)
    {
        return y1 + 0.5 * x * (y2 - y0 + x * (2.0 * y0 - 5.0 * y1 + 4.0 * y2 - y3
                                              + x * (3.0 * (y1 - y2) + y3 - y0)));
    }
    
    void DelaySimpleTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        std::shared_ptr<CircularBuffer> buffer = load();
        
        size_t buffer_size = input[0].size();
        
        for (int i = 0; i < buffer_size; ++i)
        {
            buffer->push_back(input[0][i] + m_reinject_signal->operator[](i));
        }
        
        float delay = std::max<float>(1. / m_sr, std::min<float>(m_delay.load(), m_max_delay));
        float offset = buffer->size() - (delay * m_sr) - (buffer_size - 1);
        size_t offset_floor = std::floor(offset);
        float decimal_part = offset - offset_floor;
        
        for(int i = 0; i < buffer_size; ++i)
        {
            output[0][i] = cubicInterpolate(decimal_part,
                                            m_circular_buffer->operator[](offset_floor - 1),
                                            m_circular_buffer->operator[](offset_floor),
                                            m_circular_buffer->operator[](offset_floor + 1),
                                            m_circular_buffer->operator[](offset_floor + 2));
            
            m_reinject_signal->operator[](i) = m_reinject_level.load() * output[0][i];
            ++offset_floor;
        }
    }
    
    void DelaySimpleTilde::performDelay(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        std::shared_ptr<CircularBuffer> buffer = load();
        
        size_t buffer_size = input[0].size();
        
        for (int i = 0; i < buffer_size; ++i)
        {
            buffer->push_back(input[0][i] + m_reinject_signal->operator[](i));
        }
        
        for(int i = 0; i < buffer_size; ++i)
        {
            float delay = std::max<float>(1. / m_sr, std::min<float>(input[1][i] / 1000., m_max_delay));
            float offset = buffer->size() - (delay * m_sr) - (buffer_size - 1) + i;
            size_t offset_floor = std::floor(offset);
            
            output[0][i] = cubicInterpolate(offset - offset_floor,
                                            buffer->operator[](offset_floor - 1),
                                            buffer->operator[](offset_floor),
                                            buffer->operator[](offset_floor + 1),
                                            buffer->operator[](offset_floor + 2));
            
            m_reinject_signal->operator[](i) = m_reinject_level.load() * output[0][i];
        }
    }
    
    void DelaySimpleTilde::prepare(dsp::Processor::PrepareInfo const& infos)
    {
        m_sr = infos.sample_rate;
        size_t vector_size = infos.vector_size;
        
        size_t buffer_size = std::ceil(m_max_delay * m_sr) + 1 + vector_size;
        
        std::shared_ptr<CircularBuffer> new_buffer(new CircularBuffer(buffer_size, buffer_size, 0.));
        m_pool.add(new_buffer);
        store(new_buffer);
        
        m_reinject_signal.reset(new dsp::Signal(vector_size));
        
        if (infos.inputs.size() > 1 && infos.inputs[1])
        {
            setPerformCallBack(this, &DelaySimpleTilde::performDelay);
        }
        else
        {
            setPerformCallBack(this, &DelaySimpleTilde::perform);
        }
    }
    
    // ================================================================================ //
    //                                  RELEASEPOOL                                     //
    // ================================================================================ //
    
    DelaySimpleTilde::ReleasePool::ReleasePool():
    m_pool(),
    m_mutex()
    {
    }
    
    DelaySimpleTilde::ReleasePool::~ReleasePool()
    {
    }
    
    void DelaySimpleTilde::ReleasePool::add(std::shared_ptr<DelaySimpleTilde::CircularBuffer> & buffer)
    {
        if (buffer != nullptr)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            
            m_pool.emplace_back(buffer);
        }
    }
    
    void DelaySimpleTilde::ReleasePool::clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        for (auto it = m_pool.begin(); it != m_pool.end();)
        {
            if (it->use_count() <= 1)
            {
                it = m_pool.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    
}}
