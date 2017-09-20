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

#include <functional>
#include <algorithm>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Slider.h>

#include <KiwiEngine/KiwiEngine_Scheduler.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Controller/KiwiEngine_Slider.h>

namespace kiwi { namespace engine {
    
    class Slider::Task final : public Scheduler<>::Task
    {
    public: // methods
        
        Task(double output_value, Slider & slider):
        m_output_value(output_value),
        m_slider(slider)
        {
        }
        
        ~Task()
        {
        }
        
        void execute()
        {
            m_slider.send(0, {m_output_value});
        }
        
    private: // members
        
        double      m_output_value;
        Slider &    m_slider;
    };
    
    // ================================================================================ //
    //                                  OBJECT SLIDER                                   //
    // ================================================================================ //
    
    Slider::Slider(model::Object const& object_model, Patcher& patcher, std::vector<Atom> const& args):
    Object(object_model, patcher),
    m_signal(object_model.getSignal<double>(model::Slider::Signal::ValueChanged)),
    m_connection(m_signal.connect(std::bind(&Slider::valueChanged, this, std::placeholders::_1))),
    m_tasks(100)
    {
    }
    
    
    Slider::~Slider()
    {
        while(m_tasks.load_size() > 0)
        {
            std::shared_ptr<Task> task;
            m_tasks.pop(task);
            getScheduler().unschedule(task);
        }
    }
    
    void Slider::valueChanged(double new_value)
    {
        std::shared_ptr<Task> task(new Task(new_value, *this));
        m_tasks.push(task);
        getScheduler().schedule(task);
    }
    
    void Slider::receive(size_t index, std::vector<Atom> const& args)
    {
        if (index == 0 && !args.empty())
        {
            if (args[0].isNumber())
            {
                m_signal(std::max(0., std::min(args[0].getFloat(), 1.)));
            }
        }
    }
    
}}

