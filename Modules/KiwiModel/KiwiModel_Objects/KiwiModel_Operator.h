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

#pragma once

#include <KiwiModel/KiwiModel_Objects/KiwiModel_OperatorTilde.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    OPERATOR                                      //
    // ================================================================================ //
    
    class Operator : public model::Object
    {
    public: // methods
        
        Operator(flip::Default& d) : model::Object(d) {}
        
        Operator(std::vector<tool::Atom> const& args);
        
        std::string getIODescription(bool is_inlet, size_t index) const override;
        
        static void declare();
    };
    
}}
