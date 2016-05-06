/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#ifndef KIWI_ENGINE_OBJECTS_OBJECTPLUS_HPP_INCLUDED
#define KIWI_ENGINE_OBJECTS_OBJECTPLUS_HPP_INCLUDED

#include "../KiwiLink.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
        
        class ObjectPlus : public engine::Object
        {
        public:
            
            ObjectPlus(model::ObjectPlus& model) :
            m_lhs(1.0), m_rhs(0.0)
            {
                ;
            }

            ObjectPlus(std::vector<Atom> args) :
            m_lhs(1.0), m_rhs(0.0)
            {
                ;
            }
            
            void receive(uint32_t index, std::vector<Atom> args) override
            {
                if(args.size() > 0)
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
                    else if(index == 0
                            && args[0].isString()
                            && (args[0].getString() == "bang"))
                    {
                        bang();
                    }
                }
            }
            
            void bang()
            {
                send(0, {m_rhs + m_lhs});
            }
            
            void signalTriggerCalled() override
            {
                bang();
            }
            
        private:
            double m_lhs = 1.0;
            double m_rhs = 0.0;
        };
    }
}


#endif // KIWI_MODEL_OBJECTS_OBJECTPLUS_HPP_INCLUDED
