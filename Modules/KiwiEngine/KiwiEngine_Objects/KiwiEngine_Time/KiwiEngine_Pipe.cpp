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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Time/KiwiEngine_Pipe.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                  OBJECT PIPE                                     //
        // ================================================================================ //
        
        class Pipe::Task final : public Scheduler<>::Task
        {
        public: // methods
            
            Task(Pipe & object, std::vector<Atom> const& atoms):
            Scheduler<>::Task(),
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
                                                    [this](std::shared_ptr<Task> const& task)
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
            for (std::shared_ptr<Task> const& task : m_tasks)
            {
                getScheduler().unschedule(task);
            }
        }
        
        void Pipe::receive(size_t index, std::vector<Atom> const& args)
        {
            if (!args.empty())
            {
                if (index == 0)
                {
                    std::shared_ptr<Task> task(new Task(*this, args));
                    getScheduler().schedule(task, m_delay);
                    m_tasks.insert(std::move(task));
                }
                else if(index == 1 && args[0].isNumber())
                {
                    m_delay = std::chrono::milliseconds(args[0].getInt());
                }
            }
        }
    }
}

