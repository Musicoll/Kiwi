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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Bang.h>

#include <KiwiTool/KiwiTool_Scheduler.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Controller/KiwiEngine_Bang.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                  OBJECT BANG                                     //
        // ================================================================================ //
        
        class Bang::Task : public tool::Scheduler<>::Task
        {
        public: // methods
            
            Task(Bang & object):
            m_object(object)
            {
            }
            
            ~Task()
            {
            }
            
            void execute() override final
            {
                m_object.send(0, {"bang"});
            }
            
        private: // members
            
            Bang & m_object;
        };
        
        Bang::Bang(model::Object const& model, Patcher& patcher, std::vector<tool::Atom> const& args):
        Object(model, patcher),
        m_task(new Task(*this)),
        m_signal(model.getSignal<>(model::Bang::Signal::TriggerBang)),
        m_connection(m_signal.connect(std::bind(&Bang::signalTriggered, this)))
        {
        }
        
        Bang::~Bang()
        {
            getScheduler().unschedule(m_task);
        }
        
        void Bang::signalTriggered()
        {
            getScheduler().defer(m_task);
        }
        
        void Bang::receive(size_t index, std::vector<tool::Atom> const& args)
        {
            if (index == 0)
            {
                m_signal();
            }
        }
    }
}

