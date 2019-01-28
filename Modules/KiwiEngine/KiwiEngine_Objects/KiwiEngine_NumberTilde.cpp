/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Number.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_NumberTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT NUMBER TILDE                             //
    // ================================================================================ //
    
    void NumberTilde::declare()
    {
        Factory::add<NumberTilde>("number~", &NumberTilde::create);
    }
    
    std::unique_ptr<Object> NumberTilde::create(model::Object const& model, Patcher& patcher)
    {
        return std::make_unique<NumberTilde>(model, patcher);
    }
    
    NumberTilde::NumberTilde(model::Object const& object_model, Patcher& patcher):
    AudioObject(object_model, patcher),
    tool::Scheduler<>::Timer(patcher.getScheduler()),
    m_value(object_model.getParameter("value")[0].getFloat()),
    m_interval(100)
    {
    }
    
    void NumberTilde::perform(dsp::Buffer const& input, dsp::Buffer& output)
    {
        m_value.store(input[0][input[0].size() - 1]);
    }
    
    void NumberTilde::prepare(dsp::Processor::PrepareInfo const& infos)
    {
        if(infos.inputs[0])
        {
            setPerformCallBack(this, &NumberTilde::perform);
        }
        
        startTimer(std::chrono::milliseconds(m_interval));
    }
    
    void NumberTilde::release()
    {
        stopTimer();
    }
    
    void NumberTilde::timerCallBack()
    {
        double current_value = m_value.load();
        
        setParameter("value", tool::Parameter(tool::Parameter::Type::Float, {current_value}));
        
        send(0,{current_value});
    }
    
    void NumberTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
    }
}}

