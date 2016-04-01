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

#ifndef KIWI_CONTROLLERS_OBJECTS_OBJECTPLUS_HPP_INCLUDED
#define KIWI_CONTROLLERS_OBJECTS_OBJECTPLUS_HPP_INCLUDED

#include "../KiwiLink.hpp"

namespace kiwi
{
    namespace controller
    {
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
        
        class ObjectPlus : public controller::Object
        {
        public:

            ObjectPlus(model::ObjectPlus& model, std::vector<Atom> const& args) : controller::Object(model)
            {
                if(args.size() > 0 && args[0].isNumber())
                {
                    m_operand = args[0].getFloat();
                }
                
                std::cout << "m_operand = " << m_operand << '\n';
            }
            
            void receive(uint32_t index, std::vector<Atom> args) override
            {
                if(args.size() > 0)
                {
                    if(args[0].isNumber())
                    {
                        if(index == 0)
                        {
                            m_current_value = m_operand + args[0].getFloat();
                            bang();
                        }
                        else if(index == 1)
                        {
                            m_operand = args[0].getFloat();
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
                send(0, {m_current_value});
            }
            
        private:
            double m_operand = 0.0;
            double m_current_value = 0.0;
        };
    }
}


#endif // KIWI_MODEL_OBJECTS_OBJECTPLUS_HPP_INCLUDED
