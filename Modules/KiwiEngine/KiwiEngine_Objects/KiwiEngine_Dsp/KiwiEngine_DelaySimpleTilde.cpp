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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Dsp/KiwiEngine_DelaySimpleTilde.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                  DELAYSIMPLETILDE                                //
        // ================================================================================ //
        
        DelaySimpleTilde::DelaySimpleTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args):
        AudioObject(model, patcher),
        m_circular_buffer(),
        m_reinject_signal(),
        m_max_delay(60.),
        m_delay(1.),
        m_reinject_level(0.),
        m_sr(0.),
        m_vector_size(0),
        m_mutex()
        {
        }
        
        void DelaySimpleTilde::receive(size_t index, std::vector<Atom> const& args)
        {
            if (index == 0 && args[0].isString())
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                
                m_circular_buffer->assign(m_circular_buffer->size(), 0);
            }
            if (index == 1 && args[0].isNumber())
            {
                m_delay.store(args[0].getFloat() / 1000.);
            }
            else if(index == 2 && args[0].isNumber())
            {
                m_reinject_level.store(std::max(0., std::min(1., args[0].getFloat())));
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
            std::lock_guard<std::mutex> lock(m_mutex);
            
            size_t buffer_size = input[0].size();
            
            for (int i = 0; i < buffer_size; ++i)
            {
                m_circular_buffer->push_back(input[0][i] + m_reinject_signal->operator[](i));
            }
            
            float delay = std::max<float>(1. / m_sr, std::min<float>(m_delay.load(), m_max_delay));
            float offset = m_circular_buffer->size() - (delay * m_sr) - (buffer_size - 1);
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
            //! Durty solution to synchronize circular buffer.
            //! Should be implemented as an atomic shared pointer and a release pool
            //! https://www.youtube.com/watch?v=boPEO2auJj4 48'
            std::lock_guard<std::mutex> lock(m_mutex);
            
            size_t buffer_size = input[0].size();
            
            for (int i = 0; i < buffer_size; ++i)
            {
                m_circular_buffer->push_back(input[0][i] + m_reinject_signal->operator[](i));
            }
            
            for(int i = 0; i < buffer_size; ++i)
            {
                float delay = std::max<float>(1. / m_sr, std::min<float>(input[1][i] / 1000., m_max_delay));
                float offset = m_circular_buffer->size() - (delay * m_sr) - (buffer_size - 1) + i;
                size_t offset_floor = std::floor(offset);
                
                output[0][i] = cubicInterpolate(offset - offset_floor,
                                                m_circular_buffer->operator[](offset_floor - 1),
                                                m_circular_buffer->operator[](offset_floor),
                                                m_circular_buffer->operator[](offset_floor + 1),
                                                m_circular_buffer->operator[](offset_floor + 2));
                
                m_reinject_signal->operator[](i) = m_reinject_level.load() * output[0][i];
            }
        }
        
        void DelaySimpleTilde::prepare(dsp::Processor::PrepareInfo const& infos)
        {
            if (infos.sample_rate != m_sr || infos.vector_size != m_vector_size)
            {
                m_sr = infos.sample_rate;
                m_vector_size = infos.vector_size;
                
                size_t buffer_size = std::ceil(m_max_delay * m_sr) + 1 + m_vector_size;
                
                m_circular_buffer.reset(new CircularBuffer<dsp::sample_t>(buffer_size, buffer_size, 0.));
                m_reinject_signal.reset(new dsp::Signal(m_vector_size));
            }
            
            if (infos.inputs.size() > 1 && infos.inputs[1])
            {
                setPerformCallBack(this, &DelaySimpleTilde::performDelay);
            }
            else
            {
                setPerformCallBack(this, &DelaySimpleTilde::perform);
            }
        }
    }
}
