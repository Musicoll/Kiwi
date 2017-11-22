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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Message/KiwiEngine_Receive.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                  OBJECT RECEIVE                                  //
        // ================================================================================ //
        
        class Receive::Task final : public tool::Scheduler<>::Task
        {
        public: // methods
            
            Task(Receive& object, std::vector<tool::Atom> const& atoms):
            tool::Scheduler<>::Task(),
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
                                                    [this](std::shared_ptr<Task> const& task)
                                                    {
                                                        return task.get() == this;
                                                    }));
            }
            
        private: // members
            
            Receive&            m_object;
            std::vector<tool::Atom>   m_atoms;
        };
        
        Receive::Receive(model::Object const& model, Patcher& patcher, std::vector<tool::Atom> const& args):
        Object(model, patcher),
        m_name(),
        m_tasks()
        {
            m_name = !args.empty() ? args[0].getString() : "";
            
            if(!m_name.empty())
            {
                tool::Beacon& beacon = getBeacon(m_name);
                beacon.bind(*this);
            }
        }
        
        Receive::~Receive()
        {
            for (std::shared_ptr<Task> const& task : m_tasks)
            {
                getScheduler().unschedule(task);
            }
            
            if(!m_name.empty())
            {
                tool::Beacon& beacon = getBeacon(m_name);
                beacon.unbind(*this);
            }
        }
        
        void Receive::receive(size_t, std::vector<tool::Atom> const& args)
        {
            
        }
        
        void Receive::receive(std::vector<tool::Atom> const& args)
        {
            if (!args.empty())
            {
                std::shared_ptr<Task> task(new Task(*this, args));
                
                m_tasks.insert(task);
                
                getScheduler().defer(task);
            }
        }
    }
}

