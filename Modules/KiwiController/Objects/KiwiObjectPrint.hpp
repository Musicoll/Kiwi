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

#ifndef KIWI_CONTROLLERS_OBJECTS_OBJECTPRINT_HPP_INCLUDED
#define KIWI_CONTROLLERS_OBJECTS_OBJECTPRINT_HPP_INCLUDED

#include "KiwiObjectPlus.hpp"

namespace kiwi
{
    namespace controller
    {
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
        
        class ObjectPrint : public controller::Object
        {
        public:

            ObjectPrint(model::ObjectPrint& model, std::vector<Atom> const& args) : controller::Object(model)
            {
                m_print_prefix = (args.size() > 0) ? AtomHelper::toString(args[0]) : "print";
            }
            
            void receive(uint32_t, std::vector<Atom> args) override
            {
                if(!args.empty())
                {
                    std::cout << "\u2022 " << m_print_prefix << " : "
                    << AtomHelper::toString(args) << '\n';
                }
            }
            
        private:
            std::string m_print_prefix;
        };
    }
}


#endif // KIWI_CONTROLLERS_OBJECTS_OBJECTPRINT_HPP_INCLUDED
