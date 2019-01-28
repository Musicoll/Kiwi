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

#include <KiwiEngine/KiwiEngine_Factory.h>

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_NewBox.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       NEWBOX                                     //
    // ================================================================================ //
    
    void NewBox::declare()
    {
        Factory::add<NewBox>("newbox", &NewBox::create);
    }
    
    std::unique_ptr<Object> NewBox::create(model::Object const& model, Patcher& patcher)
    {
        return std::make_unique<NewBox>(model, patcher);
    }
    
    NewBox::NewBox(model::Object const& model, Patcher& patcher)
    : Object(model, patcher)
    {
        ;
    }
    
    void NewBox::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        ;
    }  
}}
