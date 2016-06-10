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

#ifndef KIWI_ENGINE_OBJECTS_HPP_INCLUDED
#define KIWI_ENGINE_OBJECTS_HPP_INCLUDED

#include "KiwiTypedObjects.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                       NEWBOX                                     //
        // ================================================================================ //
        
        NewBox::NewBox(std::vector<Atom> const& args)
        {
            ;
        }
        
        void NewBox::receive(size_t index, std::vector<Atom> const& args)
        {
            ;
        }
        
        // ================================================================================ //
        //                                      ERRORBOX                                    //
        // ================================================================================ //
        
        ErrorBox::ErrorBox(std::vector<Atom> const& args)
        {
            ;
        }
        
        void ErrorBox::receive(size_t index, std::vector<Atom> const& args)
        {
            ;
        }
        
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
        
        ObjectPlus::ObjectPlus(std::vector<Atom> const& args)
        {
            m_lhs = 0.;
            if(!args.empty() && args[0].isNumber())
            {
                m_rhs = args[0].getFloat();
            }
        }
        
        void ObjectPlus::receive(size_t index, std::vector<Atom> const& args)
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
        
        void ObjectPlus::bang()
        {
            send(0, {m_rhs + m_lhs});
        }
        
        void ObjectPlus::signalTriggerCalled()
        {
            bang();
        }
        
        // ================================================================================ //
        //                                    OBJECT PRINT                                  //
        // ================================================================================ //
        
        ObjectPrint::ObjectPrint(std::vector<Atom> const& args)
        {
            m_name = !args.empty() ? args[0].getString() : "print";
        }
        
        void ObjectPrint::receive(size_t, std::vector<Atom> const& args)
        {
            if(!args.empty())
            {
                Console::post(m_name + " \u2022 " + AtomHelper::toString(args));
            }
        }
    }
}


#endif // KIWI_ENGINE_OBJECTS_HPP_INCLUDED
