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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Message/KiwiEngine_Print.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                    OBJECT PRINT                                  //
        // ================================================================================ //
        
        Print::Print(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args)
        : Object(model, patcher)
        {
            m_name = !args.empty() ? args[0].getString() : "print";
        }
        
        void Print::receive(size_t, std::vector<Atom> const& args)
        {
            if(!args.empty())
            {
                post(m_name + " \xe2\x80\xa2 " + AtomHelper::toString(args));
            }
        }
    }
}

