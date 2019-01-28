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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Comment.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                  OBJECT COMMENT                                  //
    // ================================================================================ //
    
    void Comment::declare()
    {
        Factory::add<Comment>("comment", &Comment::create);
    }
    
    std::unique_ptr<Object> Comment::create(model::Object const& model, Patcher& patcher)
    {
        return std::make_unique<Comment>(model, patcher);
    }
    
    Comment::Comment(model::Object const& object_model, Patcher& patcher):
    Object(object_model, patcher)
    {
    }
    
    
    Comment::~Comment()
    {
    }
    
    void Comment::receive(size_t index, std::vector<tool::Atom> const& args)
    {
    }
    
}}

